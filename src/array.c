#include <stdlib.h>
#include <string.h>
#include "collections/array.h"


struct arr_Array {
	void *body;
	size_t elem_size;
	unsigned int len;
	
	void *user_data;

	struct {
		int flag;
		unsigned int step_len;
	} autoext;
	
	struct {
		arr_Initializer initializer;
		arr_Finalizer finalizer;
		arr_Comparator comparator;
	} callbacks;
};


#define arr_AUTOEXT_OFF 0
#define arr_AUTOEXT_ON 1


/*
 * arr_Arrayから指定のインデックスの要素を取得する。
 *
 * PARAMETER
 * arr 取得元のarr_Arrayへのポインタ
 * index 取得する要素のインデックス
 * elem 取得した要素へのポインタの格納先
 */
static void *arr_elem(arr_Array *arr, unsigned int index)
{
	if (index >= arr->len) {
		unsigned int numof_step;
		unsigned int shortage;
		unsigned int size;
		
		if (arr->autoext.flag != arr_AUTOEXT_ON) {
			return NULL;
		}
		
		shortage = index - arr->len + 1;
		numof_step = (shortage / arr->autoext.step_len) + ((shortage % arr->autoext.step_len == 0)? 0 : 1);
		size = arr_extend(arr, numof_step * arr->autoext.step_len);
		if (size <= 0) {
			return NULL;
		}
	}

	// arr->bodyはvoid*で、これに対するアドレス演算はGCCを除きエラーとなるようなので、
	// 一時的にchar*の変数へ代入して使用する。
	// （GCCでも警告が出る）
	return (void *) &((char *) arr->body)[arr->elem_size * index];
}


arr_Array *arr_new(size_t elem_size)
{
	arr_Array *arr;
	
	if (elem_size <= 0) {
		return NULL;
	}
	
	arr = (arr_Array *) malloc(sizeof (arr_Array));
	if (arr == NULL) {
		return NULL;
	}
	arr->body = NULL;
	arr->elem_size = elem_size;
	arr->len = 0;
	arr->autoext.flag = arr_AUTOEXT_OFF;
	arr->autoext.step_len = 0;
	arr->user_data = NULL;
	arr->callbacks.initializer = NULL;
	arr->callbacks.finalizer = NULL;
	arr->callbacks.comparator = NULL;
	
	return arr;
}


void arr_delete(arr_Array *arr)
{
	if (arr == NULL) {
		return;
	}

	if (arr->callbacks.finalizer != NULL) {
		arr_Finalizer finalizer = arr->callbacks.finalizer;
		unsigned int i;
		
		for (i = 0; i < arr->len; i++) {
			char *body = arr->body;
			(*finalizer)(i, (void *) &body[arr->elem_size * i], arr->user_data);
		}
	}
	free(arr->body);
	arr->body = NULL;
	free(arr);
}


size_t arr_get_elem_size(const arr_Array *arr)
{
	if (arr == NULL) {
		return 0;
	}
	
	return arr->elem_size;
}


unsigned int arr_get_len(const arr_Array *arr)
{
	if (arr == NULL) {
		return 0;
	}
	
	return arr->len;
}


unsigned int arr_autoext_on(arr_Array *arr, unsigned int step_size)
{
	if (arr == NULL || step_size <= 0) {
		return 0;
	}
	
	arr->autoext.flag = arr_AUTOEXT_ON;
	arr->autoext.step_len = step_size;
	
	return step_size;
}


unsigned int arr_autoext_off(arr_Array *arr)
{
	unsigned int step_len;

	if (arr == NULL) {
		return 0;
	}

	step_len = arr->autoext.step_len;
	
	arr->autoext.flag = arr_AUTOEXT_OFF;
	arr->autoext.step_len = 0;

	return step_len;
}


arr_Initializer arr_set_initializer(arr_Array *arr, arr_Initializer initializer)
{
	arr_Initializer old_initializer;

	if (arr == NULL) {
		return NULL;
	}

	old_initializer = arr->callbacks.initializer;
	
	arr->callbacks.initializer = initializer;
	
	return old_initializer;
}


arr_Finalizer arr_set_finalizer(arr_Array *arr, arr_Finalizer finalizer)
{
	arr_Finalizer old_finalizer;

	if (arr == NULL) {
		return NULL;
	}

	old_finalizer = arr->callbacks.finalizer;
	
	arr->callbacks.finalizer = finalizer;
	
	return old_finalizer;
}


