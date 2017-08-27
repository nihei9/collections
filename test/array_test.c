#include <stdio.h>
#include <string.h>
#include "connie_sugar.h"
#include "collections.h"


void test_1(connie_Connie *c);
void test_2(connie_Connie *c);
void test_3(connie_Connie *c);
void test_4(connie_Connie *c);


int main(void)
{
	connie_Connie *c;

	c = connie_new(__FILE__);

	TEST(c, test_1);
	TEST(c, test_2);
	TEST(c, test_3);
	TEST(c, test_4);

	connie_print(c);

	connie_delete(c);

	return 0;
}


/*
 * 非ポインタのプリミティブ型を要素として扱うケース
 */
void test_1(connie_Connie *c)
{
	arr_Array *arr;
	const int values[] = {
		0,
		11,
		222,
		3333,
		44444,
	};
	unsigned int size;
	int *v;

	arr = arr_new(sizeof (int));
	A_NOT_NULL(c, arr);
	size = arr_extend(arr, sizeof values / sizeof (int));
	A_EQL_UINT(c, sizeof values / sizeof (int), size);

	v = (int *) arr_set(arr, 0, (void *) &values[0]);
	A_NOT_NULL(c, v);

	v = (int *) arr_set(arr, 1, (void *) &values[1]);
	A_NOT_NULL(c, v);

	v = (int *) arr_set(arr, 2, (void *) &values[2]);
	A_NOT_NULL(c, v);

	v = (int *) arr_set(arr, 3, (void *) &values[3]);
	A_NOT_NULL(c, v);

	v = (int *) arr_set(arr, 4, (void *) &values[4]);
	A_NOT_NULL(c, v);

	v = (int *) arr_get(arr, 0);
	A_EQL_INT(c, values[0], *v);

	v = (int *) arr_get(arr, 1);
	A_EQL_INT(c, values[1], *v);

	v = (int *) arr_get(arr, 2);
	A_EQL_INT(c, values[2], *v);

	v = (int *) arr_get(arr, 3);
	A_EQL_INT(c, values[3], *v);

	v = (int *) arr_get(arr, 4);
	A_EQL_INT(c, values[4], *v);

	arr_delete(arr);
}


/*
 * ポインタを要素として扱うケース
 */
void test_2(connie_Connie *c)
{
	arr_Array *arr;
	const char *values[] = {
		"Kuwata",
		"Hara",
		"Matsuda",
		"Sekiguchi",
		"Nozawa",
	};
	const char **v;
	unsigned int size;

	arr = arr_new(sizeof (char *));
	A_NOT_NULL(c, arr);
	size = arr_extend(arr, sizeof values / sizeof (char *));
	A_EQL_UINT(c, sizeof values / sizeof (char *), size);

	v = (const char **) arr_set(arr, 0, (void *) &values[0]);
	A_NOT_NULL(c, v);
	A_NOT_NULL(c, *v);

	v = (const char **) arr_set(arr, 1, (void *) &values[1]);
	A_NOT_NULL(c, v);
	A_NOT_NULL(c, *v);

	v = (const char **) arr_set(arr, 2, (void *) &values[2]);
	A_NOT_NULL(c, v);
	A_NOT_NULL(c, *v);

	v = (const char **) arr_set(arr, 3, (void *) &values[3]);
	A_NOT_NULL(c, v);
	A_NOT_NULL(c, *v);

	v = (const char **) arr_set(arr, 4, (void *) &values[4]);
	A_NOT_NULL(c, v);
	A_NOT_NULL(c, *v);

	v = (const char **) arr_get(arr, 0);
	A_EQL_STRING(c, values[0], *v);

	v = (const char **) arr_get(arr, 1);
	A_EQL_STRING(c, values[1], *v);

	v = (const char **) arr_get(arr, 2);
	A_EQL_STRING(c, values[2], *v);

	v = (const char **) arr_get(arr, 3);
	A_EQL_STRING(c, values[3], *v);

	v = (const char **) arr_get(arr, 4);
	A_EQL_STRING(c, values[4], *v);

	arr_delete(arr);
}


/*
 * arr_setにarr_ArrayとしてNULLを渡すケース
 */
void test_3(connie_Connie *c)
{
	const int values[] = {
		0,
	};
	void *v;

	v = arr_set(NULL, 0, (void *) &values[0]);
	A_NULL(c, v);
}


/*
 * arr_extendせずarr_setするケース
 */
void test_4(connie_Connie *c)
{
	arr_Array *arr;
	const int values[] = {
		0,
	};
	void *v;

	arr = arr_new(sizeof (int));
	A_NOT_NULL(c, arr);

	v = arr_set(arr, 0, (void *) &values[0]);
	A_NULL(c, v);

	arr_delete(arr);
}
