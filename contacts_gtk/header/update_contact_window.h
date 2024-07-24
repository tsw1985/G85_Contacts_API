#ifndef UPDATE_CONTACT_WINDOW_H
#define UPDATE_CONTACT_WINDOW_H

#include <gtk/gtk.h>

typedef struct update_contact_struct
{
    int mode ;
    char name[50];
    char first_name[100];
    char phone[100];
    int id;
} update_contact_struct;

void open_update_contact_window(GtkWidget *widget, gpointer data );
void update_contact_go_api();
#endif /* UPDATE_CONTACT_WINDOW */