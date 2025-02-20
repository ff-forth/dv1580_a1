#include "linked_list.h"
#include <stdio.h>  
#include <stdlib.h>
#include <stdlib.h>
#include <stdbool.h>
#include "memory_manager.h"
#include <string.h>

void list_init(Node** head, size_t size)
{
    // check if size is greater than 0
    if (size <= 0)
    {
        fprintf(stderr, "list_init failed: Too small, list size is %zu\n", size);
        return;
    }
    
    // initialze list
    mem_init(size);
};

void list_insert(Node** head, uint16_t data)
{
    // Create a new node
    Node* new_node = mem_alloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed in list_insert\n");
        return;
    }
    // Initialize all fields
    new_node->data = data;
    new_node->next = NULL;
    
    // Check if list is empty
    if (*head == NULL)
    {
        *head = new_node;
        return;
    }
    
    // Defind a node
    Node* cur_node = *head;

    // Find the end node of the list
    while(cur_node->next != NULL)
    {
        cur_node = cur_node->next;
    }
    
    // Insert the new node to the end of the list
    cur_node->next = new_node;
};

void list_insert_after(Node* prev_node, uint16_t data)
{
    if (prev_node == NULL) {
        fprintf(stderr, "Previous node cannot be NULL\n");
        return;
    }

    // Create a new node
    Node* new_node = mem_alloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed in list_insert_after\n");
        return;
    }
    // Initialize all fields
    new_node->data = data;
    new_node->next = prev_node->next;
    prev_node->next = new_node;
};

void list_insert_before(Node** head, Node* next_node, uint16_t data)
{
    if (next_node == NULL) {
        fprintf(stderr, "Next node cannot be NULL\n");
        return;
    }

    // Create a new node
    Node* new_node = mem_alloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed in list_insert_before\n");
        return;
    }
    // Initialize all fields
    new_node->data = data;
    new_node->next = next_node;

    // check if next_node is the head of the list
    if (*head == next_node)
    {
        *head = new_node;
        return;
    }

    // Defind a node
    Node* cur_node = *head;

    // Find the end node of the list
    while(cur_node->next != next_node)
    {
        cur_node = cur_node->next;
    }
    
    // Insert the new node after the current node
    cur_node->next = new_node;
};

void list_delete(Node** head, uint16_t data)
{
    // Find the node
    Node* node = list_search(head, data);

    // Check if the node of data exists
    if (node == NULL)
    {
        fprintf(stderr, "list_delete failed, node of data %hu can not find\n", data);
        return;
    }

    // Check if deleted node is head
    if (node == *head)
    {
        *head = node->next;
        mem_free(node);
        return;
    }

    // Defind a node
    Node* cur_node = *head;

    // Find the end node of the list
    while(cur_node->next != node)
    {
        cur_node = cur_node->next;
    }
    
    // Link the node's next node to the current node
    cur_node->next = node->next;
    
    // Remove the node
    mem_free(node);
};

Node* list_search(Node** head, uint16_t data)
{
    // Check if any node exist in the list
    if (list_count_nodes(head) == 0)
    {
        fprintf(stderr, "list_search failed, list is empty.\n");
        return NULL;
    }
    
    // Defind a node
    Node* cur_node = *head;

    // Find the node of the data
    while (cur_node->data != data)
    {
        if (cur_node->next == NULL) return NULL;
        cur_node = cur_node->next;
    }
    
    return cur_node;
};

void list_display(Node** head)
{
    list_display_range(head,NULL,NULL);
};

void list_display_range(Node** head, Node* start_node, Node* end_node)
{
    if (start_node == NULL) start_node = *head;

    Node* cur_node = start_node;

    printf("[");
    while (cur_node->next != NULL)
    {
        if (cur_node == end_node) break;
        
        printf("%d, ", cur_node->data);
        cur_node = cur_node->next;
    }

    printf("%d]", cur_node->data);
};

int list_count_nodes(Node** head)
{
    int num = 0;
    Node* cur_node = *head;
    while(cur_node != NULL)
    {
        num++;
        cur_node = cur_node->next;
    }
    return num;
};

void list_cleanup(Node** head)
{
    while (*head != NULL)
    {
        list_delete(head, (*head)->data);
    }
    mem_deinit();
};
