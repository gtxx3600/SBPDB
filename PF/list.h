/*
 * list.h
 *
 *  Created on: 2010-8-21
 *      Author: cxy
 */

#ifndef LIST_H_
#define LIST_H_

#include "stdlib.h"
#include "unistd.h"

typedef struct _listnode_t
{
    struct _listnode_t    *next;
    union{
        void*            data;
        struct _list_t    *list;
        const char        *str;
        long            key;
    };
}listnode_t;

typedef struct _list_t
{
    size_t        size;    /* count of nodes */
    listnode_t    *head;
    listnode_t  *tail;
}list_t, *list_p;

/* A prototype of callbacked function called by list_destroy(), NULL for no use. */
typedef void(*pfcb_list_node_free)(listnode_t* node);

/* An example of free node data function implemented by callee:
void my_list_node_free(listnode_t *node)
{
    free(node->data);
}
*/

/* Appends a node to a list */
extern void
list_append_node(list_t *in_list, listnode_t *in_node);

/* Removes the first node from a list and returns it */
extern listnode_t*
list_remove_head(list_t *in_list);

/* Removes all nodes but for list itself */
extern void
list_remove_all(list_t *in_list, pfcb_list_node_free pfunc /* NULL for no use or a key node */);

/* Returns a copy of a list_t from heap */
extern list_t*
list_copy(list_t in_list);

/* Concatenates two lists into first list. NOT freeing the second */
extern void
list_concat(list_t *first, list_t *second);

/* Allocates a new listnode_t from heap. NO memory allocated for input node_data */
extern listnode_t*
list_node_create(void* node_data);

/* Allocates a new listnode_t with a key node type */
extern listnode_t*
list_key_create(long node_key);

/* Allocates a empty list_t from heap */
extern list_t*
list_create();

/* Frees in_list's all nodes and destroys in_list from heap.
 * the callee is responsible for freeing node data.
 * the node freed-function(pfunc) is called by list_destroy.
 */
extern void
list_destroy(list_t *in_list, pfcb_list_node_free pfunc /* NULL for no use or a key node */);

/* Gets count of nodes in the list */
extern size_t
list_size(const list_t* in_list);

/* Gets node by index 0-based. 0 is head */
extern listnode_t*
list_node_at(const list_t* in_list, int index);

#endif /* LIST_H_ */
