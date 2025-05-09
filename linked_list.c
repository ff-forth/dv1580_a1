#include "linked_list.h"

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
    *head = NULL;
};

void list_insert(Node** head, uint16_t data)
{
    // Create a new node
    Node* new_node = mem_alloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed in list_insert\n");
        return;
    }
    
    // Initialize the new node
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
    // Initialize the new node
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
    // Initialize the new node
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
    
    if (!start_node) 
    {
        fprintf(stderr, "list_display_range failed, list is empty.\n");
        return;
    }

    Node* cur_node = start_node;

    printf("[");
    while (cur_node != NULL)
    {
        printf("%d", cur_node->data);
        
        if (cur_node == end_node) break;
        
        cur_node = cur_node->next;
        if (cur_node) printf(", ");
    }

    printf("]");
};

int list_count_nodes(Node** head)
{
    if (*head == NULL) 
    {
        fprintf(stderr, "list_count_nodes failed, list is empty.\n");
        return 0;
    }
    
    int count = 0;
    Node* cur_node = *head;
    while(cur_node != NULL)
    {
        count++;
        cur_node = cur_node->next;
    }
    return count;
};

void list_cleanup(Node** head)
{
    Node* cur_node = *head;
    while (cur_node != NULL)
    {
        Node* next_node = cur_node->next;
        mem_free(cur_node);
        cur_node = next_node;
    }
    *head = NULL;
    mem_deinit();
};
