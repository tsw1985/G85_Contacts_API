# G85_contacts_web_api

This is a small project to work with the JWT (Json Web Token) authentification. It is running over a
web server made it with Rust and for client side we have a small window application made it with
C , GTK 3 , Curl_lib ( API CALLS ) , and libjansson-dev libreries.

## Getting started

- You need install this libs : curl , pthread , jansson 


## How run it ? 

1 . Install Docker and download the image of postgres : "docker pull postgres:14-alpine"

2 . Create a folder called "postres" , put there the file who is on the folder "docker" called "docker-compose.yml" and run "docker compose up -d" . This will run your database container.

3 . Execute "diesel migration run" once you database is connected to create the database. Maybe you need 
    install "diesel cli"

4 . Execute "cargo run" . If you have some problems on your compilation with Linux, you will need
    install  the DEV-LIBS for "Postgres" , "SqlLite" and "Mysql" , search the correct package for your distribution.

5 . When all is ok with "cargo run" , later you will have your server working on localhost:8081

6 . Go into the folder "contacts_gtk" and run : "/compile.sh"

7 . Before to run the client "contacts_gtk" you must create a user to login . You need do a /POST call to
    localhost:8081/user/add and put this body :

    { 
       "username" : "ted" ,
       "password" : 1234"
    }

8 . Execute the client gtk_contacts with "compile.sh" and add your data.   

## API POST

- /auth : get JWT Token
- /user/add : create a user on the system
- /contact/add : add a contact
- /contact/update/{id} : update a contact
- /contact/delete/{id} : delete contact
- /contact/list : list all contacts

The body to create/update a contact is :

    {
        "name" : "Jackie" ,
        "first_name" , "Chan" ,
        "phone" : "777888777"
    }


## CLIENT ( contacts_gtk )

1 . Run the script ./compile.sh . You must have installed GTK3 library on your system. On the file
    "http_api.c" are all the POST methods with hardcoded strings. All this was for playing.



    ![fibob](https://github.com/tsw1985/G85_Contacts_API/blob/main/contacts_gtk/main_screen.png)