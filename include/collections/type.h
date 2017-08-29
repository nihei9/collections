#ifndef c_COLLECTIONS_TYPE_H
#define c_COLLECTIONS_TYPE_H

#include <stdlib.h>

typedef enum c_Type {
	c_TYPE_PTR,		// void *
	c_TYPE_STRING,	// const char *
	c_TYPE_CHAR,	// char
	c_TYPE_UCHAR,	// unsigned char
	c_TYPE_INT,		// int
	c_TYPE_UINT,	// unsigned int
	c_TYPE_LONG,	// long
	c_TYPE_ULONG,	// unsigned long
} c_Type;

typedef union c_TypeUnion {
	void			*t_ptr;
	const char		*t_string;
	char			t_char;
	unsigned char	t_uchar;
	int				t_int;
	unsigned int	t_uint;
	long			t_long;
	unsigned long	t_ulong;
} c_TypeUnion;

typedef int (*c_Comparator)(void *key1, void *key2);
typedef unsigned int (*c_Hasher)(void *key);

#define c_EQ	0	// =
#define c_GT	1	// >
#define c_LT	-1	// <

size_t c_get_type_size(c_Type type);
c_Comparator c_get_comparator(c_Type type);
c_Hasher c_get_hasher(c_Type type);
void c_assign_from_union(c_Type type, const c_TypeUnion *rhs, void *lhs);
void c_assign_to_union(c_Type type, const void *rhs, c_TypeUnion *lhs);

#endif
