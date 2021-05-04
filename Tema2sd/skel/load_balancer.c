/* Copyright 2021 <> */
#include <stdlib.h>
#include <string.h>

#include "load_balancer.h"
#include "data_structures/CircularDoublyLinkedList.h"
#include "utils.h"

#define REPLICA 100000

struct load_balancer {
	doubly_linked_list_t *hash_ring;
};

unsigned int hash_function_servers(void *a) {
    unsigned int uint_a = *((unsigned int *)a);

    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = (uint_a >> 16u) ^ uint_a;
    return uint_a;
}



load_balancer* init_load_balancer() {
	load_balancer *main = calloc(1, sizeof(load_balancer));
    main->hash_ring = dll_create(sizeof(server_memory));
    return main;
}

dll_node_t*
dll_get_hash(doubly_linked_list_t *list, unsigned int hash)
{
    DIE(list == NULL, "Lista nealocata!");
    dll_node_t* curr = list->head;
    if (hash > ((server_memory*)list->head->prev->data)->hash) {
        return list->head;
    }
    while (((server_memory*)curr->data)->hash < hash) {
        curr = curr->next;
    }
    return curr;
}

dll_node_t*
dll_add_server(doubly_linked_list_t *list, unsigned int hash, const void* new_data)
{
    DIE(list == NULL, "Lista nealocata!");
    dll_node_t* curr = list->head, *new;
    new = malloc(sizeof(dll_node_t));
    DIE(new == NULL, "Failed to malloc!");
    new->data = malloc(list->data_size);
    DIE(new->data == NULL, "Failed to malloc!");
    new->next = NULL;
    new->prev = NULL;
    memcpy(new->data, new_data, list->data_size);
    if (list->size == 0) {
        list->head = new;
        list->head->prev = list->head;
        list->head->next = list->head;
        list->size++;
        return list->head;
    }
    if (((server_memory*)list->head->data)->hash > hash) {
        new->next = list->head;
        new->prev = list->head->prev;
        list->head->prev->next = new;
        list->head->prev = new;
        list->head = new;
        list->size++;
        return list->head;
        // check if its the first element
    }
    if (((server_memory*)list->head->prev->data)->hash < hash) {
        new->next = list->head;
        new->prev = list->head->prev;
        list->head->prev->next = new;
        list->head->prev = new;
        list->size++;
        return list->head->prev;
        //check if its the last element 
    }
    while (((server_memory*)curr->data)->hash < hash) {
        curr = curr->next;
    }
    new->next = curr;
    new->prev = curr->prev;
    curr->prev->next = new;
    curr->prev = new;
    list->size++;
    return new;
}

void loader_store(load_balancer* main, char* key, char* value, int* server_id) {
	unsigned int hash = hash_function_key(key);
    dll_node_t *node = dll_get_hash(main->hash_ring, hash);
    *server_id = ((server_memory*)node->data)->id % REPLICA;
    server_store((server_memory*)node->data, key, value);
}


char* loader_retrieve(load_balancer* main, char* key, int* server_id) {
	unsigned int hash = hash_function_key(key);
    dll_node_t *node = dll_get_hash(main->hash_ring, hash);
    *server_id = ((server_memory*)node->data)->id % REPLICA;
	return server_retrieve((server_memory*)node->data, key);
}

void __loader_add_server(load_balancer* main, int server_id) {
    server_memory *server = init_server_memory();
    server->hash = hash_function_servers(&server_id);
    server->id = server_id;
    dll_node_t *node = dll_add_server(main->hash_ring, server->hash, server);
    free(server);
    update_server((server_memory*)node->data, (server_memory*)node->next->data);
}

void loader_add_server(load_balancer* main, int server_id) {
	__loader_add_server(main, server_id);
    __loader_add_server(main, server_id + REPLICA);
    __loader_add_server(main, server_id + 2 * REPLICA);
}

void __loader_remove_server(load_balancer *main, int server_id) {
    unsigned int hash = hash_function_servers(&server_id);
    dll_node_t *node = dll_get_hash(main->hash_ring, hash);
    update_and_free_server((server_memory*)node->next->data, (server_memory*)node->data);
    if (main->hash_ring->head == node) {
        main->hash_ring->head = node->next;
    }
    node->prev->next = node->next;
    node->next->prev = node->prev;
    main->hash_ring->size -= 1;
    free(node);
}

void loader_remove_server(load_balancer* main, int server_id) {
	__loader_remove_server(main, server_id);
    __loader_remove_server(main, (server_id + REPLICA) % (3 * REPLICA));
    __loader_remove_server(main, (server_id + 2 * REPLICA) % (3 * REPLICA));
}

void free_load_balancer(load_balancer* main) {
    unsigned int pas = 0;
    dll_node_t *kill, *next;
    kill = main->hash_ring->head;
    while (main->hash_ring->size) {
        kill = dll_remove_nth_node(main->hash_ring, 0);
        free_server_memory((server_memory*)kill->data);
        free(kill);
    }
    free(main->hash_ring);
    free(main); 
}
