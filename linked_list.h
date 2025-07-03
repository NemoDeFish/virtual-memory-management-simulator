#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node {
    unsigned int page;
    struct Node* next;
    struct Node* prev;
} Node;

// Creates and returns a new node with given data.
Node* create_node(unsigned int pnumber);

// Adds a node with page number to the head of the doubly linked list.
void add_head(Node** head, Node** tail, unsigned int pnumber);

// Adds a node with page number to the tail of the doubly linked list.
void add_tail(Node** head, Node** tail, unsigned int pnumber);

// Removes and returns the page number from the head of the doubly linked list.
unsigned int remove_head(Node** head, Node** tail);

// Finds a node by page number and moves it to the head.
unsigned int find_move_head(Node** head, Node** tail, int pnumber);

// Free the entire list.
void freeList(Node* head);

#endif