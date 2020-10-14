#ifndef __AVL_H__
#define __AVL_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
	int value;
	int index;
	struct node *left;
	struct node *right;
	struct node *parent;
	int height;
} *Node;

int myIndex;


Node newNode(int value);
Node freeNode(Node node);
Node rotateRight(Node node);
Node rotateLeft(Node y);
Node insert(Node node, int newValue);
Node insertAtIndex(Node node, int newValue, int index);
Node lookup(Node root, int index);
Node set(Node root, int newValue, int index);
Node freeTree(Node root);
int getBalanceFactor(Node root);
int getHeight(Node root);
void printTree(Node root);
struct Node deleteNode(struct Node root, int key);

#endif
