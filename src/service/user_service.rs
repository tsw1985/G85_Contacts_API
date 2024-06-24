use actix_web::web;
use crate::config::db::Pool;
use crate::models::user::{NewUser , User};
use argonautica::{Hasher, Verifier};

pub fn create_user(mut new_user: NewUser, pool: &web::Data<Pool>) -> Result<User, String> {

    let hash_secret = std::env::var("HASH_SECRET").expect("HASH_SECRET must be set!");
    let mut hasher = Hasher::default();
    let hash = hasher
        .with_password(new_user.password)
        .with_secret_key(hash_secret)
        .hash()
        .unwrap();

    new_user.password = hash;

    return User::create_user(new_user, &mut pool.get().unwrap())
}