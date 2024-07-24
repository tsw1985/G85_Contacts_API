#ifndef SHOW_ALL_CONTACT_WINDOW_H
#define SHOW_ALL_CONTACT_WINDOW_H

#include <gtk/gtk.h>
#include "../header/http_api.h"
#include <jansson.h>

typedef struct contact_row
{
    int mode ;
    char name[50];
    char first_name[100];
    char phone[9];
} contact_row;

enum
{
  COL_ID = 0,
  COL_NAME ,
  COL_FIRST_NAME,
  COL_PHONE,
  NUM_COLS
} ;

void open_show_all_contacts_window(GtkWidget *widget, gpointer data );
static GtkWidget *create_view_and_model (void);
void remove_contact();
void update_contact_call_api();
void update_contact();


#endif /* SHOW_ALL_CONTACT_WINDOW_H */