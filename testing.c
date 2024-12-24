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
	str d = str_replace_str(c, a, str("Hello"), c_alloc);
	str_print(d);
	return 0;
}