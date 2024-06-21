use actix_web::{HttpResponse, post, web::{Data, Json}};
use crate::config::db::Pool;
use crate::models::contact::{NewContact,Contact};
use crate::service::contact_service;


#[post("/add")]
pub async fn create_contact(request: Json<NewContact>, pool: Data<Pool>) -> HttpResponse {

    println!("Creando contacto !!");
    println!("{:?}",request);
    
    let result: Result<Contact, String> =  contact_service::create_contact(request.0, &pool);
    match result {
        Ok(contact) => HttpResponse::Ok().json(contact),
        Err(message) => HttpResponse::BadRequest().body(message)
    }
}