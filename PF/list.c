/*
 * list.c
 *
 *  Created on: 2010-8-21
 *      Author: cxy
 */
#include "list.h"
#include "string.h"
void list_append_node(list_t *in_list, listnode_t *node) {
	node->next = NULL;

	if (in_list->head) {
		in_list->tail->next = node;
		in_list->tail = node;
	} else
		in_list->head = in_list->tail = node;

	in_list->size++;
}

/* Removes the first node from a list and returns it */
listnode_t*
list_remove_head(list_t *in_list) {
	listnode_t *node = NULL;
	if (in_list->head) {
		node = in_list->head;
		in_list->head = in_list->head->next;
		if (in_list->head == NULL)
			in_list->tail = NULL;
		node->next = NULL;

		in_list->size--;
	}
	return node;
}

/* Removes all nodes but for list itself */
void list_remove_all(list_t *in_list, pfcb_list_node_free pf) {
	listnode_t *node;
	while ((node = list_remove_head(in_list))) {
		if (pf)
			(*pf)(node);
		free(node);
	}
}

/* Returns a copy of a list_t from heap */
list_t*
list_copy(list_t list) {
	list_t *newlist = (list_t*) malloc(sizeof(list_t));
	*newlist = list;
	return newlist;
}

/* Concatenates two lists into first list */
void list_concat(list_t *first, list_t *second) {
	if (first->head) {
		if (second->head) {
			first->tail->next = second->head;
			first->tail = second->tail;
		}
	} else
		*first = *second;
	second->head = second->tail = NULL;

	first->size += second->size;
}

/* Allocates a new listnode_t from heap */
listnode_t*
list_node_create(void* data) {
	listnode_t *node = (listnode_t*) malloc(sizeof(listnode_t));
	node->next = NULL;
	node->data = data;
	return node;
}

listnode_t*
list_key_create(long key) {
	listnode_t *node = (listnode_t*) malloc(sizeof(listnode_t));
	node->next = NULL;
	node->key = key;
	return node;
}

/* Allocates a empty list_t from heap */
list_t*
list_create() {
	list_t *list = (list_t*) malloc(sizeof(list_t));
	list->size = 0;
	list->head = list->tail = NULL;
	return list;
}

/* Frees a empty list_t from heap */
void list_destroy(list_t *in_list, pfcb_list_node_free pf) {
	list_remove_all(in_list, pf);
	free(in_list);
}

/* Gets count of nodes in the list */
size_t list_size(const list_t* in_list) {
	return in_list->size;
}

/* Gets node by index 0-based. 0 is head */
listnode_t*
list_node_at(const list_t* in_list, int index) {
	int i = 0;
	listnode_t *node = in_list->head;


	while (i < index) {
		node = node->next;
		i++;
	}

	return node;
}
