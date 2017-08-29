#include <string.h>
#include "connie_sugar.h"
#include "collections/hash_map.h"


void test_1(connie_Connie *c);


int main(void)
{
	connie_Connie *c;

	c = connie_new(__FILE__);

	TEST(c, test_1);

	connie_print(c);

	connie_delete(c);

    return 0;
}


void test_1(connie_Connie *c)
{
	hmap_HashMap *hmap;
	unsigned int key;
	const char *value_in = "foo";
	const char **value_out;

	hmap = hmap_new(c_TYPE_UINT, c_TYPE_STRING);
	A_NOT_NULL(c, hmap);

	key = 1;
	value_out = hmap_put(hmap, &key, &value_in);
	A_EQL_STRING(c, value_in, *value_out);

	value_out = hmap_lookup(hmap, &key);
	A_EQL_STRING(c, value_in, *value_out);

	key = 2;
	value_out = hmap_lookup(hmap, &key);
	A_NULL(c, value_out);

	hmap_delete(hmap);
}
