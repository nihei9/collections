#include "collections.h"
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

void put_and_lookup(connie_Connie *c, syms_SymbolStore *syms, const char *str)
{
	const syms_SymbolID *id;
	const char *s;

	id = syms_put(syms, str);
	A_NOT_NULL(c, id);

	s = syms_lookup(syms, *id);
	A_NOT_NULL(c, s);
	A_EQL_STRING(c, str, s);
}

void test_1(connie_Connie *c)
{
	syms_SymbolStore *syms;

	syms = syms_new();
	A_NOT_NULL(c, syms);

	put_and_lookup(c, syms, "a");
	put_and_lookup(c, syms, "bb");
	put_and_lookup(c, syms, "ccc");
	put_and_lookup(c, syms, "dddd");
	put_and_lookup(c, syms, "eeeee");
	put_and_lookup(c, syms, "ffffff");
	put_and_lookup(c, syms, "ggggggg");
	put_and_lookup(c, syms, "hhhhhhhh");
	put_and_lookup(c, syms, "iiiiiiiii");
	put_and_lookup(c, syms, "jjjjjjjjjj");
	put_and_lookup(c, syms, "kkkkkkkkkkk");
	put_and_lookup(c, syms, "llllllllllll");
	put_and_lookup(c, syms, "mmmmmmmmmmmmm");
	put_and_lookup(c, syms, "nnnnnnnnnnnnnn");
	put_and_lookup(c, syms, "ooooooooooooooo");
	put_and_lookup(c, syms, "pppppppppppppppp");
	put_and_lookup(c, syms, "qqqqqqqqqqqqqqqqq");
	put_and_lookup(c, syms, "rrrrrrrrrrrrrrrrrr");
	put_and_lookup(c, syms, "sssssssssssssssssss");
	put_and_lookup(c, syms, "tttttttttttttttttttt");
	put_and_lookup(c, syms, "uuuuuuuuuuuuuuuuuuuuu");
	put_and_lookup(c, syms, "vvvvvvvvvvvvvvvvvvvvvv");
	put_and_lookup(c, syms, "wwwwwwwwwwwwwwwwwwwwwww");
	put_and_lookup(c, syms, "xxxxxxxxxxxxxxxxxxxxxxxx");
	put_and_lookup(c, syms, "yyyyyyyyyyyyyyyyyyyyyyyyy");
	put_and_lookup(c, syms, "zzzzzzzzzzzzzzzzzzzzzzzzzz");

	syms_delete(syms);
}
