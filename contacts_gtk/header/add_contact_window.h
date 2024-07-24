#ifndef ADD_CONTACT_WINDOW_H
#define ADD_CONTACT_WINDOW_H

#include <gtk/gtk.h>

typedef struct contact
{
    int mode ;
    char name[50];
    char first_name[100];
    char phone[100];
    int id;
} contact;

void open_add_contact_window(GtkWidget *widget, gpointer data );
void save_contact();


#endif /* ADD_CONTACT_WINDOW_H */