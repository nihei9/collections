#ifndef hmap_COLLECTIONS_HASH_MAP_H
#define hmap_COLLECTIONS_HASH_MAP_H

#include "type.h"
#include <stdlib.h>


typedef struct hmap_HashMap hmap_HashMap;


hmap_HashMap *hmap_new(c_Type key_type, c_Type value_type);
void hmap_delete(hmap_HashMap *hmap);
void *hmap_put(hmap_HashMap *hmap, void *key, void *value);
void *hmap_lookup(hmap_HashMap *hmap, void *key);

#endif
