#include "../header/http_api.h"
#include <stdlib.h>
#include <string.h>

size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *context) {
    size_t bytec = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)context;
    mem->memory = realloc(mem->memory, mem->size + bytec + 1);
    if(mem->memory == NULL) {
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
    memcpy(&(mem->memory[mem->size]), ptr, bytec);
    mem->size += bytec;
    mem->memory[mem->size] = 0;
    return nmemb;
}

//POST : /auth
char *post_do_login(char *user_name , char *password)
{

    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;
    chunk.memory[chunk.size] = 0;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8081/auth");

        char *val1 = curl_easy_escape(curl, "ted", 0);
        char *val2 = curl_easy_escape(curl, "1234", 0);

        char *fields = malloc(40 + strlen(val1) + 40 + strlen(val2) + 40);
        sprintf(fields, "username=%s&password=%s", val1, val2);
        printf("FIELDS TO SEND %s\n",fields);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields);
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
        
        curl_easy_setopt(curl, CURLOPT_USERNAME, user_name); // "user"
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password); //password

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // The output from the example URL is easier to read as plain text.
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: text/plain");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        // Make the example URL work even if your CA bundle is missing.
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        } else {
            //printf("HTTP GET RESPONSE : %s\n",chunk.memory);
        }

        // Remember to call the appropriate "free" functions.
        free(fields);
        curl_free(val1);
        curl_free(val2);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        printf("END CALL API AUTH!!!!!!!!!\n");
    }

    return chunk.memory;
}


// Inicializar la estructura de respuesta
void init_response_string(struct response_string *s) {
    s->len = 0;
    s->ptr = malloc(s->len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

// Callback para escribir la respuesta en la estructura
size_t write_response_callback(void *ptr, size_t size, size_t nmemb, struct response_string *_response_string) {
    size_t new_len = _response_string->len + size * nmemb;
    _response_string->ptr = realloc(_response_string->ptr, new_len + 1);
    if (_response_string->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(_response_string->ptr + _response_string->len, ptr, size * nmemb);
    _response_string->ptr[new_len] = '\0';
    _response_string->len = new_len;

    return size * nmemb;
}


void remove_quotes(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if (*src != '"') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}


// POST - ADD CONTACT
int api_call_add_contact(char *name , char *first_name , char *phone){

    printf("Go to save a contact !!!!!!!!! *******");
    printf("BEARER STATIC %s\n",current_bearer);

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if (curl) {
        
        /* First set the URL that is about to receive our POST. This URL can
           just as well be an https:// URL if that is what should receive the
           data. */
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8081/contact/add");

        /* Now specify the POST data */
        const char *json_data = malloc(256);
        sprintf(json_data,"{\"name\":\"%s\",\"first_name\":\"%s\", \"phone\":\"%s\"}",name,first_name,phone);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

        /* Create a list of headers */
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        /* Add the Authorization header with the Bearer token */
        char bearer_header[1024];
        sprintf(bearer_header, "Authorization: Bearer %s", current_bearer);
        remove_quotes(bearer_header);

        headers = curl_slist_append(headers, bearer_header);

        /* Set the headers */
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        /* Enable verbose output for debugging */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* Set up the response string */
        struct response_string response;
        init_response_string(&response);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        /* Perform the request, res gets the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else
            printf("RESPONSE: %s\n", response.ptr);

        /* always cleanup */
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers); // free the header list
        free(response.ptr); // free the response string
    }

    curl_global_cleanup();
    return 0;
}

//UPDATE CONTACT
int api_call_update_contact(int id_contact,char *name , char *first_name , char *phone){

    printf("Go to save a contact !!!!!!!!! *******");
    printf("BEARER STATIC %s\n",current_bearer);

    CURL *curl;
    CURLcode res;

    /* In windows, this inits the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if (curl) {
        

        char *update_url = malloc(50);
        sprintf(update_url,"http://localhost:8081/contact/update/%d",id_contact); 
        curl_easy_setopt(curl, CURLOPT_URL, update_url);

        /* Now specify the POST data */
        const char *json_data = malloc(256);
        sprintf(json_data,"{\"name\":\"%s\",\"first_name\":\"%s\", \"phone\":\"%s\"}",name,first_name,phone);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

        /* Create a list of headers */
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        /* Add the Authorization header with the Bearer token */
        char bearer_header[1024];
        sprintf(bearer_header, "Authorization: Bearer %s", current_bearer);
        remove_quotes(bearer_header);

        headers = curl_slist_append(headers, bearer_header);

        /* Set the headers */
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        /* Enable verbose output for debugging */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* Set up the response string */
        struct response_string response;
        init_response_string(&response);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        /* Perform the request, res gets the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else
            printf("RESPONSE: %s\n", response.ptr);

        /* always cleanup */
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers); // free the header list
        free(response.ptr); // free the response string
    }

    curl_global_cleanup();
    return 0;
}


