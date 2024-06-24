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
async fn basic_auth(pool: Data<Pool>, credentials: BasicAuth) -> impl Responder {

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
    

                //let user_on_db : Result<User,String> = User::get_user_by_username_and_password(username, &mut state.get().unwrap());
                let user_on_db : Result<User,String> = user_service::get_user_by_username_and_password(username, &pool);
                match user_on_db {
                    Ok(user_on_db) => {

                            println!("User from DB");
                            println!("{:?}",user_on_db);
                        
                            if user_on_db.username.eq(username) {

                                dotenv().ok();

                                //if user and password match we will send the token
                                let hash_secret = std::env::var("HASH_SECRET").expect("HASH_SECRET must be set!!!!!!!!!!!!!!!!!!!!");
                                let mut verifier = Verifier::default();

                                
                                let is_valid = verifier
                                    .with_hash(user_on_db.password)
                                    .with_password(pass)
                                    .with_secret_key(&hash_secret)
                                    .verify()
                                    .unwrap();

                                if is_valid {
                                    let claims = TokenClaims { id: user_on_db.id }; //ID de usuario
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

