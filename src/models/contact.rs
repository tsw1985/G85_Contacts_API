use diesel::prelude::*;

#[derive(Queryable, Selectable)]
#[diesel(table_name = crate::schema::contacts)]
#[diesel(check_for_backend(diesel::pg::Pg))]
pub struct Contact {
    pub id: i32,
    pub name: String,
    pub first_name: String,
    pub phone: String,
}