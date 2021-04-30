#ifndef __CIRCULAR_DOUBLY_LINKED_LIST_H_
#define __CIRCULAR_DOUBLY_LINKED_LIST_H_

typedef struct dll_node_t dll_node_t;
struct dll_node_t
{
    void* data; /* Pentru ca datele stocate sa poata avea orice tip, folosim un
                   pointer la void. */
    dll_node_t *prev, *next;
};

typedef struct doubly_linked_list_t doubly_linked_list_t;
struct doubly_linked_list_t
{
    dll_node_t* head;
    unsigned int data_size;
    unsigned int size;
};

/*
 * Acestea sunt functiile pe care trebuie sa le implementam.
 * Implementarea acestora se va face in LinkedList.c .
 */

doubly_linked_list_t*
dll_create(unsigned int data_size);

dll_node_t*
dll_get_nth_node(doubly_linked_list_t* list, unsigned int n);

void
dll_add_nth_node(doubly_linked_list_t* list, unsigned int n, const void* data);

dll_node_t*
dll_remove_nth_node(doubly_linked_list_t* list, unsigned int n);

unsigned int
dll_get_size(doubly_linked_list_t* list);

void
dll_free(doubly_linked_list_t** pp_list);

void
dll_print_int_list(doubly_linked_list_t* list);

void
dll_print_string_list(doubly_linked_list_t* list);

void
dll_print_ints_left_circular(dll_node_t* start);

void
dll_print_ints_right_circular(dll_node_t* start);
#endif /* __CIRCULAR_DOUBLY_LINKED_LIST_H_*/
