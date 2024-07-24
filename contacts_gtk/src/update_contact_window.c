#include "../header/update_contact_window.h"
#include "../header/http_api.h"

GtkWidget *update_contact_window;
GtkWidget *update_contact_window_grid;

//Name
GtkWidget *update_contact_name_label;
GtkWidget *update_contact_name_entry;

//First name
GtkWidget *update_contact_first_name_label;
GtkWidget *update_contact_first_name_entry;

//Phone
GtkWidget *update_contact_phone_label;
GtkWidget *update_contact_phone_entry;

GtkWidget *update_contact_id_label;
GtkWidget *update_contact_id_entry;

//Save button
GtkWidget *update_contact_save_button;

//test code

void update_contact_create_window()
{
    update_contact_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(update_contact_window), "UPDATE Existing Contact");
    gtk_window_set_default_size(GTK_WINDOW(update_contact_window), 300, 200);
    gtk_container_set_border_width(GTK_CONTAINER(update_contact_window), 10);
}

void update_contact_create_layout()
{
    update_contact_window_grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(update_contact_window_grid), TRUE);
    gtk_container_add(GTK_CONTAINER(update_contact_window), update_contact_window_grid);
}

void update_contact_create_gtk_widgets()
{
    update_contact_name_label = gtk_label_new("Name:");
    update_contact_name_entry = gtk_entry_new();

    update_contact_first_name_label = gtk_label_new("First Name:");
    update_contact_first_name_entry = gtk_entry_new();

    update_contact_phone_label = gtk_label_new("Phone:");
    update_contact_phone_entry = gtk_entry_new();

    update_contact_id_label = gtk_label_new("ID");
    update_contact_id_entry = gtk_entry_new();

    update_contact_save_button = gtk_button_new_with_label("Save");
}

void update_contact_init_gtk_signals(){
    printf("INIT EVENT CLICK UPDATE CONTACT!\n");
    g_signal_connect (update_contact_save_button, "clicked", G_CALLBACK (update_contact_go_api), NULL);
}

void update_contact_go_api(){

    printf("CALLING UPDATE A CONTACT BEFORE API !\n");

    //get values from fields
    char *name = gtk_entry_get_text(GTK_ENTRY(update_contact_name_entry));
    char *first_name = gtk_entry_get_text(GTK_ENTRY(update_contact_first_name_entry));
    char *phone = gtk_entry_get_text(GTK_ENTRY(update_contact_phone_entry));
    char *id_contact = gtk_entry_get_text(GTK_ENTRY(update_contact_id_entry));

    int id_contact_int = atoi(id_contact);

    api_call_update_contact(id_contact_int, name , first_name , phone);
}



void update_contact_insert_widgets_on_layout()
{
    gtk_grid_attach(GTK_GRID(update_contact_window_grid), update_contact_name_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(update_contact_window_grid), update_contact_name_entry, 1, 0, 1, 1);


    gtk_grid_attach(GTK_GRID(update_contact_window_grid), update_contact_first_name_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(update_contact_window_grid), update_contact_first_name_entry, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(update_contact_window_grid), update_contact_phone_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(update_contact_window_grid), update_contact_phone_entry, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(update_contact_window_grid), update_contact_id_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(update_contact_window_grid), update_contact_id_entry, 1, 3, 1, 1);


    gtk_grid_attach(GTK_GRID(update_contact_window_grid), update_contact_save_button, 0, 4, 2, 1);
}

void open_update_contact_window(GtkWidget *widget, gpointer data)
{

    update_contact_struct *_contact = (update_contact_struct*)data;
    update_contact_create_window();
    update_contact_create_layout();
    update_contact_create_gtk_widgets();
    update_contact_insert_widgets_on_layout();

    //set text to text box
    gtk_entry_set_text(GTK_ENTRY(update_contact_name_entry),_contact->name);   
    gtk_entry_set_text(GTK_ENTRY(update_contact_first_name_entry) ,_contact->first_name);
    gtk_entry_set_text(GTK_ENTRY(update_contact_phone_entry) ,_contact->phone);

    //set ID to string
    char id_char[5];
    snprintf(id_char,sizeof(id_char),"%d",_contact->id );
    gtk_entry_set_text(GTK_ENTRY(update_contact_id_entry) , id_char);
    //end set ID to string
    

    gtk_widget_show_all(update_contact_window);
    update_contact_init_gtk_signals();
}