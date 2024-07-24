#include "../header/add_contact_window.h"
#include "../header/http_api.h"

GtkWidget *add_contact_window;
GtkWidget *add_contact_window_grid;

//Name
GtkWidget *name_label;
GtkWidget *name_entry;

//First name
GtkWidget *first_name_label;
GtkWidget *first_name_entry;

//Phone
GtkWidget *phone_label;
GtkWidget *phone_entry;

GtkWidget *id_label;
GtkWidget *id_entry;

//Save button
GtkWidget *save_button;

//test code

void create_window()
{
    add_contact_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(add_contact_window), "Add New Contact");
    gtk_window_set_default_size(GTK_WINDOW(add_contact_window), 300, 200);
    gtk_container_set_border_width(GTK_CONTAINER(add_contact_window), 10);
}

void create_layout()
{
    add_contact_window_grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(add_contact_window_grid), TRUE);
    gtk_container_add(GTK_CONTAINER(add_contact_window), add_contact_window_grid);
}

void create_gtk_widgets()
{
    name_label = gtk_label_new("Name:");
    name_entry = gtk_entry_new();

    first_name_label = gtk_label_new("First Name:");
    first_name_entry = gtk_entry_new();

    phone_label = gtk_label_new("Phone:");
    phone_entry = gtk_entry_new();

    id_label = gtk_label_new("ID: (not write on add mode)");
    id_entry = gtk_entry_new();

    save_button = gtk_button_new_with_label("Save");
}

void init_gtk_signals(){
    printf("INIT EVENT CLICK ADD CONTACT!\n");
    g_signal_connect (save_button, "clicked", G_CALLBACK (save_contact), NULL);
}

void save_contact(){

    printf("CALLING SAVE A CONTACT BEFORE API !\n");

    //get values from fields
    char *name = gtk_entry_get_text(GTK_ENTRY(name_entry));
    char *first_name = gtk_entry_get_text(GTK_ENTRY(first_name_entry));
    char *phone = gtk_entry_get_text(GTK_ENTRY(phone_entry));
    char *id_contact = gtk_entry_get_text(GTK_ENTRY(id_entry));

    if(MODE_INSERT_UPDATE == 0){
        api_call_add_contact(name,first_name,phone);
    }else{
        api_call_update_contact((int)id_contact, name , first_name , phone);
    }
    
}



void insert_widgets_on_layout()
{
    gtk_grid_attach(GTK_GRID(add_contact_window_grid), name_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(add_contact_window_grid), name_entry, 1, 0, 1, 1);


    gtk_grid_attach(GTK_GRID(add_contact_window_grid), first_name_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(add_contact_window_grid), first_name_entry, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(add_contact_window_grid), phone_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(add_contact_window_grid), phone_entry, 1, 2, 1, 1);

    //gtk_grid_attach(GTK_GRID(add_contact_window_grid), id_label, 0, 3, 1, 1);
    //gtk_grid_attach(GTK_GRID(add_contact_window_grid), id_entry, 1, 3, 1, 1);


    gtk_grid_attach(GTK_GRID(add_contact_window_grid), save_button, 0, 4, 2, 1);
}

void open_add_contact_window(GtkWidget *widget, gpointer data)
{

    contact *_contact = (contact*)data;
    create_window();
    create_layout();
    create_gtk_widgets();
    //init_gtk_signals();
    insert_widgets_on_layout();

    //gtk_entry_set_text(name_entry,"Gabriel");   
    if(_contact != NULL && _contact->mode && _contact->mode == 1){
        gtk_window_set_title(GTK_WINDOW(add_contact_window), "UPDATE Contact");

        
        gtk_entry_set_text(GTK_ENTRY(name_entry),_contact->name);   
        gtk_entry_set_text(GTK_ENTRY(first_name_entry) ,_contact->first_name);
        gtk_entry_set_text(GTK_ENTRY(phone_entry) ,_contact->phone);

        //set ID to string
        char id_char[5];
        snprintf(id_char,sizeof(id_char),"%d",_contact->id );
        gtk_entry_set_text(GTK_ENTRY(id_entry) , id_char);
        //end set ID to string
    }

    gtk_widget_show_all(add_contact_window);
    init_gtk_signals();
}