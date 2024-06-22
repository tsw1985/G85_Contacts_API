use actix_web::web;
use crate::config::db::Pool;
use crate::models::user::{NewUser , User};

pub fn create_user(new_user: NewUser, pool: &web::Data<Pool>) -> Result<User, String> {
    return User::create_user(new_user, &mut pool.get().unwrap())
}