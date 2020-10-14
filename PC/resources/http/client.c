#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "string.h"
#include "parson.h" /* JSON manipulation. */

int main(int argc, char *argv[])
{
    char *user_input_buff = (char *)malloc(sizeof(char) * MAX_INPUT_LEN);
    char *message;
    char *response;
    int sockfd;

    char *username;
    char *password;
    char *book_id;
    char *cookie = NULL;
    char *JWT = NULL;
    
    /* To read details avout a book. */
    char title[100];
    char author[100];
    char genre[100];
    char publisher[100];
    char page_count_char[100];
    int page_count;

    /* Helpers for parceing JSON objects. */
    char *body_data;
    JSON_Value *root_value;
    JSON_Object *root_object;
    JSON_Array *fields;
    JSON_Object *field;

    /* Connect to host server. */
    while (1)
    {
        /* Get the user input. */
        fgets(user_input_buff, MAX_INPUT_LEN, stdin);
        char *user_input = strtok(user_input_buff, " ");
        user_input = strtok(user_input, "\n ");

        /* Handle REGISTER command. */
        if (strcmp(user_input, "register") == 0)
        {
            /* Connect to server. */
            sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0){fprintf(stderr, "Could not connect to host server.\n");exit(1);}

            /* Make space for input buffer. */
            username = (char *)malloc(100 * sizeof(char));
            password = (char *)malloc(100 * sizeof(char));

            fprintf(stderr, "username=");
            fgets(username, 100, stdin);
            username = strtok(username, "\n ");

            fprintf(stderr, "password=");
            fgets(password, 100, stdin);
            password = strtok(password, "\n ");

            /* Make space for JSON object. */
            root_value = json_value_init_object();
            root_object = json_value_get_object(root_value);
            body_data = NULL;

            /* Populate JSON string. */
            json_object_set_string(root_object, "username", username);
            json_object_set_string(root_object, "password", password);
            body_data = json_serialize_to_string(root_value);

            /* Make request. */
            message = compute_post_request("3.8.116.10", "/api/v1/tema/auth/register", "application/json",
                                           body_data, NULL, 0);
            send_to_server(sockfd, message);

            /* Get response from server. */
            response = receive_from_server(sockfd);

            /* Parce responce. */
            char *token = strtok(response, "\n");
            while (token != NULL && token[0] != '{')
                token = strtok(NULL, "\n");
            if (token != NULL)
            {
                token = strtok(token, "\"");
                for (int i = 0; i < 3; i++, token = strtok(NULL, "\""));
                fprintf(stderr, "%s\n", token);
            }
            else
            {
                printf("Account created succesfully.\n");
            }

            /* Erase password and username for memory for a minimal safety. */
            free(username);
            free(password);
            username = NULL;
            password = NULL;
            json_free_serialized_string(body_data);
            json_value_free(root_value);

            /* Close connection. */
            close_connection(sockfd);
        }

        /* Handle LOGIN command. */
        else if (strcmp(user_input, "login") == 0)
        {
            /* Connect to server. */
            sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0){fprintf(stderr, "Could not connect to host server.\n");exit(1);}

            /* Make space for buffer. */
            char *username = (char *)malloc(100 * sizeof(char));
            char *password = (char *)malloc(100 * sizeof(char));

            fprintf(stderr, "username=");
            fgets(username, 100, stdin);
            username = strtok(username, "\n ");

            fprintf(stderr, "password=");
            fgets(password, 100, stdin);
            password = strtok(password, "\n ");

            /* Make space for JSON object. */
            root_value = json_value_init_object();
            root_object = json_value_get_object(root_value);
            body_data = NULL;

            /* Populate JSON string. */
            json_object_set_string(root_object, "username", username);
            json_object_set_string(root_object, "password", password);
            body_data = json_serialize_to_string(root_value);

            response = NULL;
            /* Make request. */
            message = compute_post_request("3.8.116.10", "/api/v1/tema/auth/login", "application/json",
                                           body_data, NULL, 0);
            send_to_server(sockfd, message);
            message = NULL;
            body_data = NULL;

            /* Get response from server. */
            response = receive_from_server(sockfd);

            /* Get responce code and cookie(if succesfull) */
            char *token = NULL;
            if (response != NULL)
                token = strtok(response, "\n");
            while (token != NULL && token[0] != '{')
            {   
                if (strncmp(token, "Set-Cookie", 10) == 0)
                {
                    token = strtok(token, " ");
                    /* Make room for the cookie. */
                    cookie = (char *) malloc (300 * sizeof(char));
                    strcpy(cookie, strtok(NULL, " "));
                    cookie[strlen(cookie) - 1] = '\0';
                }
                token = strtok(NULL, "\n");
            }

            /*Print the error message if there is one. */
            if (token != NULL)
            {
                token = strtok(token, "\"");
                for (int i = 0; i < 3; i++, token = strtok(NULL, "\""));
                fprintf(stderr, "%s\n", token);
            }

            /* If there is no error message and the cookie has been received. */
            if (token == NULL && cookie != NULL)
            {
                /* If the user just logged in, his JWT must be NULL. This is done so a 
                 * new user does not have the JWT of the user that was previously logged in.
                 */
                if (JWT != NULL) {
                    free(JWT);
                    JWT = NULL;
                }
                printf("Login succesful.\n");
            }

            /* Erase password and username for memory for a minimal safety. */
            free(username);
            free(password);
            username = NULL;
            password = NULL;
            json_free_serialized_string(body_data);
            json_value_free(root_value);

            /* Close connection. */
            close_connection(sockfd);
        }

        /* Handle ENTER_LIBRARY command. */
        else if (strcmp(user_input, "enter_library") == 0 || strcmp(user_input, "enter_library\n") == 0)
        {
            sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0){fprintf(stderr, "Could not connect to host server.\n");exit(1);}

            response = NULL;
            /* Make request. */
            if (cookie != NULL) {
                message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", "/api/v1/tema/library/access", cookie, NULL);
                send_to_server(sockfd, message);

                /* Get response from server. */
                response = receive_from_server(sockfd);
            }

            /* Get responce code and JWT(if succesfull) */
            char *token = NULL;
            if (response != NULL)
                token = strtok(response, "\n");
            while (token != NULL && token[0] != '{')
                token = strtok(NULL, "\n");
            if (token != NULL)
            {
                token = strtok(token, "\"");
                for (int i = 0; i < 3; i++, token = strtok(NULL, "\""));
                /* Make room for JWT.*/
                if (JWT == NULL)
                    JWT = (char *) malloc(300 * sizeof (char));
                strcpy(JWT, token);
                printf("Access granted.\n");
            }
            else
            {
                printf("First you must log into your account.\n");
            }

            /* Close connection. */
            close_connection(sockfd);
        }

        /* Handle GET_BOOKS command. */
        else if (strcmp(user_input, "get_books") == 0)
        {
            sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0){fprintf(stderr, "Could not connect to host server.\n");exit(1);}

            response = NULL;
            if (JWT != NULL) {
                /* Make request. */
                message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", "/api/v1/tema/library/books", NULL, JWT);
                send_to_server(sockfd, message);

                /* Get response from server. */
                response = NULL;
                response = receive_from_server(sockfd);
                //puts(response);
            }

            /* parce responce. */
            char *token = NULL;
            if (response != NULL)
                token = strtok(response, "\n");
            while (token != NULL && token[0] != '[')
                token = strtok(NULL, "\n");

            if (token != NULL) {
                root_value = json_parse_string(token);
                fields = json_value_get_array(root_value);
                int number_of_books = json_array_get_count(fields);
                if (number_of_books == 0)
                    fprintf(stderr, "There are no books in your library.\n");
                //fields = json_value_get_array(root_value);
                for (int i = 0; i < number_of_books; i++) {
                    field = json_array_get_object(fields, i);

                    /* Print id. */
                    char *id = json_serialize_to_string_pretty(json_object_get_value(field, "id"));
                    if (id != NULL)
                        fprintf(stderr, "id:%s: ", id);

                    /* Print title. */
                    char *title = (char *)json_object_get_string(field, "title");
                    int title_len = strlen(title);
                    if (title[title_len - 1] == '\n') title[title_len - 1] = '\0';
                    fprintf(stderr, "%s\n", title);
                }
                json_value_free(root_value);
            }
            else {
                fprintf(stderr, "Access to the library requierd.\n");
            }

            /* Close connection. */
            close_connection(sockfd);
        }

        /* Handle GET_BOOK command. */
        else if (strcmp(user_input, "get_book") == 0)
        {
            sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0){fprintf(stderr, "Could not connect to host server.\n");exit(1);}

            response = NULL;
            if (JWT != NULL) {
                /* Get book id. */
                book_id = (char *)malloc(20 * sizeof(char));
                fprintf(stderr, "id=");
                fgets(book_id, 20, stdin);
                book_id = strtok(book_id, "\n ");

                /* Make request. */
                char *path = (char*) malloc (100 * sizeof(char));
                strcpy(path, "/api/v1/tema/library/books/");
                strcat(path, book_id);
                message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", path, NULL, JWT);
                send_to_server(sockfd, message);

                /* Get response from server. */
                response = receive_from_server(sockfd);

                free (path);
                free (book_id);
            }
            else
            {
                fprintf(stderr, "Access to the library requierd.\n");
            }
            
            /* Parce responce. */
            char *token = NULL;
            if (response != NULL)
                token = strtok(response, "\n");
            while (token != NULL && (token[0] != '{' && token[0] != '['))
                token = strtok(NULL, "\n");

            /* If it was an error message, print it. */
            if (token != NULL && token[0] == '{') {
                token = strtok(token, "\"");
                for (int i = 0; i < 3; i++, token = strtok(NULL, "\""));
                fprintf(stderr, "%s\n", token);
            }
            /* Parce JSON and print details about that book. */
            else if (token != NULL){
                root_value = json_parse_string(token);
                fields = json_value_get_array(root_value);
                field = json_array_get_object(fields, 0);

                int len;
                char *title = (char *)json_object_get_string(field, "title");
                if (title != NULL) {
                    len = strlen(title);
                    if (title[len - 1] == '\n') title[len - 1] = '\0';
                    fprintf(stderr, "title: %s\n", title);
                }

                char *author = (char *)json_object_get_string(field, "author");
                if (author != NULL) {
                    len = strlen(author);
                    if (author[len - 1] == '\n') author[len - 1] = '\0';
                    fprintf(stderr, "author: %s\n", author);
                }

                char *genre = (char *)json_object_get_string(field, "genre");
                if (genre != NULL) {
                    len = strlen(genre);
                    if (genre[len - 1] == '\n') genre[len - 1] = '\0';
                    fprintf(stderr, "genre: %s\n", genre);
                }

                char *publisher = (char *)json_object_get_string(field, "publisher");
                if (publisher != NULL) {
                    len = strlen(publisher);
                    if (publisher[len - 1] == '\n') publisher[len - 1] = '\0';
                    fprintf(stderr, "publisher: %s\n", publisher);
                }

                char *page_count = json_serialize_to_string_pretty(json_object_get_value(field, "page_count"));
                if (page_count != NULL)
                    fprintf(stderr, "page_count: %s\n", page_count);
            }

            /* Close connection. */
            close_connection(sockfd);
        }

        /* Handle ADD_BOOK command. */
        else if (strcmp(user_input, "add_book") == 0)
        {
            body_data = NULL;
            sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0){fprintf(stderr, "Could not connect to host server.\n");exit(1);}

            if (JWT == NULL) {
                fprintf(stderr, "Access to the library requierd.\n");
            }
            else {
                fprintf(stderr, "title=");
                fgets(title, 100, stdin);
                if (title[strlen(title) -1] == '\n') title[strlen(title) - 1] = '\0';

                fprintf(stderr, "author=");
                fgets(author, 100, stdin);
                if (author[strlen(author) - 1] == '\n') author[strlen(author) - 1] = '\0';

                fprintf(stderr, "genre=");
                fgets(genre, 100, stdin);
                if (genre[strlen(genre) - 1] == '\n') genre[strlen(genre) - 1] = '\0';

                fprintf(stderr, "publisher=");
                fgets(publisher, 100, stdin);
                if (publisher[strlen(publisher) - 1] == '\n') publisher[strlen(publisher) - 1] = '\0';

                fprintf(stderr, "page_count=");
                fgets(page_count_char, 100, stdin);
                if (page_count_char[strlen(page_count_char) - 1] == '\n') page_count_char[strlen(page_count_char) - 1] = '\0';
                page_count = atoi(page_count_char);

                fprintf(stderr, "%d\n", page_count );
                if (page_count > 0) {
                    /* Make space for JSON object. */
                    root_value = json_value_init_object();
                    root_object = json_value_get_object(root_value);

                    /* Populate JSON string. */
                    json_object_set_string(root_object, "title", title);
                    json_object_set_string(root_object, "author", author);
                    json_object_set_string(root_object, "genre", genre);
                    json_object_set_string(root_object, "publisher", publisher);
                    json_object_set_number(root_object, "page_count", page_count);
                    body_data = json_serialize_to_string(root_value);
                }
                else
                {
                    fprintf(stderr, "Page count should not be negative or 0.\n");
                }
                
            }

            /* Make request. */
            response = NULL;
            if (body_data != NULL) {
                message = compute_post_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", "/api/v1/tema/library/books", "application/json",
                                            body_data, NULL, JWT);
                send_to_server(sockfd, message);

                /* Get response from server. */
                response = receive_from_server(sockfd);
                if (response != NULL)
                    fprintf(stderr, "Book added succesfully!\n");
            }

            /* Close connection. */
            close_connection(sockfd);
        }

        /* Handle DELETE BOOK command. */
        else if (strcmp(user_input, "delete_book") == 0)
        {
            sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0){fprintf(stderr, "Could not connect to host server.\n");exit(1);}

            char *path = NULL;
            if (JWT != NULL) {
                /* Get book id. */
                book_id = (char *)malloc(20 * sizeof(char));
                fprintf(stderr, "id=");
                fgets(book_id, 20, stdin);
                book_id[strlen(book_id) - 1] = '\0';

                /* Make request. */
                path = (char*) malloc (100 * sizeof(char));
                strcpy(path, "/api/v1/tema/library/books/");
                strcat(path, book_id);
            }
            else {
                fprintf(stderr, "Access to the library requierd.\n");
            }

            if (path != NULL) {
                message = compute_delete_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", path, NULL, JWT);
                send_to_server(sockfd, message);

                /* Get response from server. */
                response = receive_from_server(sockfd);
                
                /* Parse response. */
                char *token = NULL;
                if (response != NULL) {
                    token = strtok(response, "\n");
                }

                while (token != NULL && token[0] != '{')
                    token = strtok(NULL, "\n");
                if (token != NULL)
                {
                    token = strtok(token, "\"");
                    for (int i = 0; i < 3; i++, token = strtok(NULL, "\""));
                    fprintf(stderr, "%s\n", token);
                }
                else
                {
                    printf("Book deleted succesfully.\n");
                }
            }

            /* Close connection. */
            close_connection(sockfd);
        }

        /* Handle LOGOUT command. */
        else if (strcmp(user_input, "logout") == 0)
        {
            sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0){fprintf(stderr, "Could not connect to host server.\n");exit(1);}

            response = NULL;
            /* Make request. */
            if (cookie != NULL) {
                message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", "/api/v1/tema/auth/logout", cookie, NULL);
                send_to_server(sockfd, message);

                /* Get response from server. */
                response = receive_from_server(sockfd);
                //puts(response);
            }

            if (response != NULL) {
                if (JWT != NULL) {
                    free(JWT);
                    JWT = NULL;
                }

                if (cookie != NULL) {
                    free(cookie);
                    cookie = NULL;
                }

                fprintf(stderr, "See you later.\n");
            }
            else {
                fprintf(stderr, "You must be logged in first.\n");
            }

            /* Close connection. */
            close_connection(sockfd);
        }

        /* Handle EXIT command. */
        else if (strcmp(user_input, "exit") == 0)
        {
            if (JWT != NULL) {
                free(JWT);
                JWT = NULL;
            }

            if (cookie != NULL) {
                free(cookie);
                cookie = NULL;
            }
            break;
        }
    }

    free(user_input_buff);

    return 0;
}