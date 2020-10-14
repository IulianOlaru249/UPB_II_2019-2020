#include "arp.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <arpa/inet.h>

int read_rtable (struct route_table_entry **rtable, int route_table_size)
{
    FILE *tableData = fopen ("rtable.txt", "r");

    if (tableData == NULL) {
        perror("File cannot be opened!");
        exit(1);
    }

    /* buffer to store a line from the table file 
     * expected to get at most 3 addressed and 1 
     * value for the interface 
     */
    char buff[3 * IPV4_LENGTH + 32], c;
    int buff_index = 0;

    /* buffer for the new entries before put in array */
    struct route_table_entry new_route_table_entry;

    char *token;

    /* Read the file character by character */
    c = fgetc (tableData);
    while (c != EOF)
    {
        /* One line from the arp table has been read*/
        if (c == '\n') {
            buff[buff_index] = '\0';
            
            /* Parse the line */
           token = strtok (buff, " ");

           inet_pton(AF_INET, token, &new_route_table_entry.prefix);
           token = strtok (NULL, " ");

            inet_pton (AF_INET, token, &new_route_table_entry.next_hop);
            token = strtok (NULL, " ");

            inet_pton (AF_INET, token, &new_route_table_entry.mask);
            token = strtok (NULL, " ");
                
            new_route_table_entry.interface = atoi (token);

            //make room for the new entry
            route_table_size ++;
            *rtable = (struct route_table_entry *) realloc (*rtable,
                sizeof(struct route_table_entry) * route_table_size);

            /* store the entry in the array */
            struct route_table_entry *my_table = *rtable;
            memcpy(&my_table[arp_table_size - 1],
                    &new_route_table_entry,
                    sizeof(struct route_table_entry));

            /* Reset buffer index for a new line*/
            buff_index = 0;
        }
        else {
            buff[buff_index] = c;
            buff_index ++;
        }

        c = fgetc (tableData);
    }

    fclose (tableData);
    return route_table_size;
}

void print_route_table_entry(struct route_table_entry entry)
{
    for (int i = 0; i < 20; i ++) printf("*");

    printf ("\nprefix: %d\nnext_hop: %d\nmask: %d\ninterface: %d\n",
             entry.prefix,  
             entry.next_hop,
             entry.mask,
             entry.interface );

    for (int i = 0; i < 20; i ++) printf("*");
    printf("\n");
}

void print_route_table (struct route_table_entry * rtable, int route_table_size)
{
    for (int i = 0; i < route_table_size; i++)
        print_route_table_entry (rtable[i]); 
}
