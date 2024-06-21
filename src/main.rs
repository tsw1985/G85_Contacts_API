mod utils;
mod controller;
mod models;
mod service;
mod schema;
mod config;

use std::env;
use actix_web::{get, post, web, App, HttpResponse, HttpServer, Responder};
use diesel::{sql_types::Integer, Connection, PgConnection};
use dotenvy::dotenv;
use utils::functions::*;


#[get("/")]
async fn hello() -> impl Responder {
    HttpResponse::Ok().body("Hello world!")
}

#[post("/echo")]
async fn echo(req_body: String) -> impl Responder {
    HttpResponse::Ok().body(req_body)
}

async fn manual_hello() -> impl Responder {
    HttpResponse::Ok().body("Hey there!")
}


#[actix_web::main]
async fn main() -> std::io::Result<()> {
    
    //Load enviroment variables
    dotenv().ok();
    let db_url = env::var("DATABASE_URL").expect("DATABASE_URL must be set");
    let app_host = env::var("APP_HOST").expect("APP_HOST not found.");
    let app_port  = env::var("APP_PORT").expect("APP_PORT not found.");
    let app_url = format!("{}:{}", &app_host, &app_port);

    //Create pool conection
    let pool = config::db::init_db_pool(&db_url);


    //Start the Web server
    HttpServer::new(move || {
        App::new()
            //add to each controller method a db pool conection object
            .app_data(web::Data::new(pool.clone()))
            .configure(config::api_end_points::config_services)
    }).bind(&app_url)?
      .run()
      .await
    
}