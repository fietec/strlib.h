#define STRLIB_IMPLEMENTATION
#include "strlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void* c_alloc(size_t n)
{
	void *p = malloc(n);
	strlib_memset(p, 0, n);
	return p;
}

int main(void)
{
	str hello = str_concat(c_alloc, str("hello"), str(" "), str("world"));
	str_print(hello);
	str_pair halfs = str_split(c_alloc, hello, ' ');
	str_print(halfs.a);
	str_print(halfs.b);
	return 0;
}