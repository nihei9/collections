#ifndef arr_COLLECTIONS_ARRAY_H
#define arr_COLLECTIONS_ARRAY_H

#include "type.h"
#include "bool.h"
#include <stdio.h>

typedef struct arr_Array arr_Array;

typedef int (*arr_Initializer)(unsigned int index, void *elem, void *user_data);
typedef int (*arr_Finalizer)(unsigned int index, void *elem, void *user_data);
typedef int (*arr_Comparator)(const void *elem_1, const void *elem_2, void *user_data, int *result);
typedef int (*arr_Callback)(unsigned int index, const void *elem, void *user_data);

arr_Array *arr_new(size_t elem_size);
void arr_delete(arr_Array *arr);
size_t arr_get_elem_size(const arr_Array *arr);
unsigned int arr_get_len(const arr_Array *arr);
unsigned int arr_autoext_on(arr_Array *arr, unsigned int step_size);
unsigned int arr_autoext_off(arr_Array *arr);
arr_Initializer arr_set_initializer(arr_Array *arr, arr_Initializer initializer);
arr_Finalizer arr_set_finalizer(arr_Array *arr, arr_Finalizer finalizer);
arr_Comparator arr_set_comparator(arr_Array *arr, arr_Comparator comparator);
void *arr_set_user_data(arr_Array *arr, void *user_data);
unsigned int arr_extend(arr_Array *arr, unsigned int ext_len);
void *arr_get(arr_Array *arr, unsigned int index);
void *arr_set(arr_Array *arr, unsigned int index, void *elem);
arr_Array *arr_cleanup(arr_Array *arr);
c_Bool arr_contain(arr_Array *arr, void *elem);
int arr_map(arr_Array *arr, arr_Callback f);

typedef int (*arr_Matcher)(unsigned int index, const void *elem, c_TypeUnion cond, void *user_data);

typedef struct arr_Filter {
    arr_Matcher matcher;
    c_TypeUnion cond;
    unsigned int index;
} arr_Filter;

arr_Filter *arr_set_filter(arr_Filter *filter, arr_Matcher matcher, c_TypeUnion cond);
void *arr_next(arr_Filter *filter, const arr_Array *arr);

#endif
