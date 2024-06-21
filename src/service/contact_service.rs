use actix_web::web;
use crate::config::db::Pool;
use crate::models::contact::{NewContact,Contact};

pub fn create_contact(new_contact: NewContact, pool: &web::Data<Pool>) -> Result<Contact, String> {
    return Contact::create_contact(new_contact, &mut pool.get().unwrap())
}