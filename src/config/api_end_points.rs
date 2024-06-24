use actix_web::web::{self,scope};
use crate::controller::contact_controller::*;
use crate::controller::user_controller::*;
use serde::{Deserialize, Serialize};




#[derive(Serialize, Deserialize, Clone)]
pub struct TokenClaims {
    pub id: i32,
}

pub fn config_services(cfg: &mut web::ServiceConfig) {

    cfg.service(
        scope("/api")
               .service(hello)
    ).

    // /auth 
    service(basic_auth).

    service(
       scope("/contact")
               .service(create_contact)
    ).
    
    service(scope("/user")
            .service(create_user)
    ).




    service(hello);
}