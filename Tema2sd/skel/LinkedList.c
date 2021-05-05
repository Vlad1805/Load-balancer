// Copyright 2021 Stanciu Vlad

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"
#include "utils.h"

linked_list_t*
ll_create(unsigned int data_size)
{
    linked_list_t* my_list = malloc(sizeof(linked_list_t));
    DIE(my_list == NULL, "Failed to malloc!");
    my_list->head = NULL;
    my_list->size = 0;
    my_list->data_size = data_size;
    return my_list;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca
 * n < 0, eroare.
 */
ll_node_t*
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
    ll_node_t *new, *curr;
    unsigned int pas = 1;
    DIE(list == NULL, "Lista inexistenta");
    curr = list->head;
    if (n > list->size) {
        n = list->size;
    }
    new = malloc(sizeof(ll_node_t));
    new->data = malloc(list->data_size);
    DIE(new == NULL, "Failed to malloc!");
    DIE(new->data == NULL, "Failed to malloc!");
    new->next = NULL;
    memcpy(new->data, new_data, list->data_size);
    while (curr != NULL && curr->next != NULL && n > pas) {
        curr = curr->next;
        pas++;
    }
    if (n == 0) {
        new->next = list->head;
        list->head = new;
        list->size++;
        return new;
    }
    if (n == list->size) {
        curr->next = new;
        list->size++;
        return new;
    }
    new->next = curr->next;
    curr->next = new;
    list->size++;
    return new;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, eroare. Functia intoarce un pointer spre acest
 * nod proaspat eliminat din lista. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
ll_node_t*
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
    DIE(list == NULL, "Lista nealocata!");
    unsigned int pas = 0;
    ll_node_t *kill, *prev;
    if (n == 0) {
        ll_node_t *swap = list->head;
        list->head = list->head->next;
        list->size--;
        return swap;
    }
    kill = list->head->next;
    prev = list->head;
    while (kill->next != NULL && n > pas) {
        prev = kill;
        kill = kill->next;
        pas++;
    }
    prev->next = kill->next;
    list->size--;
    return kill;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int
ll_get_size(linked_list_t* list)
{
    DIE(list == NULL, "Lista nealocata!");
    return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void
ll_free(linked_list_t** pp_list)
{
    ll_node_t *kill, *next;
    kill = (*pp_list)->head;
    while (kill != NULL) {
        next = kill->next;
        free(kill->data);
        free(kill);
        kill = next;
    }
    free(*pp_list);
    *pp_list = NULL;
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista inlantuita separate printr-un spatiu.
 */
void
ll_print_int(linked_list_t* list)
{
    ll_node_t* print;
    print = list->head;
    while (print != NULL) {
        printf("%d ", *((int*)print->data));
        print = print->next;
    }
    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista inlantuita, separate printr-un spatiu.
 */
void
ll_print_string(linked_list_t* list)
{
    ll_node_t* print;
    print = list->head;
    while (print != NULL) {
        printf("%s ", (char*)print->data);
        print = print->next;
    }
    printf("\n");
}
