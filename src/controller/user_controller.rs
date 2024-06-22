use actix_web::{HttpResponse,HttpRequest,get, post, web::{Data, Json}};
use crate::config::db::Pool;
use crate::models::user::{NewUser,User};
use crate::service::user_service;


#[post("/add")]
pub async fn create_user(request: Json<NewUser>, pool: Data<Pool>) -> HttpResponse {

    println!("Creando usuario nuevo !!");
    println!("{:?}",request);
    
    let result: Result<User, String> =  user_service::create_user(request.0, &pool);
    match result {
        Ok(user) => HttpResponse::Ok().json(user),
        Err(message) => HttpResponse::BadRequest().body(message)
    }
}