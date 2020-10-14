#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url, char *cookie, char *JWT) {
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    /* Write the method name, URL */
    sprintf(line, "GET %s HTTP/1.1", url);
    compute_message(message, line);

    /* Add the host. */
    if (host != NULL) {
        sprintf(line, "Host: %s", host);
    }
    compute_message(message, line);

    /* Add cookies. */
    if (cookie != NULL) {
        sprintf(line, "Cookie: %s", cookie);
        compute_message(message, line);
    }

    /* Add JWT. */
    if (JWT != NULL) {
        sprintf(line, "Authorization: Bearer %s", JWT);
        compute_message(message, line);
    }

    /* Add final new line. */
    compute_message(message, "");
    return message;
}

char *compute_delete_request(char *host, char *url, char *cookie, char *JWT) {
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    /* Write the method name, URL */
    sprintf(line, "DELETE %s HTTP/1.1", url);
    compute_message(message, line);

    /* Add the host. */
    if (host != NULL) {
        sprintf(line, "Host: %s", host);
    }
    compute_message(message, line);

    /* Add cookies. */
    if (cookie != NULL) {
        sprintf(line, "Cookie: %s", cookie);
        compute_message(message, line);
    }

    /* Add JWT. */
    if (JWT != NULL) {
        sprintf(line, "Authorization: Bearer %s", JWT);
        compute_message(message, line);
    }

    /* Add final new line. */
    compute_message(message, "");
    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char *body_data,
                            char *cookie, char *JWT)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    /* Write the method name, URL and protocol type. */
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    /* Add the host */
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    /* Add cookies. */
    if (cookie != NULL) {
        sprintf(line, "Cookie: %s", cookie);
        compute_message(message, line);
    }

    /* Add JWT. */
    if (JWT != NULL) {
        sprintf(line, "Authorization: Bearer %s", JWT);
        compute_message(message, line);
    }

    /* Add the content type. */
    sprintf(line, "Content-Type: application/json");
    compute_message(message, line);
    
    /* Add the content length. */
    int length = strlen(body_data);
    sprintf(line, "Content-Length: %d", length);
    compute_message(message, line);

    /* Add new line at end of headers. */
    compute_message(message, "");

    /* Add the actual payload data. */
    compute_message(message, body_data);
    free(line);
    return message;
}
