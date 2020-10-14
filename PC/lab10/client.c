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

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;

        
    sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
    // Ex 1.1: GET dummy from main server
    message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", "/api/v1/dummy", NULL, NULL, 0);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    // Ex 1.2: POST dummy and print response from main server
    char *content_type = "application/x-www-form-urlencoded";
    char **body_data = malloc (2 * sizeof (char *));
    body_data[0] = "Hello";
    body_data[1] = "World!";

    message = compute_post_request ("3.8.116.10", "/api/v1/dummy", content_type,
                                    body_data, 2, NULL, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);

    // Ex 2: Login into main server
    content_type = "application/x-www-form-urlencoded";
    body_data[0] = "username=student";
    body_data[1] = "password=student";

    message = compute_post_request ("3.8.116.10", "/api/v1/auth/login", content_type,
                                    body_data, 2, NULL, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);

    // Ex 3: GET weather key from main server
    char **keys = (char **) malloc (sizeof(char *));
    keys[0] = (char *) malloc (100 * sizeof (char));
    strcpy (keys[0], "connect.sid=s%3AvtZiwQrrtbjC6NUw9cHuutDtM7nznhiZ.VOC4jxJxIWJ3UUwJwrbGiXY8nwKo6wLEEsu3XEz4RCo");

    message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", "/api/v1/weather/key", NULL, keys, 1);

    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    // Ex 4: GET weather data from OpenWeather API
    message = compute_get_request("api.openweathermap.org", "/data/2.5/weather", NULL, NULL, 0);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);



    // Ex 5: POST weather data for verification to main server

    // BONUS: make the main server return "Already logged in!"
    content_type = "application/x-www-form-urlencoded";
    body_data[0] = "username=student";
    body_data[1] = "password=student";

    char **cookies = (char **) malloc (sizeof(char *));
    cookies[0] = (char *) malloc (100 * sizeof (char));
    strcpy (cookies[0], "connect.sid=s%3AeZ-09mWjVkRjasBZuZEfIm2MxdkdgZaI.azrelyu6fgXplI0m5ilSvM2iQ9SyMnr%2FG9Xhmez%2BKn8");

    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    // Ex 6: Logout from main server
    message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", "/api/v1/auth/logout", NULL, NULL, 0);

    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);




    // free the allocated data at the end!
    free (body_data);
    free (keys[0]);
    free (keys);
    close_connection(sockfd);
    return 0;
}
