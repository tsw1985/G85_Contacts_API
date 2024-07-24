#ifndef HTTP_API_H
#define HTTP_API_H

#include <curl/curl.h>
#include <jansson.h>


struct MemoryStruct {
  char *memory;
  size_t size;
};

typedef struct response_string {
    char *ptr;
    size_t len;
} response_string;



extern char *current_bearer;
extern int MODE_INSERT_UPDATE;


size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *context);
char* post_do_login(char *user_name , char *password);
int api_call_add_contact(char *name , char *first_name , char *phone);
int api_call_update_contact(int id_contact,char *name , char *first_name , char *phone);
response_string api_call_list_contacts();
int api_call_remove_contact(int id_contact);
void hello_api();
json_t *parse_json_array(const char *json_str);

#endif /* HTTP_API_H */
