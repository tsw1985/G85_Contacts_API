use actix_web::dev::ServiceRequest;
use actix_web::web::{self,scope};
use actix_web::{Error, HttpMessage};
use actix_web_httpauth::extractors::bearer::{self, BearerAuth};
use actix_web_httpauth::extractors::AuthenticationError;
use actix_web_httpauth::middleware::HttpAuthentication;
use chrono::Utc;
use hmac::{Hmac, Mac};
use jwt::VerifyWithKey;
use sha2::Sha256;
use crate::controller::contact_controller::*;
use crate::controller::user_controller::*;
use serde::{Deserialize, Serialize};



//When the login is done , we send the "Claims" . This is filled on
// "user_controller" basic_auth method.
/*
    The claims are formed by HEADER + PAYLOAD + SIGNATURE . So we sent
    the beared string composed by this 3 elements
*/
#[derive(Serialize, Deserialize, Clone)]
pub struct TokenClaims {
    pub id: i32,
    pub username : String,
    pub exp : i64
}

pub fn config_services(cfg: &mut web::ServiceConfig) {

    let bearer_middleware = HttpAuthentication::bearer(validator);
    
    cfg.service(
        scope("/api")
               .service(hello)
    ).

    // UNPROTECTED : LOGIN AUTHENTIFICATION /auth 
    service(basic_auth).
    // UNPROTECTED : Create user
    service(scope("/user")
            .service(create_user)
    ).

    // *** PROTECTED **** CONTACTS END POINTS
    service(
       scope("/contact")
               .wrap(bearer_middleware)
               .service(create_contact)
               .service(update_contact)
               .service(delete_contact)
               .service(list_all_contacts)
    ).

    service(hello);
}

async fn validator(
    req: ServiceRequest,
    credentials: BearerAuth,
) -> Result<ServiceRequest, (Error, ServiceRequest)> {

    let jwt_secret: String = std::env::var("JWT_SECRET").expect("JWT_SECRET must be set!");
    let key: Hmac<Sha256> = Hmac::new_from_slice(jwt_secret.as_bytes()).unwrap();
    let token_string = credentials.token();

    let claims: Result<TokenClaims, &str> = token_string
        .verify_with_key(&key)
        .map_err(|_| "Invalid token");

    match claims {
        Ok(value) => {
            // Verificar la expiración del token
            let current_timestamp :i64 = Utc::now().timestamp();

            //if exp field on claim is less than NOW , not allow continue

            if value.exp < current_timestamp {
                let config = req
                    .app_data::<bearer::Config>()
                    .cloned()
                    .unwrap_or_default()
                    .scope("");
                
                let err = AuthenticationError::from(config).into();
                return Err((err, req));
            }
            
            req.extensions_mut().insert(value);
            Ok(req)
        }
        Err(_) => {
            let config = req
                .app_data::<bearer::Config>()
                .cloned()
                .unwrap_or_default()
                .scope("");

            let err = AuthenticationError::from(config).into();
            Err((err, req))
        }
    }
}