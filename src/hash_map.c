#include "collections/hash_map.h"
#include "collections/type.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct hmap_Entry {
	c_TypeUnion key;
	c_TypeUnion value;

	struct hmap_Entry *next;
} hmap_Entry;


typedef struct hmap_Bucket {
	hmap_Entry *head;
} hmap_Bucket;


#define BUCKETS_LEN 11


struct hmap_HashMap {
	c_Type key_type;
	c_Type value_type;
	
	hmap_Bucket *buckets;

	struct {
		c_Hasher hasher;
		c_Comparator comparator;
	} callbacks;
};


static unsigned int hmap_hash(hmap_HashMap *hmap, void *key);
static hmap_Entry *hmap_lookup_entry(hmap_HashMap *hmap, void *key, unsigned int *hval);


hmap_HashMap *hmap_new(c_Type key_type, c_Type value_type)
{
	hmap_HashMap *hmap = NULL;
	hmap_Bucket *buckets = NULL;
	c_Hasher hasher;
	c_Comparator comparator;
	int ret;
	
	hmap = (hmap_HashMap *) malloc(sizeof (hmap_HashMap));
	buckets = (hmap_Bucket *) calloc(BUCKETS_LEN, sizeof (hmap_Bucket));
	if (hmap == NULL || buckets == NULL) {
		goto FAILURE;
	}

	hasher = c_get_hasher(key_type);
	if (hasher == NULL) {
		goto FAILURE;
	}
	comparator = c_get_comparator(key_type);
	if (comparator == NULL) {
		goto FAILURE;
	}

	hmap->key_type = key_type;
	hmap->value_type = value_type;
	hmap->buckets = buckets;
	hmap->callbacks.hasher = hasher;
	hmap->callbacks.comparator = comparator;

	return hmap;

FAILURE:
	free(hmap);
	hmap = NULL;
	free(buckets);
	buckets = NULL;

	return NULL;
}


void hmap_delete(hmap_HashMap *hmap)
{
	unsigned int i;
	
	if (hmap == NULL) {
		return;
	}

	for (i = 0; i < BUCKETS_LEN; i++) {
		hmap_Entry *entry = hmap->buckets[i].head;
		hmap_Entry *next;

		while (entry != NULL) {
			next = entry->next;
			free(entry);
			entry = next;
		}

		hmap->buckets[i].head = NULL;
	}
	free(hmap->buckets);
	hmap->buckets = NULL;
	free(hmap);
}


void *hmap_put(hmap_HashMap *hmap, void *key, void *value)
{
	unsigned int hval;
	hmap_Entry *entry;
	hmap_Bucket *bucket;
	
	if (hmap == NULL || key == NULL || value == NULL) {
		return NULL;
	}
	
	entry = hmap_lookup_entry(hmap, key, &hval);
	if (entry != NULL) {
		// TODO Applys finalizer to an entry.

		entry->key.t_ptr = key;
		entry->value.t_ptr = value;
		
		return value;
	}
	
	entry = (hmap_Entry *) malloc(sizeof (hmap_Entry));
	if (entry == NULL) {
		return NULL;
	}
	
	c_assign_to_union(hmap->key_type, key, &entry->key);
	c_assign_to_union(hmap->value_type, value, &entry->value);
	entry->next = NULL;
	
	bucket = &hmap->buckets[hval];
	entry->next = bucket->head;
	bucket->head = entry;
	
	return value;
}


void *hmap_lookup(hmap_HashMap *hmap, void *key)
{
	hmap_Entry *entry;
	int ret;
	
	if (hmap == NULL || key == NULL) {
		return NULL;
	}

	entry = hmap_lookup_entry(hmap, key, NULL);
	if (entry == NULL) {
		return NULL;
	}

	return &entry->value;
}


static unsigned int hmap_hash(hmap_HashMap *hmap, void *key)
{	
	return (*hmap->callbacks.hasher)(key) % BUCKETS_LEN;
}


static hmap_Entry *hmap_lookup_entry(hmap_HashMap *hmap, void *key, unsigned int *hval)
{
	unsigned int _hval;
	hmap_Bucket *bucket;
	hmap_Entry *entry;
	
	_hval = hmap_hash(hmap, key);
	if (hval != NULL) {
		*hval = _hval;
	}
	
	bucket = &hmap->buckets[_hval];
	for (entry = bucket->head; entry != NULL; entry = entry->next) {
		int result;

		result = (*hmap->callbacks.comparator)(key, &entry->key);
		if (result == 0) {
			return entry;
		}
	}
	
	return NULL;
}
