#include "../header/show_all_contacts_window.h"
#include "../header/add_contact_window.h"
#include "../header/update_contact_window.h"


GtkWidget *show_all_add_contact_window;
GtkWidget *show_all_contact_window_grid;
GtkWidget *table_view;
GtkWidget *show_all_info_label;
GtkWidget *show_all_remove_entry_button;
GtkWidget *show_all_update_button;
GtkTreeModel *model;
int MODE_ADD_UPDATE = 0;


void create_show_all_contacts_window()
{
    show_all_add_contact_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(show_all_add_contact_window), "Show All contacts");
    gtk_window_set_default_size(GTK_WINDOW(show_all_add_contact_window), 500, 200);
    gtk_container_set_border_width(GTK_CONTAINER(show_all_add_contact_window), 10);
}

void create_show_all_contacts_layout()
{

    //2 create layout
    show_all_contact_window_grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(show_all_contact_window_grid), TRUE);
    gtk_container_add(GTK_CONTAINER(show_all_add_contact_window), show_all_contact_window_grid);

    //3 create widgets
    show_all_info_label = gtk_label_new("Select a row to remove");
    show_all_remove_entry_button = gtk_button_new_with_label("Remove");
    g_signal_connect (show_all_remove_entry_button, "clicked", G_CALLBACK (remove_contact), NULL);


    //update contact button
    show_all_update_button = gtk_button_new_with_label("Update");
    g_signal_connect (show_all_update_button, "clicked", G_CALLBACK (update_contact), NULL);


    //4 insert items on layout
    gtk_grid_attach(GTK_GRID(show_all_contact_window_grid), show_all_info_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(show_all_contact_window_grid), show_all_remove_entry_button, 1, 0, 1, 1);
    
    

    //5 insert table
    //GtkWidget *view = create_view_and_model();    
    table_view = create_view_and_model();    
    gtk_grid_attach(GTK_GRID(show_all_contact_window_grid), table_view, 0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(show_all_contact_window_grid), show_all_update_button, 0, 2, 2, 1);
    
}

//Go to remove contact
void remove_contact(){
    printf("REMOVE CONTACT !!\n");
    GtkTreeIter iter;
    GtkTreeSelection *selection;
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(table_view));
    if(gtk_tree_selection_get_selected(selection,&model, &iter)){

        char *name;
        int *id;

        gtk_tree_model_get(model,&iter,COL_ID,&id, COL_NAME,&name, -1);
        api_call_remove_contact((int)id);
        
        //remove selected row on table
        gtk_list_store_remove(GTK_LIST_STORE(model), &iter);

        //g_print("SELECTED ROW ID %d IS %s\n",id , name);
        free(name);

    }
}


//Update contact open add contact window
void update_contact(){
    printf("UPDATE CONTACT OPEN NEW CONTACT WINDOW!!\n");

    GtkTreeIter iter;
    GtkTreeSelection *selection;
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(table_view));
    if(gtk_tree_selection_get_selected(selection,&model, &iter)){

        char *name;
        char *first_name;
        char *phone;
        int *id;

        gtk_tree_model_get(model,&iter,COL_ID         ,&id, 
                                       COL_NAME       ,&name,
                                       COL_FIRST_NAME ,&first_name,
                                       COL_PHONE      ,&phone,
                                       -1);


        //api_call_update_contact(&id);
        printf("Preview - ID --> %d\n",id);
        printf("Preview - NAME --> %s\n",name);
        printf("Preview - FIRST_NAME --> %s\n",first_name);
        printf("Preview - PHONE --> %s\n",phone);


        
        //send object to update . Test code.
        update_contact_struct *_contact = malloc(sizeof(update_contact_struct));
        _contact->mode = 1; //set UPDATE MODE
        // id ID to char
        _contact->id = (int)id;
        //printf("PUTO ID %d\n",(int)_contact->id);

        
        strcpy(_contact->name,name);
        strcpy(_contact->first_name,first_name);
        strcpy(_contact->phone,phone);

        printf("------------------------------------------\n");
        printf("ID contact --> %d\n", (int)_contact->id);
        printf("NAME --> %s\n",       _contact->name);
        printf("FIRST_NAME --> %s\n", _contact->first_name);
        printf("PHONE --> %s\n",      _contact->phone);
        
        open_update_contact_window(NULL,_contact);
        //g_print("SELECTED ROW ID %d IS %s\n",id , name);
        //free(name);
        free(_contact);
        
        

    }
}


//Update contact go to API
void update_contact_call_api(){
 
  
}


void open_show_all_contacts_window(GtkWidget *widget, gpointer data)
{
    contact_row *_contact_row = (contact_row*)data;
    create_show_all_contacts_window();
    create_show_all_contacts_layout();
    gtk_widget_show_all(show_all_add_contact_window);
}


// TABLES
static GtkTreeModel *create_and_fill_model (void)
{
    //------------- FILL TABLE  -----------------------------------------------------
    //Add num of cols to table and type ( String, String, String )
    GtkListStore *store = gtk_list_store_new (NUM_COLS,G_TYPE_INT, G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);

    /* Append a row and fill in some data */
    GtkTreeIter iter;

    struct response_string json_contact_list;
    json_contact_list = api_call_list_contacts();
    if(json_contact_list.ptr != NULL && json_contact_list.len > 0){

        json_t *root;
        json_error_t error;

        root = json_loads(json_contact_list.ptr, 0, &error);
        if (!root) {
            fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
            return NULL;
        }else if (!json_is_array(root)) {
            fprintf(stderr, "error: root is not an array\n");
            json_decref(root);
            return NULL;
        }else{

            size_t index;
            json_t *value;
            json_array_foreach(root, index, value) {
                json_t *id = json_object_get(value, "id");
                json_t *name = json_object_get(value, "name");
                json_t *first_name = json_object_get(value, "first_name");
                json_t *phone = json_object_get(value, "phone");

                if (json_is_integer(id) && 
                    json_is_string(name) && 
                    json_is_string(first_name) && 
                    json_is_string(phone)) {

                        gtk_list_store_append (store, &iter);
                        gtk_list_store_set (store, &iter,
                                                     COL_ID,         json_integer_value(id),
                                                     COL_NAME,       json_string_value(name),
                                                     COL_FIRST_NAME, json_string_value(first_name), 
                                                     COL_PHONE ,     json_string_value(phone),
                                                      -1);
                }
            }

            json_decref(root);

        }
    }

    free(json_contact_list.ptr);

  return GTK_TREE_MODEL (store);

}

static GtkWidget *create_view_and_model (void)
{
    GtkWidget *view = gtk_tree_view_new ();

    GtkCellRenderer *renderer;

    // col ID
    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),-1,"ID",renderer,"text", COL_ID,NULL);


    /* --- Column #1 --- */
    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),-1,"Name",renderer,"text", COL_NAME,NULL);

    /* --- Column #2 --- */
    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),-1,"First Name",renderer,"text", COL_FIRST_NAME,NULL);

    /* --- Column #3 --- */
    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),-1,"Phone",renderer,"text", COL_PHONE,NULL);

    //GtkTreeModel *model = create_and_fill_model ();
    model = create_and_fill_model ();

    gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

    /* The tree view has acquired its own reference to the
    *  model, so we can drop ours. That way the model will
    *  be freed automatically when the tree view is destroyed
    */
    g_object_unref (model);

    return view;
}






