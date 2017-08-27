#include "collections/string_store.h"
#include "collections/hash_map.h"
#include <stdlib.h>
#include <string.h>


struct strs_StringStore {
	hmap_HashMap *str2id_map;
	hmap_HashMap *id2str_map;
	strs_ID id;

	strs_ID ret_id;
};


#define INITIAL_ID 0


strs_StringStore *strs_new(void)
{
	strs_StringStore *strs = NULL;
	hmap_HashMap *str2id_map = NULL;
	hmap_HashMap *id2str_map = NULL;

	strs = (strs_StringStore *) malloc(sizeof (strs_StringStore));
	if (strs == NULL) {
		goto FAILURE;
	}

	str2id_map = hmap_new(c_TYPE_STRING, c_TYPE_UINT);
	if (str2id_map == NULL) {
		goto FAILURE;
	}
	id2str_map = hmap_new(c_TYPE_UINT, c_TYPE_STRING);
	if (id2str_map == NULL) {
		goto FAILURE;
	}
	strs->str2id_map = str2id_map;
	strs->id2str_map = id2str_map;
	strs->id = INITIAL_ID;
	strs->ret_id = INITIAL_ID;

	return strs;

FAILURE:
	free(strs);
	strs = NULL;
	hmap_delete(str2id_map);
	str2id_map = NULL;
	hmap_delete(id2str_map);
	id2str_map = NULL;

	return NULL;
}


void strs_delete(strs_StringStore *strs)
{
	if (strs == NULL) {
		return;
	}

	hmap_delete(strs->str2id_map);
	strs->str2id_map = NULL;
	hmap_delete(strs->id2str_map);
	strs->id2str_map = NULL;
	free(strs);
}


const strs_ID *strs_put(strs_StringStore *strs, const char *str)
{
	const strs_ID *id;

	if (strs == NULL || str == NULL) {
		return NULL;
	}

	id = (const strs_ID *) hmap_lookup(strs->str2id_map, &str);
	if (id != NULL) {
		strs->ret_id = *id;

		return &strs->ret_id;
	}

	if (hmap_put(strs->str2id_map, &str, &strs->id) == NULL) {
		return NULL;
	}
	if (hmap_put(strs->id2str_map, &strs->id, &str) == NULL) {
		return NULL;
	}
	
	strs->ret_id = strs->id++;

	return &strs->ret_id;
}


const char *strs_lookup(strs_StringStore *strs, strs_ID id)
{
	void *v;

	if (strs == NULL) {
		return NULL;
	}

	v = hmap_lookup(strs->id2str_map, &id);
	if (v == NULL) {
		return NULL;
	}

	return *((const char **) v);
}
