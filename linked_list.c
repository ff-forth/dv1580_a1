#include "linked_list.h"
#include <stdio.h>  
#include <stdlib.h>
#include <stdlib.h>
#include <stdbool.h>
#include "memory_manager.h"
#include <string.h>

void list_init(Node** head, size_t size)
{
    mem_init(size);
};

void list_insert(Node** head, uint16_t data)
{
    Node* new_node = mem_alloc(sizeof(Node));
    new_node->data = data;

    Node* cur_node = *head;
    for (int i = -1; i<list_count_nodes(head); i++){
        // printf("insert %d\n", data);
        if (cur_node == NULL)
        {
            *head = new_node;
            break;
        }
        else if (cur_node->next == NULL)
        {
            cur_node->next = new_node;
            // printf("inserted\n");
            break;
        }
        else
        {
            cur_node = cur_node->next;
        }
    }
    // printf("insert %d at %p\n", data, cur_node);
};

void list_insert_after(Node* prev_node, uint16_t data)
{
    Node* new_node = mem_alloc(sizeof(Node));
    new_node->data = data;
    new_node->next = prev_node->next;
    prev_node->next = new_node;
};

void list_insert_before(Node** head, Node* next_node, uint16_t data){
    Node* new_node = mem_alloc(sizeof(Node));
    new_node->data = data;
    new_node->next = next_node;

    Node* cur_node = *head;
    int num = list_count_nodes(head);
    // printf("Insert before--->\n");
    for (int i = 0; i<num; i++)
    {
        // printf("current: %p\n", cur_node);
        if (cur_node == next_node)
        {
            *head = new_node;
            break;
        }
        else if (cur_node->next == next_node)
        {
            cur_node->next = new_node;
            break;
        }
        else
        {
            cur_node = cur_node->next;
        }
    }
    // printf("current: %p\n", cur_node);
};

void list_delete(Node** head, uint16_t data){
    Node* cur_node = *head;
    for (int i = -1; i < list_count_nodes(head); i++)
    {
        if ((*head)->data == data)
        {
            *head = cur_node->next;
            mem_free(cur_node);
        }
        else if (cur_node->next->data == data)
        {
            Node* temp = cur_node->next;
            cur_node->next = temp->next;
            mem_free(temp);
        }
        else if (cur_node->next != NULL)
        {
            cur_node = cur_node->next;
        }
    }
};

Node* list_search(Node** head, uint16_t data){
    Node* cur_node = *head;
    for (int i = -1; i < list_count_nodes(head); i++)
    {
        if (cur_node->data == data)
        {
            return cur_node;
        }
        else if (cur_node->next != NULL)
        {
            cur_node = cur_node->next;
        }
    }
    return NULL;
};

void list_display(Node** head){
    printf("\n[ ");
    
    Node* cur_node = *head;
    for (int i = -1; i < list_count_nodes(head); i++)
    {
        printf("%d, ", cur_node->data);
        cur_node = cur_node->next;
    }
    printf("]\n");
};

void list_display_range(Node** head, Node* start_node, Node* end_node){
    printf("HELLO from display\n");
    if (start_node == NULL && end_node == NULL) 
    {
        list_display(head);
    }
    else
    {   
        if (start_node == NULL)
        {
            start_node = *head;
        }
        // if (end_node == NULL)
        // {
        //     start_node = NULL;
        // }

        printf("\n[ ");
        Node* cur_node = start_node;
        for (int i = -1; i < list_count_nodes(head); i++)
        {
            printf("%d, ", cur_node->data);
            if(cur_node == end_node)
            {
                break;
            }
            else
            {
                cur_node = cur_node->next;
            }
        }
        printf("]\n");
    }
};

int list_count_nodes(Node** head){
    int num = 0;
    Node* cur_node = *head;
    while(cur_node != NULL)
    {
        // printf("%d:%p\n" ,cur_node->data,cur_node);
        num++;
        cur_node = cur_node->next;
    }
    //printf(" num = %d\n", num);
    return num;
};

void list_cleanup(Node** head)
{
    mem_deinit();
};


// int main()
// {
//     printf("  Testing list_insert ---> ");
//     Node *head = NULL;
//     list_init(&head, sizeof(Node) * 2);
//     list_insert(&head, 10);
//     list_insert(&head, 20);
//     if(head->data == 10 && head->next->data == 20)
//     printf("[PASS].\n");
//     list_cleanup(&head);
//     return 0;
// }
