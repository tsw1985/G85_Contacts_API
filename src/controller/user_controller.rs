use actix_web::{HttpResponse,HttpRequest,Responder,get, post, web::{Data, Json}};
use actix_web_httpauth::extractors::basic::BasicAuth;
use sha2::Sha256;
use hmac::{Hmac, Mac};
use crate::config::db::Pool;
use crate::models::user::{NewUser,User};
use crate::service::user_service;



#[get("/auth")]
async fn basic_auth(state: Data<Pool>, credentials: BasicAuth) -> impl Responder {

    println!("CREDENCIALES BASIC AUTH!!");
    println!("{:?}",credentials);
    println!("FIN CREDENCIALES BASIC AUTH!!");

    let jwt_secret: Hmac<Sha256> = Hmac::new_from_slice(
        std::env::var("JWT_SECRET")
            .expect("JWT_SECRET must be set!")
            .as_bytes(),
    )
    .unwrap();


    let username = credentials.user_id();
    let password = credentials.password();
    

    match password {
        None => HttpResponse::Unauthorized().json("Must provide username and password"),
        Some(pass) => {



            HttpResponse::Ok().json("OK")
         
                
            
            
        }
    }
}












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

