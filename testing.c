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
	str b = str("World");
	str c = str_concat(c_alloc, a, str(" "), b, str("!"));
	str_print(c);
	str_pair pair = str_split_str(c, str(" "), c_alloc);
	str_print(pair.a);
	str_print(pair.b);
	return 0;
}