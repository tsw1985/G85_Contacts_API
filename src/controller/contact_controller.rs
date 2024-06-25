use actix_web::{get, post , web::{self, Data, Json}, HttpRequest, HttpResponse};
use diesel::update;
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


#[post("/update/{id}")]
pub async fn update_contact(request: Json<NewContact>, id: web::Path<(i32,)> , pool: Data<Pool>) -> HttpResponse {

    println!("UPDATE contact !!");
    println!("{:?}",request);

    println!("ID TO UPDATE {:?}",id);

    
    let result: Result<Contact, String> =  contact_service::update_contact(request.0 , id.into_inner().0 , &pool);
    match result {
        Ok(contact) => HttpResponse::Ok().json(contact),
        Err(message) => HttpResponse::BadRequest().body(message)
    }
}

#[post("/delete/{id}")]
pub async fn delete_contact(id: web::Path<(i32,)> , pool: Data<Pool>) -> HttpResponse {

    println!("ID TO DELETE {:?}",id);
    let result :Result<usize, String> = contact_service::delete_contact(id.into_inner().0 , &pool);
    match result {
        Ok(contact) => HttpResponse::Ok().json(contact),
        Err(message) => HttpResponse::BadRequest().body(message)
    }
}


#[get("/hello")]
pub async fn hello(request: HttpRequest) -> HttpResponse {
    println!("Hello Server!!");
    HttpResponse::Ok().json("Hello ! I', here !")
}