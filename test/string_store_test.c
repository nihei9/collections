#include "collections/string_store.h"
#include "connie_sugar.h"


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

void put_and_lookup(connie_Connie *c, strs_StringStore *strs, const char *str)
{
	const strs_ID *id;
	const char *s;

	id = strs_put(strs, str);
	A_NOT_NULL(c, id);

	s = strs_lookup(strs, *id);
	A_NOT_NULL(c, s);
	A_EQL_STRING(c, str, s);
}

void test_1(connie_Connie *c)
{
	strs_StringStore *strs;

	strs = strs_new();
	A_NOT_NULL(c, strs);

	put_and_lookup(c, strs, "a");
	put_and_lookup(c, strs, "bb");
	put_and_lookup(c, strs, "ccc");
	put_and_lookup(c, strs, "dddd");
	put_and_lookup(c, strs, "eeeee");
	put_and_lookup(c, strs, "ffffff");
	put_and_lookup(c, strs, "ggggggg");
	put_and_lookup(c, strs, "hhhhhhhh");
	put_and_lookup(c, strs, "iiiiiiiii");
	put_and_lookup(c, strs, "jjjjjjjjjj");
	put_and_lookup(c, strs, "kkkkkkkkkkk");
	put_and_lookup(c, strs, "llllllllllll");
	put_and_lookup(c, strs, "mmmmmmmmmmmmm");
	put_and_lookup(c, strs, "nnnnnnnnnnnnnn");
	put_and_lookup(c, strs, "ooooooooooooooo");
	put_and_lookup(c, strs, "pppppppppppppppp");
	put_and_lookup(c, strs, "qqqqqqqqqqqqqqqqq");
	put_and_lookup(c, strs, "rrrrrrrrrrrrrrrrrr");
	put_and_lookup(c, strs, "sssssssssssssssssss");
	put_and_lookup(c, strs, "tttttttttttttttttttt");
	put_and_lookup(c, strs, "uuuuuuuuuuuuuuuuuuuuu");
	put_and_lookup(c, strs, "vvvvvvvvvvvvvvvvvvvvvv");
	put_and_lookup(c, strs, "wwwwwwwwwwwwwwwwwwwwwww");
	put_and_lookup(c, strs, "xxxxxxxxxxxxxxxxxxxxxxxx");
	put_and_lookup(c, strs, "yyyyyyyyyyyyyyyyyyyyyyyyy");
	put_and_lookup(c, strs, "zzzzzzzzzzzzzzzzzzzzzzzzzz");

	strs_delete(strs);
}
