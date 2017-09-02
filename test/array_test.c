#include <stdio.h>
#include <string.h>
#include "connie_sugar.h"
#include "collections.h"


void test_1(connie_Connie *c);
void test_2(connie_Connie *c);
void test_3(connie_Connie *c);
void test_4(connie_Connie *c);
void test_filter(connie_Connie *c);


int main(void)
{
	connie_Connie *c;

	c = connie_new(__FILE__);

	TEST(c, test_1);
	TEST(c, test_2);
	TEST(c, test_3);
	TEST(c, test_4);
	TEST(c, test_filter);

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


typedef struct String {
	int id;
	const char *str;
} String;

static int initialize_string(unsigned int index, void *elem, void *user_data)
{
	String *str = (String *) elem;

	str->id = 0;
	str->str = NULL;

	return 0;
}

int match_string(unsigned int index, const void *elem, c_TypeUnion cond, void *user_data)
{
	const String *str = (const String *) elem;

	if (str->str == NULL) {
		return 0;
	}

	if (strcmp(cond.t_string, str->str) == 0) {
		return 1;
	}

	return 0;
}

void test_filter(connie_Connie *c)
{
	arr_Array *arr;
	const String strs[] = {
		{1, "foo"},
		{2, "bar"},
		{3, "baz"},
		{4, "foo"},
		{5, "baz"},
	};

	arr = arr_new(sizeof (String));
	A_NOT_NULL(c, arr);

	arr_set_initializer(arr, initialize_string);
	arr_autoext_on(arr, 64);

	{
		unsigned int strs_len = sizeof strs / sizeof (String);
		unsigned int i;

		for (i = 0; i < strs_len; i++) {
			arr_set(arr, i, &strs[i]);
		}
	}

	{
		arr_Filter filter;
		arr_Filter *f;
		const String *str;
		const char *cond = "foo";

		f = arr_set_filter(&filter, match_string, (c_TypeUnion) cond);
		A_NOT_NULL(c, f);

		str = (const String *) arr_next(f, arr);
		A_NOT_NULL(c, str);
		A_EQL_INT(c, 1, str->id);
		A_EQL_STRING(c, cond, str->str);

		str = (const String *) arr_next(f, arr);
		A_NOT_NULL(c, str);
		A_EQL_INT(c, 4, str->id);
		A_EQL_STRING(c, cond, str->str);

		str = (const String *) arr_next(f, arr);
		A_NULL(c, str);
	}

	{
		arr_Filter filter;
		arr_Filter *f;
		const String *str;
		const char *cond = "bar";

		f = arr_set_filter(&filter, match_string, (c_TypeUnion) cond);
		A_NOT_NULL(c, f);

		str = (const String *) arr_next(f, arr);
		A_NOT_NULL(c, str);
		A_EQL_INT(c, 2, str->id);
		A_EQL_STRING(c, cond, str->str);

		str = (const String *) arr_next(f, arr);
		A_NULL(c, str);
	}

	{
		arr_Filter filter;
		arr_Filter *f;
		const String *str;
		const char *cond = "baz";

		f = arr_set_filter(&filter, match_string, (c_TypeUnion) cond);
		A_NOT_NULL(c, f);

		str = (const String *) arr_next(f, arr);
		A_NOT_NULL(c, str);
		A_EQL_INT(c, 3, str->id);
		A_EQL_STRING(c, cond, str->str);

		str = (const String *) arr_next(f, arr);
		A_NOT_NULL(c, str);
		A_EQL_INT(c, 5, str->id);
		A_EQL_STRING(c, cond, str->str);

		str = (const String *) arr_next(f, arr);
		A_NULL(c, str);
	}

	arr_delete(arr);
}
