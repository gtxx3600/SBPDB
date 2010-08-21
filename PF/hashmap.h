/*
 * hashmap.h
 *
 *  Created on: 2010-8-21
 *      Author: cxy
 */

#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "unistd.h"
#include "assert.h"
/* You should always use 1024 */
#define     HASHMAP_SIZE    1024

/* Opaque struct pointer to _hash_map_t */
typedef struct    _hash_map_t*        hash_map;

typedef void(*pfcb_hmap_value_free)(void* value);

/* An example of free value function implemented by caller:
void my_hmap_free_value(void* pv)
{
    free(pv);
}
*/

/* Create before use. eg:
 * hash_map  hm;
 * hmap_create (&hm, HASHMAP_SIZE);
 * assert (hm);     // out of memory if hm==NULL
 * void* mydata=malloc(n);
 * hmap_insert(hm, "shanghai", -1, mydata);
   ...
 * hmap_destroy(hm, my_hmap_free_value);
 */
extern void
hmap_create(hash_map *hmap, int size);

/* Destroy after use */
extern void
hmap_destroy(hash_map hmap, pfcb_hmap_value_free);

/* Insert a key-value into hash map. value is a pointer to callee-allocated memory */
extern void
hmap_insert(hash_map hmap, const char* key, int key_len/* -1 for strlen to be called */, void* value);

/* Search a hash map for value of given key string */
extern void*
hmap_search(hash_map hmap, const char  *key);
extern void
hmap_delete(hash_map hmap, const char*key);

#endif /* HASHMAP_H_ */
