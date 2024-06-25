use diesel::{prelude::*, Insertable, Queryable};
use diesel::result::Error::DatabaseError;
use crate::schema::contacts::{
    self,
    dsl::contacts as contact_table,
    name,
    first_name,
    phone
};
use serde::{Serialize, Deserialize};


//To insert a contact
#[derive(Insertable, Deserialize,Debug)]
#[diesel(table_name = contacts)]
pub struct NewContact {
    //pub id: i32,  //Avoid send ID on JSON Object
    pub name: String,
    pub first_name : String,
    pub phone : String
}

//For CRUD
#[derive(Queryable, Identifiable, Selectable, Serialize, AsChangeset)]
#[diesel(check_for_backend(diesel::pg::Pg))]
pub struct Contact {
    pub id: i32,
    pub name: String,
    pub first_name: String,
    pub phone: String,
}
impl Contact {

    // ADD
    pub fn create_contact(new_contact: NewContact, conn: &mut PgConnection) -> Result<Contact, String> {

        let result: QueryResult<Contact> = new_contact.insert_into(contact_table).get_result(conn);
        match result {
            Ok(author) => Ok(author),
            Err(err) => match err {
                DatabaseError(_kind, info) =>
                    Err(info.message().to_owned()),
                _ => Err(format!("unknown error"))
            }
        }
    }


    //UPDATE
    pub fn update_contact(new_contact: NewContact, id: i32 , pool: &mut PgConnection) -> Result<Contact,String> {

        Ok(
            diesel::update( contact_table.find(id))
            .set(&Contact {
                        id,
                        name : new_contact.name,
                        first_name: new_contact.first_name,
                        phone: new_contact.phone,
                })
            .returning(Contact::as_returning())
            .get_result(pool)
            .expect("Error updating contact"))

    }



}