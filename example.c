#define STR_DEBUG
#define STR_COLOR_PRINT
#define STRLIB_IMPLEMENTATION
#include "strlib.h"
#include <stdio.h>
#include <stdlib.h>

void* c_alloc(size_t n)
{
	void *p = malloc(n);
	strlib_memset(p, 0, n);
	return p;
}

int main(void)
{
	str a = str("Hello");
	str b = str("Earth");
	str c = str_concat(c_alloc, a, str(" "), b, str("!"));
	str_print(c);
	str_replace_str_mod(c, b, str("World"));
	str_replace_mod(c, 'W', 'w');
	str_print(c);
	str e = str_insert(c, str("to the entire "), str_find(c, 'w'), c_alloc);
	str_print(e);
	str_print_pair(str_split_str(e, str(" the "), c_alloc));
	return 0;
}