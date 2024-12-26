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
	printf("%d\n", c.len);
	str d = str_remove_str(c, str("llo"), c_alloc);
	str_print(d);
	printf("%d\n", d.len);
	return 0;
}