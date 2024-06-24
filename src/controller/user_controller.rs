use actix_web::{HttpResponse,HttpRequest,Responder,get, post, web::{Data, Json}};
use actix_web_httpauth::extractors::basic::BasicAuth;
use argonautica::{Hasher, Verifier};
use dotenvy::dotenv;
use jwt::SignWithKey;
use sha2::Sha256;
use hmac::{Hmac, Mac};
use crate::config::{api_end_points::TokenClaims, db::Pool};
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
    let password: Option<&str> = credentials.password();

    if username.is_empty(){
        HttpResponse::Unauthorized().json("Must provide username!!")
    }else{

        match password {
            None => {
                HttpResponse::Unauthorized().json("Must provide a password")
            },
            Some(pass) => {
    

                //Todo : get user and password from DataBase
                if username.eq("pepe") && pass.eq("1234") {

                    dotenv().ok();

                    //if user and password match we will send the token
                    let hash_secret = std::env::var("HASH_SECRET").expect("HASH_SECRET must be set!!!!!!!!!!!!!!!!!!!!");
                    let mut verifier = Verifier::default();

                    
                    let is_valid = verifier
                        .with_hash(password.unwrap())
                        .with_password(pass)
                        .with_secret_key(&hash_secret)
                        .verify()
                        .unwrap();

                    if is_valid {
                        let claims = TokenClaims { id: 1 }; //ID de usuario
                        let token_str = claims.sign_with_key(&jwt_secret).unwrap();
                        HttpResponse::Ok().json(token_str)
                    } else {
                        HttpResponse::Unauthorized().json("Incorrect username or password")
                    }


                    //HttpResponse::Ok().json("OK - You entered")

                }else{
                    //HttpResponse::Ok().json("OK")
                    HttpResponse::Unauthorized().json("User and password is invalid")
                }
            }
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

