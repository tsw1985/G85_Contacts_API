#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <pthread.h>
#include "../header/main.h"
#include "../header/add_contact_window.h"
#include "../header/show_all_contacts_window.h"
#include "../header/http_api.h"



//Menu items
GtkWidget *window;
GtkWidget *grid;
GtkWidget *title_label;
GtkWidget *add_contact_button;
GtkWidget *update_contact_button;
GtkWidget *remove_contact_button;
GtkWidget *list_all_contacts_button;
GtkWidget *quit_program;

char *current_bearer;
int MODE_INSERT_UPDATE = 0;


int main (int argc, char **argv)
{
  
    GtkApplication *app;
    int status;
    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    return status;
}

static void do_login()
{
   
    //move this call to a thread

    char *user_name = "gabriel";
    char *password = "1234";
    char *login_response = post_do_login(user_name,password);

    current_bearer = malloc(1024);
    memset(current_bearer,' ',1024);
    strcpy(current_bearer,login_response);


    printf("RESPUESTA DESDE API EN THREAD!! ---> %s\n",login_response);
    printf("RESPUESTA COPIADA %s\n",current_bearer);

    free(login_response);
    //free(user_name);
    //free(password);
}

static void config_window(){

    gtk_window_set_default_size(GTK_WINDOW(window),400,100);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);

    //create layout
    grid = gtk_grid_new ();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid),TRUE);

    /* Pack the container in the window */
    gtk_container_add (GTK_CONTAINER (window), grid);

}


static void init_gtk_title(){
    gtk_window_set_title (GTK_WINDOW (window), "G85 Contacts");
}

static void init_gtk_buttons(){
    add_contact_button = gtk_button_new_with_label ("Add contact");
    //update_contact_button = gtk_button_new_with_label ("Update contact");
    remove_contact_button = gtk_button_new_with_label ("Delete contact");
    list_all_contacts_button = gtk_button_new_with_label ("List contacts");
    quit_program = gtk_button_new_with_label ("Quit");
}

static void init_gtk_signals(){
    g_signal_connect_swapped (quit_program, "clicked", G_CALLBACK (gtk_widget_destroy), window);
    g_signal_connect (add_contact_button,       "clicked", G_CALLBACK (open_add_contact_window), NULL);
    //g_signal_connect (update_contact_button,    "clicked", G_CALLBACK (print_hello), NULL);
    g_signal_connect (remove_contact_button,    "clicked", G_CALLBACK (print_hello), NULL);
    g_signal_connect (list_all_contacts_button, "clicked", G_CALLBACK (open_show_all_contacts_window), NULL);
}

static void init_layout(){

    gtk_grid_attach (GTK_GRID (grid), title_label, 0, 0, 5, 1);
    gtk_grid_attach (GTK_GRID (grid), add_contact_button, 0, 1, 5, 1);
    //gtk_grid_attach (GTK_GRID (grid), update_contact_button, 0, 2, 5, 1);
    //gtk_grid_attach (GTK_GRID (grid), remove_contact_button, 0, 2, 5, 1);
    gtk_grid_attach (GTK_GRID (grid), list_all_contacts_button, 0, 2, 5, 1);
    gtk_grid_attach (GTK_GRID (grid), quit_program, 0, 3, 5, 1);
    gtk_widget_show_all (window);

}

static void activate (GtkApplication *app, gpointer user_data)
{
    //create login thread
    pthread_t http_api_threads[1];
    pthread_create(&http_api_threads[0], NULL, (void*)do_login, NULL);
    // Esperar a que terminen los hilos
    for (int i = 0; i < 1; i++) {
        pthread_join(http_api_threads[i], NULL);
    }
    //end create login thread

    //create window ...
    window = gtk_application_window_new (app);
    config_window();
    init_gtk_title();
    init_gtk_buttons();
    init_gtk_signals();
    init_layout();
}

static void print_hello (GtkWidget *widget, gpointer data)
{
    g_print ("Hello World\n");
}