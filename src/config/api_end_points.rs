use actix_web::web::{self,scope};
use crate::controller::contact_controller::*;
use crate::controller::user_controller::*;

pub fn config_services(cfg: &mut web::ServiceConfig) {

    cfg.service(
        scope("/api")
               .service(hello)
    ).

    service(
       scope("/contact")
               .service(create_contact)
    ).
    
    service(scope("/user")
            .service(create_user)
    ).




    service(hello);
}