//POST - REMOVE CONTACT
int api_call_remove_contact(int id_contact){

   printf("Go to REMOVE a contact !!!!!!!!! *******");
    printf("BEARER STATIC %s\n",current_bearer);

    CURL *curl;
    CURLcode res;

    /* In windows, this inits the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if (curl) {
        /* First set the URL that is about to receive our POST. This URL can
           just as well be an https:// URL if that is what should receive the
           data. */

        char *remove_url = malloc(50);
        sprintf(remove_url,"http://localhost:8081/contact/delete/%d",id_contact); 
        curl_easy_setopt(curl, CURLOPT_URL, remove_url);

        /* Now specify the POST data */
        const char *json_data = malloc(256);
        sprintf(json_data,"{}");

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

        /* Create a list of headers */
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        /* Add the Authorization header with the Bearer token */
        char bearer_header[1024];
        sprintf(bearer_header, "Authorization: Bearer %s", current_bearer);
        remove_quotes(bearer_header);

        headers = curl_slist_append(headers, bearer_header);

        /* Set the headers */
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        /* Enable verbose output for debugging */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* Set up the response string */
        struct response_string response;
        init_response_string(&response);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        /* Perform the request, res gets the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else
            printf("RESPONSE: %s\n", response.ptr);

        /* always cleanup */
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers); // free the header list
        free(response.ptr); // free the response string
        free(remove_url);
    }

    curl_global_cleanup();
    return 0;
}


//POST LIST CONTACTS
struct response_string api_call_list_contacts(){

    printf("Go to save a contact !!!!!!!!! *******");
    printf("BEARER STATIC %s\n",current_bearer);

    CURL *curl;
    CURLcode res;

    /* Set up the response string */
    struct response_string response;

    /* In windows, this inits the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if (curl) {
        /* First set the URL that is about to receive our POST. This URL can
           just as well be an https:// URL if that is what should receive the
           data. */
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8081/contact/list");

        /* Now specify the POST data */
        const char *json_data = malloc(256);
        //sprintf(json_data,"{\"name\":\"%s\",\"first_name\":\"%s\", \"phone\":\"%s\"}",name,first_name,phone);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

        /* Create a list of headers */
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        /* Add the Authorization header with the Bearer token */
        char bearer_header[1024];
        sprintf(bearer_header, "Authorization: Bearer %s", current_bearer);
        remove_quotes(bearer_header);

        headers = curl_slist_append(headers, bearer_header);

        /* Set the headers */
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        /* Enable verbose output for debugging */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* Set up the response string */
        //struct response_string response;
        init_response_string(&response);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        /* Perform the request, res gets the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }else{
            printf("RESPONSE ALL CONTACTS: \n");
            //printf("%s\n",response.ptr);
            printf("JSON LENGTH : %ld\n",response.len);
        }

            
        /* always cleanup */
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers); // free the header list
    }

    curl_global_cleanup();

    return response;
}


void hello_api()
{
    printf("Hello APIIIII !!!!!!!!!");
}