#ifndef strs_COLLECTIONS_STRING_STORE_H
#define strs_COLLECTIONS_STRING_STORE_H

#include <stdio.h>

typedef unsigned long strs_ID;

typedef struct strs_StringStore strs_StringStore;

strs_StringStore *strs_new(void);
void strs_delete(strs_StringStore *strs);
const strs_ID *strs_put(strs_StringStore *strs, const char *str);
const char *strs_lookup(strs_StringStore *strs, strs_ID id);

#endif
