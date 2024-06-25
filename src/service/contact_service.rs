use actix_web::web;
use crate::config::db::Pool;
use crate::models::contact::{NewContact,Contact};

//ADD
pub fn create_contact(new_contact: NewContact, pool: &web::Data<Pool>) -> Result<Contact, String> {
    return Contact::create_contact(new_contact, &mut pool.get().unwrap())
}

//UPDATE
pub fn update_contact(new_contact: NewContact , id: i32,  pool: &web::Data<Pool>) -> Result<Contact,String> {
    return Contact::update_contact(new_contact, id, &mut pool.get().unwrap());
}

//DELETE
pub fn delete_contact(id: i32,  pool: &web::Data<Pool>) -> Result<usize,String> {
    return Contact::delete_contact(id, &mut pool.get().unwrap());
}

//LIST ALL CONTACTS
pub fn list_all_contacts(pool: &web::Data<Pool>) -> Result<Vec<Contact>, String> {
    return Contact::list_all_contacts(&mut pool.get().unwrap());
}