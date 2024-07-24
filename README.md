# G85_contacts_web_api

## Getting started

- You need install this libs : curl , pthread , jansson 


## 1 . Download Postgree docker container

1 . Download the image of postgres : "docker pull postgres:14-alpine"

2 . Create a folder called "postres" , put there the file who is on the folder "docker" called "docker-compose.yml" into your folder just created and execute "docker compose up -d" . This will run your database.

3 . Execute "diesel migration run" once you database is connected to create the dabase. Maybe you need 
    install "diesel cli"

4 . Execute "cargo run" . If you have some problems on your compilation with Linux, you will need
    install  the DEV-LIBS for "Postgres" , "SqlLite" and "Mysql" , search the correct package for your distribution.

5 . When all is ok with "cargo run" , later you will have your server working on localhost:8081

6 . Go into the folder "contacts_gtk" and run : "compile.sh"

7 . Before call the client "contacts_gtk" you must create a user to login . You need do a /POST call to
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


## CLIENT ( contacts_gtk )

1 . Run the script ./compile.sh . You must have installed GTK3 library on your system. On the file
    "http_api.c" are all the POST methods with hardcoded strings. All this was for playing.