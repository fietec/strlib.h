#define STRLIB_IMPLEMENTATION
#define STR_DEBUG
#include "strlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define b(value) ((value)? "true": "false")

void* c_alloc(size_t n)
{
	void *p = malloc(n);
	strlib_memset(p, 0, n);
	return p;
}

int main(void)
{
	// str a = str("halloal al");
	// printf("%d\n", str_find_str(str_from(a, 2), str("al")));
	str a = str("Hallo World");
	printf("%d -> %s\n", a.len, a.value);
	str_print(a);
	str_remove_mod(&a, 'l');
	printf("%d -> %s\n", a.len, a.value);
	return 0;
}