use diesel::{prelude::*, Insertable, Queryable};
use diesel::result::Error::DatabaseError;
use crate::schema::users::username;
use crate::schema::users::{
    self,
    dsl::users as user_table
};

use serde::{Serialize, Deserialize};

//To insert a contact
#[derive(Insertable, Deserialize,Debug)]
#[diesel(table_name = users)]
pub struct NewUser {
    pub username : String,
    pub password : String
}


//For CRUD Objects
#[derive(Queryable, Identifiable, Selectable, Serialize)]
#[diesel(check_for_backend(diesel::pg::Pg))]
pub struct User {
    pub id : i32,
    pub username : String,
    pub password : String
}

impl User {

    pub fn create_user(new_user: NewUser, conn: &mut PgConnection)
    -> Result<User, String> {
        
        let result: QueryResult<User> = new_user.insert_into(user_table).get_result(conn);
        match result {
            Ok(user) => Ok(user),
            Err(err) => match err {
                DatabaseError(_kind, info) =>
                    Err(info.message().to_owned()),
                _ => Err(format!("unknown error create_user()"))
            }
        }
    }


    pub fn get_user_by_username_and_password(username_param : &str , conn: &mut PgConnection) 
    -> Result<User, String>{

        let result = user_table
            .filter(username.eq(username_param))
            .first::<User>(conn);
        
        match result {
            Ok(user) => Ok(user),
            Err(err) => match err {
                DatabaseError(_kind, info) => Err(info.message().to_owned()),
                _ => Err(format!("unknown error in get_user_by_username()")),
            },
        }


       

        //user_table.filter(username.eq(username_param)).first::<User>(conn)


    }



}


