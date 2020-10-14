#include "avl.h"

Node newNode(int value)
{
	Node newNode = (Node) malloc(sizeof (struct node));
	newNode->value = value;
	newNode->index = -1;
	myIndex ++;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->height = 1;
	return newNode;
}

Node freeNode(Node node)
{
	if(node != NULL)
		free(node);
	return NULL;
}

Node rotateRight(Node y)
{
	Node x = y->left;
	Node t2 = x->right;
	
	x->right = y;
	y->left = t2;

	if(getHeight(x->left) > getHeight(x->right))
		x->height = getHeight(x->left) + 1; 
	else
		x->height = getHeight(x->right) + 1; 

	if(getHeight(y->left) > getHeight(y->right))
		y->height = getHeight(y->left) + 1; 
	else
		y->height = getHeight(y->right) + 1; 

	return x;
}

Node rotateLeft(Node x)
{
	Node y = x->right;
	Node t2 = y->left;

	y->left = x;
	x->right = t2;

	if(getHeight(x->left) > getHeight(x->right))
		x->height = getHeight(x->left) + 1; 
	else
		x->height = getHeight(x->right) + 1; 

	if(getHeight(y->left) > getHeight(y->right))
		y->height = getHeight(y->left) + 1; 
	else
		y->height = getHeight(y->right) + 1; 

	return y;
}

Node insertAtIndex(Node node, int newValue, int index)
{
}

Node lookup(Node root, int index)
{
	if(root == NULL || index == root->value)
		return root;
	if(index < root->value)
		return lookup(root->left, index);
	else 
		return lookup(root->right, index);
}

Node set(Node root, int newValue, int index)
{
	Node n = lookup(root, index);
	n->value = newValue;
	return n;
}

Node insert(Node node, int newValue)
{
	if(node == NULL)
		return newNode(newValue);

	if(newValue < node->value)
		node->left = insert(node->left, newValue);
	else if(newValue > node->value)
		node->right = insert(node->right, newValue);
	else
		return node;

	if(getHeight(node->left) > getHeight(node->right))
		node->height = getHeight(node->left) + 1; 
	else
		node->height = getHeight(node->right) + 1; 

	int balanceFactor = getBalanceFactor(node);

	if(balanceFactor > 1 && newValue < node->left->value)
		return rotateRight(node);	

	if(balanceFactor < -1 && newValue > node->right->value)
		return rotateLeft(node);	

	if(balanceFactor > 1 && newValue > node->left->value){
		node->left = rotateLeft(node->left);
		return rotateRight(node);
	}

	if(balanceFactor < -1 && newValue < node->right->value){
		node->right = rotateRight(node->right);
		return rotateLeft(node);
	}
	
	return node;
}

Node freeTree(Node root)
{
	if(root != NULL){
		printTree(root->left);
		printTree(root->right);
		root = freeNode(root);
	}
	return NULL;
}

int getBalanceFactor(Node root)
{
	if(root == NULL)	
		return 0;
	return getHeight(root->left) - getHeight(root->right);
}

int getHeight(Node root)
{
	if(root == NULL)
		return 0;
	return root->height;	
}

void printTree(Node root)
{
	if(root != NULL){
		printf("%d ", root->value);
		printTree(root->left);
		printTree(root->right);
	}
    return;
}

struct Node deleteNode(struct Node root, int key) 
{ 
    if (root == NULL) 
        return root; 
  
    if ( key < root->key ) 
        root->left = deleteNode(root->left, key); 
  
    else if( key > root->key ) 
        root->right = deleteNode(root->right, key); 
  
    else
    { 
        if( (root->left == NULL) || (root->right == NULL) ) 
        { 
            struct Node temp = root->left ? root->left : 
                                             root->right; 
  
            // No child case 
            if (temp == NULL) 
            { 
                temp = root; 
                root = NULL; 
            } 
            else // One child case 
             root = temp;
                         
            free(temp); 
        } 
        else
        { 
            // node with two children: Get the inorder 
            // successor (smallest in the right subtree) 
            struct Node temp = minValueNode(root->right); 
  
            // Copy the inorder successor's data to this node 
            root->key = temp->key; 
  
            // Delete the inorder successor 
            root->right = deleteNode(root->right, temp->key); 
        } 
    } 
  
    // If the tree had only one node then return 
    if (root == NULL) 
      return root; 
  
    root->height = 1 + max(height(root->left), 
                           height(root->right)); 
  
    int balance = getBalance(root); 
  
    // If this node becomes unbalanced, then there are 4 cases 
  
    // Left Left Case 
    if (balance > 1 && getBalance(root->left) >= 0) 
        return rightRotate(root); 
  
    // Left Right Case 
    if (balance > 1 && getBalance(root->left) < 0) 
    { 
        root->left =  leftRotate(root->left); 
        return rightRotate(root); 
    } 
  
    // Right Right Case 
    if (balance < -1 && getBalance(root->right) <= 0) 
        return leftRotate(root); 
  
    // Right Left Case 
    if (balance < -1 && getBalance(root->right) > 0) 
    { 
        root->right = rightRotate(root->right); 
        return leftRotate(root); 
    } 
  
    return root; 
} 
