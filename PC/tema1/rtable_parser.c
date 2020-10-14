#include "rtable_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <arpa/inet.h>


int compare (const void *a, const void *b)
{
    struct route_table_entry *entry1 = (struct route_table_entry *) a;
    struct route_table_entry *entry2 = (struct route_table_entry *) a;

    if (entry1->prefix == entry2->prefix )
        return entry1->mask - entry2->mask;
    else
        return entry1->prefix - entry2->prefix;
}


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
            memcpy(&my_table[route_table_size - 1],
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

/* Looks for a matching entry in the routing table and returns the best 
 * posiible match.
 */

int binary_search (struct route_table_entry *rtable, 
                    int l, int r, uint32_t destination_ip)
{
    while (l <= r) {
        int mid = l + (r - l) / 2;

        if ((destination_ip & rtable[mid].mask) == rtable[mid].prefix)
            return mid;

        if ((destination_ip & rtable[mid].mask) < rtable[mid].prefix)
            return binary_search(rtable, l, mid - 1, destination_ip);

        if ((destination_ip & rtable[mid].mask) > rtable[mid].prefix)
            return binary_search(rtable, mid + 1, r, destination_ip);
    }
    return -1;
}

struct route_table_entry *get_best_route (uint32_t destination_ip,
                                          int route_table_size,
                                          struct route_table_entry *rtable)
{
    uint32_t max_mask = 0;
    int route_index = -1;
    
    for (int i = 0; i < route_table_size; i++) {
        if ((destination_ip & rtable[i].mask) == rtable[i].prefix) {
            if (rtable[i].mask >= max_mask) {
                route_index = i;
                max_mask = rtable[i].mask;
            }
        }
    }

    if (route_index != -1)
        return &rtable[route_index]; 
    return NULL;
/*
    int index = binary_search (rtable, 0, route_table_size - 1, destination_ip);
    if (index == -1) 
        return NULL;
    else 
        return &rtable[index];
*/
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
