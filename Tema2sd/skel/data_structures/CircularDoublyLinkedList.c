#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CircularDoublyLinkedList.h"
#include "../utils.h"

typedef struct server_memory server_memory;


/*
 * Functie care trebuie apelata pentru alocarea si initializarea unei liste.
 * (Setare valori initiale pentru campurile specifice structurii LinkedList).
 */
doubly_linked_list_t*
dll_create(unsigned int data_size)
{
    doubly_linked_list_t* my_list = malloc(sizeof(doubly_linked_list_t));
    DIE(my_list == NULL, "Failed to malloc!");
    my_list->head = NULL;
    my_list->size = 0;
    my_list->data_size = data_size;
    return my_list;
}

/*
 * Functia intoarce un pointer la nodul de pe pozitia n din lista.
 * Pozitiile din lista sunt indexate incepand cu 0 (i.e. primul nod din lista se
 * afla pe pozitia n=0). Daca n >= nr_noduri, atunci se intoarce nodul de pe
 * pozitia rezultata daca am "cicla" (posibil de mai multe ori) pe lista si am
 * trece de la ultimul nod, inapoi la primul si am continua de acolo. Cum putem
 * afla pozitia dorita fara sa simulam intreaga parcurgere? Daca n < 0, eroare.
 */
dll_node_t*
dll_get_nth_node(doubly_linked_list_t* list, unsigned int n)
{
    int i;
    DIE(list == NULL, "Lista nealocata!");
    n = n % list->size;
    dll_node_t* curr = list->head;
    for (i = 0 ; i < n ; i++) {
        curr = curr->next;
    }
    return curr;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Cand indexam pozitiile nu "ciclam" pe lista circulara ca la
 * get, ci consideram nodurile in ordinea de la head la ultimul (adica acel nod
 * care pointeaza la head ca nod urmator in lista). Daca n >= nr_noduri, atunci
 * adaugam nodul nou la finalul listei. Daca n < 0, eroare.
 */
void
dll_add_nth_node(doubly_linked_list_t* list, unsigned int n, const void* new_data)
{
    dll_node_t *new, *curr;
    unsigned int pas = 1;
    DIE(list == NULL, "Lista inexistenta!");
    DIE(n < 0, "EROARE");
    curr = list->head;
    if (n > list->size) {
        n = list->size;
    }
    new = malloc(sizeof(dll_node_t));
    DIE(new == NULL, "Failed to malloc!");
    new->data = malloc(list->data_size);
    DIE(new->data == NULL, "Failed to malloc!");
    new->next = NULL;
    new->prev = NULL;
    memcpy(new->data, new_data, list->data_size);
    if (n == 0) {
        if (list->size == 0) {
            list->head = new;
            list->head->prev = list->head;
            list->head->next = list->head;
            list->size++;
            return;
        }
        new->next = list->head;
        new->prev = list->head->prev;
        list->head->prev->next = new;
        list->head->prev = new;
        list->head = new;
        list->size++;
        return;
    }
    if (n == list->size) {
        new->next = list->head;
        new->prev = list->head->prev;
        list->head->prev->next = new;
        list->head->prev = new;
        list->size++;
        return;
    }
    while (n > pas) {
        curr = curr->next;
        pas++;
    }
    new->prev = curr;
    new->next = curr->next;
    curr->next->prev = new;
    curr->next = new;
    list->size++;
}


/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Functia intoarce un pointer spre acest nod
 * proaspat eliminat din lista. Daca n >= nr_noduri - 1, se elimina nodul de la
 * finalul listei. Daca n < 0, eroare. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
dll_node_t*
dll_remove_nth_node(doubly_linked_list_t* list, unsigned int n)
{
    DIE(list == NULL, "Lista nealocata!");
    unsigned int pas = 0;
    dll_node_t *kill;
    DIE(n < 0 || list->size == 0 , "EROARE");
    if (n == 0) {
        dll_node_t *swap = list->head;
        swap->prev->next = swap->next;
        swap->next->prev = swap->prev;
        list->head = list->head->next;
        list->size--;
        return swap;
    }
    if (n >= list->size) {
        n = list->size - 1;
    }
    kill = list->head;
    while (n > pas) {
        kill = kill->next;
        pas++;
    }
    kill->prev->next = kill->next;
    kill->next->prev = kill->prev;
    list->size--;
    return kill;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int
dll_get_size(doubly_linked_list_t* list)
{
    DIE(list == NULL, "Lista nealocata!");
    return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista.
 */
void
dll_free(doubly_linked_list_t** pp_list)
{
    int pas = 0;
    dll_node_t *kill, *next;
    kill = (*pp_list)->head;
    while (pas < (*pp_list)->size) {
        next = kill->next;
        free(kill->data);
        free(kill);
        kill = next;
        pas++;
    }
    free(*pp_list);
    *pp_list = NULL; 
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista separate printr-un spatiu, incepand de la primul nod din lista.
 */
void
dll_print_int_list(doubly_linked_list_t* list)
{
    int pas = 0;
    dll_node_t* print;
    print = list->head;
    while (pas < list->size) {
        printf("%d ", *((int*)print->data));
        print = print->next;
        pas++;
    }
    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista separate printr-un spatiu, incepand de la primul nod din
 * lista.
 */
void
dll_print_string_list(doubly_linked_list_t* list)
{
    int pas = 0;
    dll_node_t* print;
    print = list->head;
    while (pas < list->size) {
        printf("%s ", (char*)print->data);
        print = print->next;
        pas++;
    }
    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza o singura data toate valorile int
 * stocate in nodurile din lista, separate printr-un spatiu, incepand de la
 * nodul dat ca parametru si continuand la stanga in lista dublu inlantuita
 * circulara, pana cand sunt afisate valorile tuturor nodurilor.
 */
void
dll_print_ints_left_circular(dll_node_t* start)
{
    int pas = 0;
    dll_node_t* print;
    print = start->prev;
    printf("%d ", *((int*)print->data));
    while (print != start) {
        printf("%d ", *((int*)print->data));
        print = print->prev;
        pas++;
    }
    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza o singura data toate valorile int
 * stocate in nodurile din lista, separate printr-un spatiu, incepand de la
 * nodul dat ca parametru si continuand la dreapta in lista dublu inlantuita
 * circulara, pana cand sunt afisate valorile tuturor nodurilor.
 */
void
dll_print_ints_right_circular(dll_node_t* start)
{
    int pas = 0;
    dll_node_t* print;
    print = start->next;
    printf("%d ", *((int*)print->data));
    while (print != start) {
        printf("%d ", *((int*)print->data));
        print = print->next;
        pas++;
    }
    printf("\n");
}
