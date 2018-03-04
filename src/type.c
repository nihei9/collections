#include "collections/type.h"
#include <string.h>

size_t c_get_type_size(c_Type type)
{
	switch (type) {
	case c_TYPE_PTR:
	case c_TYPE_STRING:
		return sizeof (char *);
	case c_TYPE_CHAR:
	case c_TYPE_UCHAR:
		return sizeof (char);
	case c_TYPE_INT:
	case c_TYPE_UINT:
		return sizeof (int);
	case c_TYPE_LONG:
	case c_TYPE_ULONG:
		return sizeof (long);
	}

	return 0;
}

static int c_compare_char(void *key1, void *key2)
{
	char k1 = *((char *) key1);
	char k2 = *((char *) key2);

	return (k1 == k2)? c_EQ : (k1 > k2)? c_GT : c_LT;
}

static int c_compare_uchar(void *key1, void *key2)
{
	unsigned char k1 = *((unsigned char *) key1);
	unsigned char k2 = *((unsigned char *) key2);

	return (k1 == k2)? c_EQ : (k1 > k2)? c_GT : c_LT;
}

static int c_compare_int(void *key1, void *key2)
{
	int k1 = *((int *) key1);
	int k2 = *((int *) key2);

	return (k1 == k2)? c_EQ : (k1 > k2)? c_GT : c_LT;
}

static int c_compare_uint(void *key1, void *key2)
{
	unsigned int k1 = *((unsigned int *) key1);
	unsigned int k2 = *((unsigned int *) key2);
	
	return (k1 == k2)? c_EQ : (k1 > k2)? c_GT : c_LT;
}

static int c_compare_long(void *key1, void *key2)
{
	long k1 = *((long *) key1);
	long k2 = *((long *) key2);
	
	return (k1 == k2)? c_EQ : (k1 > k2)? c_GT : c_LT;
}

static int c_compare_ulong(void *key1, void *key2)
{
	unsigned long k1 = *((unsigned long *) key1);
	unsigned long k2 = *((unsigned long *) key2);
	
	return (k1 == k2)? c_EQ : (k1 > k2)? c_GT : c_LT;
}

static int c_compare_ptr(void *key1, void *key2)
{
	void *k1 = *((void **) key1);
	void *k2 = *((void **) key2);
	
	return (k1 == k2)? c_EQ : (k1 > k2)? c_GT : c_LT;
}

static int c_compare_string(void *key1, void *key2)
{
	const char *k1 = *((const char **) key1);
	const char *k2 = *((const char **) key2);
	int comp;
	
	comp = strcmp(k1, k2);
	return (comp == 0)? c_EQ : (comp > 0)? c_GT : c_LT;
}

c_Comparator c_get_comparator(c_Type type)
{
	switch (type) {
	case c_TYPE_CHAR:	return c_compare_char;
	case c_TYPE_UCHAR:	return c_compare_uchar;
	case c_TYPE_INT:	return c_compare_int;
	case c_TYPE_UINT:	return c_compare_uint;
	case c_TYPE_LONG:	return c_compare_long;
	case c_TYPE_ULONG:	return c_compare_ulong;
	case c_TYPE_PTR:	return c_compare_ptr;
	case c_TYPE_STRING:	return c_compare_string;
	}

	return NULL;
}

unsigned int c_hash_ptr(void *key)
{
	const void **k = (const void **) key;

	return (unsigned int) ((unsigned long) *k);
}

unsigned int c_hash_char(void *key)
{
	char *k = (char *) key;

	if (*k < 0) {
		return (unsigned int) *k * -1;
	}
	
	return (unsigned int) *k;
}

unsigned int c_hash_uchar(void *key)
{
	unsigned char *k = (unsigned char *) key;
	
	return (unsigned int) *k;
}

unsigned int c_hash_int(void *key)
{
	int *k = (int *) key;

	if (*k < 0) {
		return (unsigned int) *k * -1;
	}
	
	return (unsigned int) *k;
}

unsigned int c_hash_uint(void *key)
{
	unsigned int *k = (unsigned int *) key;
	
	return *k;
}

unsigned int c_hash_long(void *key)
{
	long *k = (long *) key;
	
	return (unsigned int) *k;
}

unsigned int c_hash_ulong(void *key)
{
	unsigned long *k = (unsigned long *) key;
	
	return (unsigned int) *k;
}

unsigned int c_hash_string(void *key)
{
	const char *str = *((const char **) key);
	
	return (unsigned int) str[0];
}

c_Hasher c_get_hasher(c_Type type)
{
	switch (type) {
	case c_TYPE_PTR:	return c_hash_ptr;
	case c_TYPE_STRING:	return c_hash_string;
	case c_TYPE_CHAR:	return c_hash_char;
	case c_TYPE_UCHAR:	return c_hash_uchar;
	case c_TYPE_INT:	return c_hash_int;
	case c_TYPE_UINT:	return c_hash_uint;
	case c_TYPE_LONG:	return c_hash_long;
	case c_TYPE_ULONG:	return c_hash_ulong;
	}

	return NULL;
}

void c_assign_from_union(c_Type type, const c_TypeUnion *rhs, void *lhs)
{
	switch (type) {
	case c_TYPE_PTR:	*((void **) lhs) = rhs->t_ptr;				break;
	case c_TYPE_STRING:	*((const char **) lhs) = rhs->t_string;		break;
	case c_TYPE_CHAR:	*((char *) lhs) = rhs->t_char;				break;
	case c_TYPE_UCHAR:	*((unsigned char *) lhs) = rhs->t_uchar;	break;
	case c_TYPE_INT:	*((int *) lhs) = rhs->t_int;				break;
	case c_TYPE_UINT:	*((unsigned int *) lhs) = rhs->t_uint;		break;
	case c_TYPE_LONG:	*((long *) lhs) = rhs->t_long;				break;
	case c_TYPE_ULONG:	*((unsigned long *) lhs) = rhs->t_ulong;	break;
	}
}

void c_assign_to_union(c_Type type, const void *rhs, c_TypeUnion *lhs)
{
	switch (type) {
	case c_TYPE_PTR:	lhs->t_ptr = *((void **) rhs);					break;
	case c_TYPE_STRING:	lhs->t_string = *((const char **) rhs);			break;
	case c_TYPE_CHAR:	lhs->t_char = *((const char *) rhs);			break;
	case c_TYPE_UCHAR:	lhs->t_uchar = *((const unsigned char *) rhs);	break;
	case c_TYPE_INT:	lhs->t_int = *((const int *) rhs);				break;
	case c_TYPE_UINT:	lhs->t_uint = *((const unsigned int *) rhs);	break;
	case c_TYPE_LONG:	lhs->t_long = *((const long *) rhs);			break;
	case c_TYPE_ULONG:	lhs->t_ulong = *((const unsigned long *) rhs);	break;
	}
}