arr_Comparator arr_set_comparator(arr_Array *arr, arr_Comparator comparator)
{
	arr_Comparator old_comparator;

	if (arr == NULL) {
		return NULL;
	}

	old_comparator = arr->callbacks.comparator;
	
	arr->callbacks.comparator = comparator;
	
	return old_comparator;
}


void *arr_set_user_data(arr_Array *arr, void *user_data)
{
	void *old_user_data;

	if (arr == NULL) {
		return NULL;
	}

	old_user_data = arr->user_data;
	
	arr->user_data = user_data;
	
	return old_user_data;
}


unsigned int arr_extend(arr_Array *arr, unsigned int ext_len)
{
	void *new_body;
	unsigned long new_len;
	
	if (arr == NULL || ext_len <= 0) {
		return 0;
	}
	
	new_len = arr->len + ext_len;
	if (arr->body == NULL) {
		new_body = malloc(arr->elem_size * new_len);
	}
	else {
		new_body = realloc(arr->body, arr->elem_size * new_len);
	}
	if (new_body == NULL) {
		return 0;
	}
	if (arr->callbacks.initializer != NULL) {
		unsigned int i;
		
		for (i = arr->len; i < new_len; i++) {
			char *body = new_body;
			(*arr->callbacks.initializer)(i, (void *) &body[arr->elem_size * i], arr->user_data);
		}
	}
	arr->body = new_body;
	arr->len = new_len;
	
	return arr->len;
}


void *arr_get(arr_Array *arr, unsigned int index)
{
	if (arr == NULL) {
		return NULL;
	}
	
	return arr_elem(arr, index);
}


void *arr_set(arr_Array *arr, unsigned int index, const void *elem)
{
	void *e;
	
	if (arr == NULL || elem == NULL) {
		return NULL;
	}
	
	e = arr_elem(arr, index);
	if (e == NULL) {
		return NULL;
	}
	memcpy(e, elem, arr->elem_size);
	
	return e;
}


arr_Array *arr_cleanup(arr_Array *arr)
{
	if (arr == NULL) {
		return NULL;
	}
	
	if (arr->callbacks.initializer != NULL) {
		arr_Initializer initializer = arr->callbacks.initializer;
		unsigned long i;
		char *body = (char *) arr->body;
		
		for (i = 0; i < arr->len; i++) {
			(*initializer)(i, &body[i * arr->elem_size], arr->user_data);
		}
	} else {
		memset(arr->body, 0, arr->elem_size * arr->len);
	}
	
	return arr;
}


c_Bool arr_contain(arr_Array *arr, void *elem)
{
	if (arr == NULL || elem == NULL) {
		return c_BOOL_FALSE;
	}
	
	if (arr->callbacks.comparator != NULL){
		char *body = arr->body;
		arr_Comparator comparator = arr->callbacks.comparator;
		unsigned long i;
		int result;
		
		for (i = 0; i < arr->len; i++) {
			(*comparator)(elem, &body[i * arr->elem_size], arr->user_data, &result);
			if (result == 0) {
				return c_BOOL_TRUE;
			}
		}
	} else {
		char *body = arr->body;
		unsigned long i;
		int ret;
		
		for (i = 0; i < arr->len; i++) {
			ret = memcmp(elem, &body[i * arr->elem_size], arr->elem_size);
			if (ret == 0) {
				return c_BOOL_TRUE;
			}
		}
	}
	
	return c_BOOL_FALSE;
}


int arr_map(arr_Array *arr, arr_Callback f)
{
	char *body;
	unsigned long i;
	
	if (arr == NULL || f == NULL) {
		return 1;
	}

	body = arr->body;
	for (i = 0; i < arr->len; i++) {
		(*f)(i, &body[arr->elem_size * i], arr->user_data);
	}
	
	return 0;
}


arr_Filter *arr_set_filter(arr_Filter *filter, arr_Matcher matcher, c_TypeUnion cond)
{
	if (filter == NULL || matcher == NULL) {
		return NULL;
	}

	filter->matcher = matcher;
	filter->index = 0;
	filter->cond = cond;

	return filter;
}


void *arr_next(arr_Filter *filter, const arr_Array *arr)
{
	arr_Matcher m;
	unsigned int i;

	if (filter == NULL || arr == NULL) {
		return NULL;
	}

	m = filter->matcher;
	for (i = filter->index; i < arr->len; i++) {
		void *v = arr_get((arr_Array *) arr, i);
		if (v == NULL) {
			return NULL;
		}
		if ((*m)(i, v, filter->cond, arr->user_data) == 1) {
			filter->index = i + 1;

			return v;
		}
	}

	return NULL;
}
