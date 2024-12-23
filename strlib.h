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

#define STR_NUMARGS(...)  (sizeof((str[]){{0}, ##__VA_ARGS__})/sizeof(str)-1)
#define STR_NOT_FOUND -1
#define StrAlloc

#ifdef STR_DEBUG
#define str_info(msg, ...) (printf("%s:%d: " msg"\n", __FILE__, __LINE__, ## __VA_ARGS__))
#else
#define str_info(msg, ...)
#endif // STR_DEBUG

typedef struct{
	char *value;
	size_t len;
} str;

typedef struct{
	str a;
	str b;
} str_pair;

size_t strlib_len(char *s);
char* strlib_ncpy(char *s, size_t n, char *d);
char* strlib_dup(char *s, Allocator alloc);
void strlib_memset(char *d, char v, size_t n);

StrAlloc str str_new(char *s, Allocator alloc);
StrAlloc str str_dup(str string, Allocator alloc);
StrAlloc str str_sub(Allocator alloc, str string, size_t from, size_t to);
StrAlloc str_pair str_split(Allocator alloc, str string, char del);
StrAlloc str str__concat(Allocator alloc, int n, ...);
StrAlloc str str_replace_str(str string, str a, str b, Allocator alloc);
char *str_to_buffer(str s, char *buffer, size_t buffer_size);
int str_find(str string, char c);
int str_find_str(str string, str query);
bool str_starts_with(str base, str start);
bool str_ends_with(str base, str end);
bool str_equals(str a, str b);
str str_from(str string, size_t from);
str str_peek(str string, size_t from, size_t to);
size_t str_count(str string, char c);
size_t str_count_str(str string, str s);
void str_replace(str string, char a, char b);

#define str(s) (str){.value=(s), .len=strlib_len((s))}
#define str_concat(alloc, ...) (str__concat((alloc), STR_NUMARGS(__VA_ARGS__), ##__VA_ARGS__))
#define str_print(str) (printf("\"%s\"\n", (str).value))
#define str_at(str, i) ((str).value[(i)])
#define str_empty(str) ((str).len == 0)

#endif // _STRLIB_H
#ifdef STRLIB_IMPLEMENTATION

size_t strlib_len(char *s)
{
	if (s == NULL) return 0;
	char *r=s;
	while (*r++ != '\0'){}
	return r-s-1;
}

char* strlib_ncpy(char *s, size_t n, char *d)
{
	if (s == NULL || d == NULL || n == 0) return d;
	while (n > 0){
		*d++ = *s++;
		n--;
	}
	return d;
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

int str_find_str(str string, str query)
{
	if (str_empty(string) || str_empty(query) || query.len > string.len) return STR_NOT_FOUND;
	for (size_t i=0; i<string.len-query.len+1; ++i){
		if (str_starts_with(str_from(string, i), query)) return i;
	}
	return STR_NOT_FOUND;
}

bool str_starts_with(str base, str start)
{
	if (start.len > base.len) return false;
	for (size_t i=0; i<start.len; ++i){
		if (start.value[i] != base.value[i]) return false;
	}
	return true;
}

bool str_ends_with(str base, str end)
{
	if (end.len > base.len) return false;
	size_t offset = end.len - base.len;
	for (size_t i=0; i<end.len; ++i){
		if (base.value[offset+i] != end.value[i]) return false;
	}
	return true;
}

bool str_equals(str a, str b)
{
	if (a.len != b.len) return false;
	size_t len = a.len;
	char *pa = a.value;
	char *pb = b.value;
	while (len-- > 0){
		if (*pa++ != *pb++) return false;
	}
	return true;
}

str str_from(str string, size_t from)
{
	if (from >= string.len) return (str) {0};
	return (str) {.value=string.value+from, .len=string.len-from};
}

str str_peek(str string, size_t from, size_t to)
{
	if (from >= to || to > string.len || from > string.len) return (str) {0};
	return (str) {.value=string.value + from, .len=to-from};
}

size_t str_count(str string, char c)
{
	size_t count = 0;
	for (size_t i=0; i<string.len; ++i){
		if (string.value[i] == c) count++;
	}
	return count;
}

size_t str_count_str(str string, str s)
{
	size_t count = 0;
	int index = 0;
	int i;
	while ((i = str_find_str(str_from(string, index), s)) != STR_NOT_FOUND){
		count++;
		index += i+1;
	}	
	return count;
}

void str_replace(str string, char a, char b)
{
	char *p;
	for (size_t i=0; i<string.len; ++i){
		p = string.value + i;
		if (*p == a) *p=b;
	}
}

str str_replace_str(str string, str a, str b, Allocator alloc)
{
	size_t count = str_count_str(string, a);
	if (count == 0) return str_dup(string, alloc);
	size_t delta = b.len - a.len;
	size_t length = string.len + delta*count;
	char *value = alloc(length+1);
	char *w = value;
	char *r = string.value;
	for (size_t i=0; i<count; ++i){
		char *n = r + str_find_str(str(r), a);
		if (n-r > 0)
		w = strlib_ncpy(r, n-r, w);
		w = strlib_ncpy(b.value, b.len, w);
		r = n+a.len;
	}
	if (r-string.value < length){
		strlib_ncpy(r, length-(r-string.value), w);
	}
	return (str) {.value=value, .len=length};
}

#endif // STRLIB_IMPLEMENTATION