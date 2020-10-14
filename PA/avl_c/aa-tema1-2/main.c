#include "avl.h"

extern int myIndex;

int main(void)
{
	Node root = NULL;

    int val;
    while (1) {
        printf("Insert val: ");
        scanf("%d", &val);

	    root = insert(root, val);
	    printTree(root);
        printf ("\n");

        printf("Lookup val: ");

        scanf("%d", &val);
	    Node l = lookup(root, val);
	    if(l == NULL)
		    printf("No such elemetn\n");
	    else
		    printf("%d\n" , l->value);
    }

	printf("\n");
	return 0;
}
