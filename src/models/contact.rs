use diesel::{prelude::*, Insertable, Queryable};
use diesel::result::Error::DatabaseError;
use crate::schema::contacts::{
    self,
    dsl::contacts as contact_table,
};
use serde::{Serialize, Deserialize};


//For queries
#[derive(Queryable, Identifiable, Selectable, Serialize)]
#[diesel(check_for_backend(diesel::pg::Pg))]
pub struct Contact {
    pub id: i32,
    pub name: String,
    pub first_name: String,
    pub phone: String,
}



//To insert a contact
#[derive(Insertable, Deserialize)]
#[diesel(table_name = contacts)]
pub struct NewContact {
    pub id: i32,
    pub name: String,
    pub first_name : String,
    pub phone : String
}

impl Contact {

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



}