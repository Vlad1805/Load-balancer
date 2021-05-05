/* Copyright 2021 Stanciu Vlad */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "server.h"

unsigned int hash_function_key(void *a) {
    unsigned char *puchar_a = (unsigned char *) a;
    unsigned int hash = 5381;
    int c;

    while ((c = *puchar_a++))
        hash = ((hash << 5u) + hash) + c;

    return hash;
}


server_memory* init_server_memory() {
	server_memory *server = calloc(1, sizeof(server_memory));
	server->ht = ht_create(HASH_MAX, hash_function_key, compare_function_strings);
	return server;
}

void server_store(server_memory* server, char* key, char* value) {
	ht_put(server->ht, key, strlen(key) + 1, value, strlen(value) + 1);
}

void server_remove(server_memory* server, char* key) {
	ht_remove_entry(server->ht, key);
}

char* server_retrieve(server_memory* server, char* key) {
	return (char*)ht_get(server->ht, key);
}

void free_server_memory(server_memory* server) {
	ht_free(server->ht);
	free(server);
}

/*
	return 1 if object should be in server dest
	return 0 if object should remain in server src
*/

int check_update_server(server_memory *dest, server_memory *src,
unsigned int hash)
{
	if (dest->hash < src->hash) {
		if (hash < dest->hash) {
			return 1;
		} else if (hash > src->hash) {
			return 1;
		}
		return 0;
	}
	if (hash > src->hash && hash < dest->hash) {
		return 1;
	}
	return 0;
}

/*
	Keep in mind that dest and src must be neighbours.
	Function called when server dest is added.
	Checks all objects from src and moves them if they should be in dest.
*/

void update_server(server_memory *dest, server_memory *src) {
	if (dest->id == src->id) {
		return;
	}
	for (unsigned int i = 0 ; i < src->ht->hmax ; i++) {
		ll_node_t *curr = (src->ht->buckets[i])->head, *kill;
		int runner = 0;
		char *key;
		while (curr != NULL)
		{
			key = (char*)(((info*)curr->data)->key);
			curr = curr->next;
			unsigned int hash = hash_function_key(key);
			if (check_update_server(dest, src, hash)) {
				kill = ll_remove_nth_node(src->ht->buckets[i], runner);
				server_store(dest, key, (char*)(((info*)kill->data)->value));
				free(((info*)kill->data)->key);
				free(((info*)kill->data)->value);
				free(kill->data);
				free(kill);
				runner--;
			}
		runner += 1;
		}
	}
}

/*
	Function called when server src is removed.
	Keep in mind that dest and src must be neighbours.
*/

void update_and_free_server(server_memory *dest, server_memory *src) {
	for (unsigned int i = 0 ; i < src->ht->hmax ; i++) {
		ll_node_t *curr = (src->ht->buckets[i])->head, *kill;
		char *key;
		while (curr != NULL)
		{
			key = (char*)((info*)curr->data)->key;
			curr = curr->next;
			kill = ll_remove_nth_node(src->ht->buckets[i], 0);
			server_store(dest, key, (char*)((info*)kill->data)->value);
			free(((info*)kill->data)->key);
			free(((info*)kill->data)->value);
			free(kill->data);
			free(kill);
		}
		free(src->ht->buckets[i]);
	}
	free(src->ht->buckets);
	free(src->ht);
	free(src);
}
