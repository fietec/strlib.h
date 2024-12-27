#define STR_DEBUG
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
	str a = str("Hallo");
	str b = str("World");
	str c = str_concat(c_alloc, a, str(" "), b, str("!"));
	str_print(c);
	str_replace_str_mod(c, str("ll"), str("d"));
	str_print(c);
	return 0;
}