// @generated automatically by Diesel CLI.

diesel::table! {
    contacts (id) {
        id -> Int4,
        name -> Varchar,
        first_name -> Varchar,
        phone -> Text,
    }
}
