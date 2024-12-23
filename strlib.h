#ifndef _STRLIB_H
#define _STRLIB_H

#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>

#ifndef ALLOCATOR
#define ALLOCATOR
typedef void* (*Allocator) (size_t);
#endif // ALLOCATOR

typedef struct{
	char *value;
	size_t len;
} str;

typedef struct{
	str a;
	str b;
} str_pair;

size_t strlib_len(char *s);
void strlib_ncpy(char *s, size_t n, char *d);
char* strlib_dup(char *s, Allocator alloc);
void strlib_memset(char *d, char v, size_t n);

str str_new(char *s, Allocator alloc);
str str_dup(str string, Allocator alloc);
char *str_to_buffer(str s, char *buffer, size_t buffer_size);
str str_sub(Allocator alloc, str string, size_t from, size_t to);
str_pair str_split(Allocator alloc, str string, char del);
int str_find(str string, char c);
str str__concat(Allocator alloc, int n, ...);

#define str(s) (str){.value=(s), .len=strlib_len((s))}
#define STR_NUMARGS(...)  (sizeof((str[]){{0}, ##__VA_ARGS__})/sizeof(str)-1)
#define str_concat(alloc, ...) (str__concat((alloc), STR_NUMARGS(__VA_ARGS__), ##__VA_ARGS__))
#define str_print(str) (printf("\"%s\"\n", (str).value))

#define STR_NOT_FOUND -1

#endif // _STRLIB_H
#ifdef STRLIB_IMPLEMENTATION

size_t strlib_len(char *s)
{
	if (s == NULL) return 0;
	char *r=s;
	while (*r++ != '\0'){}
	return r-s-1;
}

void strlib_ncpy(char *s, size_t n, char *d)
{
	if (s == NULL || d == NULL || n == 0) return;
	while (n > 0){
		*d++ = *s++;
		n--;
	}
}

void strlib_memset(char *s, char v, size_t n)
{
	if (s == NULL || n == 0) return;
	while (n-- > 0){
		*s++ = v;
	}
}

char* strlib_dup(char *s, Allocator alloc)
{
	if (s == NULL || alloc == NULL) return NULL;
	char *d = (char*) alloc(strlib_len(s)+1);
	while (*s != '\0'){
		*d++ = *s++;
	}
	return d;
}

str str_new(char *s, Allocator alloc)
{
	if (alloc == NULL) return (str) {0};
	return (str) {.value=strlib_dup(s, alloc), .len = strlib_len(s)};
}

str str_dup(str string, Allocator alloc)
{
	if (alloc == NULL) return (str) {0};
	return (str) {.value=strlib_dup(string.value, alloc), .len=string.len};
}

char* str_to_buffer(str s, char *buffer, size_t buffer_size)
{
	if (buffer == NULL || s.len >= buffer_size) return NULL;
	strlib_ncpy(s.value, s.len, buffer);
	return buffer + s.len;
}

str str__concat(Allocator alloc, int n, ...)
{
	if (alloc == NULL || n == 0) return (str) {0};
	va_list args;
	va_start(args, n);
	str strings[n];
	size_t length = 0;
	for (int i=0; i<n; ++i){
		str temp = va_arg(args, str);
		length += temp.len;
		strings[i] = temp;
	}
	va_end(args);
	size_t buff_size = length+1;
	char *value = alloc(buff_size);
	assert(value != NULL && "Out of memory!");
	char *w = value;
	for (int i=0; i<n; ++i){
		str temp = strings[i];
		w = str_to_buffer(temp, w, buff_size);
		buff_size -= temp.len;
	}
	return (str) {.value=value, .len=length};
}

str str_sub(Allocator alloc, str string, size_t from, size_t to)
{
	if (alloc == NULL || to > string.len || from >= to) return (str) {0};
	size_t length = to-from;
	char *value = alloc(length+1);
	strlib_ncpy(string.value+from, length, value);
	return (str) {.value=value, .len=length};
}

str_pair str_split(Allocator alloc, str string, char del)
{
	if (alloc == NULL || string.len == 0 || string.value == NULL) return (str_pair) {0};
	for (size_t i=0; i<string.len; ++i){
		if (string.value[i] == del){
			return (str_pair) {str_sub(alloc, string, 0, i), str_sub(alloc, string, i+1, string.len)};
		}
	}
	return (str_pair) {str_dup(string, alloc), {0}};
}

int str_find(str string, char c)
{
	for (size_t i=0; i<string.len; ++i){
		if (string.value[i] == c) return i;
	}
	return STR_NOT_FOUND;
}

#endif // STRLIB_IMPLEMENTATION