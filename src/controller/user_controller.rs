use actix_web::{post, web::{Data, Json},HttpResponse, Responder};
use actix_web_httpauth::extractors::{basic::BasicAuth};
use argonautica::Verifier;
use chrono::Utc;
use dotenvy::dotenv;
use jwt::SignWithKey;
use sha2::Sha256;
use hmac::{Hmac, Mac};
use crate::config::{api_end_points::TokenClaims, db::Pool};
use crate::models::user::{NewUser,User};
use crate::service::user_service;

#[post("/auth")]
async fn basic_auth(pool: Data<Pool>, credentials: BasicAuth) -> impl Responder {

    println!("CREDENCIALES BASIC AUTH!!");
    println!("{:?}",credentials);
    println!("FIN CREDENCIALES BASIC AUTH!!");

    let username = credentials.user_id();
    let password: Option<&str> = credentials.password();

    if username.is_empty(){
        HttpResponse::Unauthorized().json("Must provide username!!")
    }else{

        match password {
            None => {
                HttpResponse::Unauthorized().json("Must provide a password!!")
            },
            Some(pass) => {

                let user_on_db : Result<User,String> = user_service::get_user_by_username_and_password(username, &pool);
                match user_on_db {
                    Ok(user_on_db) => {

                            println!("User from DB");
                            println!("{:?}",user_on_db);
                        
                            if user_on_db.username.eq(username) {

                                dotenv().ok();

                                //if user and password match we will send the token
                                let hash_secret : String = std::env::var("HASH_SECRET").expect("HASH_SECRET must be set!!!!!!!!!!!!!!!!!!!!");
                                let mut verifier : Verifier = Verifier::default();

                                
                                let is_valid = verifier
                                    .with_hash(user_on_db.password)
                                    .with_password(pass)
                                    .with_secret_key(&hash_secret)
                                    .verify()
                                    .unwrap();

                                if is_valid {

                                    let jwt_secret: Hmac<Sha256> = Hmac::new_from_slice(
                                                                        std::env::var("JWT_SECRET")
                                                                        .expect("JWT_SECRET must be set!")
                                                                        .as_bytes(),
                                                                    ).unwrap();

                                    let expiration = Utc::now().checked_add_signed(chrono::Duration::minutes(60))
                                                                    .expect("valid timestamp")
                                                                    .timestamp();

                                    
                                    let claims :TokenClaims = TokenClaims { id: user_on_db.id ,
                                                                            username : user_on_db.username ,
                                                                            exp : expiration
                                                                          }; //ID de usuario

                                    let token_str :String = claims.sign_with_key(&jwt_secret).unwrap();

                                    //The token_str is formed by :
                                    // - header
                                    // - payload
                                    // - signature
                                    HttpResponse::Ok().json(token_str)
                                } else {
                                    HttpResponse::Unauthorized().json("Incorrect username or password")
                                }

                            }else{
                                HttpResponse::Unauthorized().json("User and password is invalid")
                            }
                    },
                    Err(_) => {
                        HttpResponse::Unauthorized().json("Error getting user by email")
                    }
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

