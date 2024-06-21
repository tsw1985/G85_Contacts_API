use actix_web::web::{self,scope};
use crate::controller::contact_controller::*;

pub fn config_services(cfg: &mut web::ServiceConfig) {

    cfg.service(scope("/contact")
        .service(create_contact)
    
    );
        //.service(create_posts)
        //.service(publish_post)
        //.service(delete_post)
    
}