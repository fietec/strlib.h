#ifndef _STRLIB_H
#define _STRLIB_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h> // only for exit

#ifndef ALLOCATOR
	#define ALLOCATOR
	typedef void* (*Allocator) (size_t);
    typedef void  (*Deallocator) (void*);
#endif // ALLOCATOR

#define STR_NUMARGS(...)  (sizeof((str[]){ __VA_ARGS__})/sizeof(str))
#define STR_NOT_FOUND -1
#define StrAlloc // functions prefixed with this dynamically allocate memory
#define StrMod // functions prefixed with this modify the content of a given string. Do not provide read-only constants!

#ifdef STR_DEBUG
	#define str_info(msg, ...) (printf("%s:%d: " msg"\n", __FILE__, __LINE__, ## __VA_ARGS__))
#else
	#define str_info(msg, ...)
#endif // STR_DEBUG

#ifdef STR_COLOR_PRINT
	#define STR_ANSI_RGB(r, g, b) ("\e[38;2;" #r ";" #g ";" #b "m") // set ansi color to rgb value
	#define STR_ANSI_END "\e[0m" // reset ansi color
	#define str_error(msg, ...) (fprintf(stderr, "%s[ERROR] %s:%d in %s: " msg STR_ANSI_END "\n", STR_ANSI_RGB(255, 0, 0), __FILE__, __LINE__, __func__, ##__VA_ARGS__))
#else
	#define str_error(msg, ...) (fprintf(stderr, "[ERROR] %s:%d in %s: " msg "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__))
#endif // STR_COLOR_PRINT

#define str_assert(state, msg, ...) do{if (!(state)) {str_error(msg, ##__VA_ARGS__); exit(1);}} while (0)
#define str__assert_alloc(value) str_assert((value)!=NULL, "Out of memory!")
#define str__assert_allocator(alloc) str_assert((alloc) != NULL, "Allocator may not be NULL!")
#define str__assert_deallocator(dealloc) str_assert((dealloc) != NULL, "Deallocator may not be NULL!")
#define str__free(_str, _dealloc) do{if((_str).value!=NULL){_dealloc((_str).value);}}while(0)

typedef struct{
	char *value;
	size_t len;
} str;

typedef struct{
	str a;
	str b;
} str_pair;

typedef struct{
    str *items;
    size_t count;
} str_array;

size_t strlib_len(char *s);
char* strlib_ncpy(char *s, size_t n, char *d);
char* strlib_dup(char *s, Allocator alloc);
void strlib_memset(char *d, char v, size_t n);
char* strlib_to_upper(char *s);
char* strlib_to_lower(char *s);

// functions using dynamic memory allocation
StrAlloc str str_new(char *s, Allocator alloc);
StrAlloc str str_dup(str string, Allocator alloc);
StrAlloc str str_sub(str string, size_t from, size_t to, Allocator alloc);
StrAlloc str_pair str_split(str string, char del, Allocator alloc);
StrAlloc str_pair str_split_str(str string, str del, Allocator alloc);
StrAlloc str_array str_split_all(str string, char del, Allocator alloc);
StrAlloc str_array str_split_all_str(str string, str del, Allocator alloc);
StrAlloc str str_replace(str string, char a, char b, Allocator alloc);
StrAlloc str str_replace_str(str string, str a, str b, Allocator alloc);
StrAlloc str str_remove(str string, char c, Allocator alloc);
StrAlloc str str_remove_str(str string, str s, Allocator alloc);
StrAlloc str str_insert(str string, str s, size_t index, Allocator alloc);
StrAlloc str str_to_upper(str string, Allocator alloc);
StrAlloc str str_to_lower(str string, Allocator alloc);
StrAlloc str str_reverse(str string, Allocator alloc);
StrAlloc str str_trim_left(str string, char c, Allocator alloc);
StrAlloc str str_trim_left_str(str string, str s, Allocator alloc);
StrAlloc str str_trim_right(str string, char c, Allocator alloc);
StrAlloc str str_trim_right_str(str string, str s, Allocator alloc);
StrAlloc str str_trim(str string, char c, Allocator alloc);
StrAlloc str str_trim_str(str string, str s, Allocator alloc);
StrAlloc str str_merge(str_array strings, Allocator alloc);
StrAlloc str str_join(str_array strings, char delimiter, Allocator alloc);
StrAlloc str str_join_str(str_array strings, str delimiter, Allocator alloc);

// functions that modify a string's content, return the given string pointer
StrMod str* str_to_upper_mod(str *string);
StrMod str* str_to_lower_mod(str *string);
StrMod str* str_replace_mod(str *string, char a, char b);
StrMod str* str_replace_str_mod(str *string, str a, str b);
StrMod str* str_remove_mod(str *string, char c);
StrMod str* str_remove_str_mod(str *string, str s);

char *str_to_buffer(str s, char *buffer, size_t buffer_size);
int str_find(str string, char c);
int str_find_str(str string, str query);
bool str_contains(str string, char c);
bool str_contains_str(str string, str s);
bool str_starts_with(str string, char c);
bool str_starts_with_str(str base, str start);
bool str_ends_with(str string, char c);
bool str_ends_with_str(str base, str end);
size_t str_hash(str s);
bool str_equals(str a, str b);
bool str_equals_hashed(str a, str b);
str str_from(str string, size_t from);
str str_peek(str string, size_t from, size_t to);
size_t str_count(str string, char c);
size_t str_count_str(str string, str s);

// use these functions when manually freeing allocated memory
void str_free(str string, Deallocator dealloc);
void str_free_pair(str_pair pair, Deallocator dealloc);
void str_free_array(str_array array, Deallocator dealloc);

void str_print_array(str_array arr);

void* str__alloc(Allocator alloc, size_t n);

#define str(s) (str){.value=(s), .len=strlib_len((s))}
#define str_array(...) ((str_array){.items=((str[]){__VA_ARGS__}), .count=STR_NUMARGS(__VA_ARGS__)})
#define str_concat(alloc, ...) (str_merge(str_array(__VA_ARGS__), (alloc)))
#define str_print(str) (printf("\"%s\"\n", (str).value))
#define str_print_pair(str_pair) (printf("(\"%s\", \"%s\")\n", (str_pair).a.value, (str_pair).b.value))
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
	if (s == NULL || d == NULL) return d;
	while (n-- > 0){
		*d++ = *s++;
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
	str__assert_allocator(alloc);
	if (s == NULL || alloc == NULL) return NULL;
	char *string = (char*) str__alloc(alloc, strlib_len(s)+1);
	char *d = string;
	while (*s != '\0'){
		*d++ = *s++;
	}
	return string;
}

char* strlib_to_lower(char *s)
{
    if (s == NULL) return NULL;
    char c;
    char *rw = s;
    while ((c=*rw) != '\0'){
        if (0x40 < c && c < 0x5b) *rw = c | 0x20;
        rw++;
    }
    return s;
}

char* strlib_to_upper(char *s)
{
    if (s == NULL) return NULL;
    char c;
    char *rw = s;
    while ((c=*rw) != '\0'){
        if (0x60 < c && c < 0x7b) *rw = c & 0xDF;
        rw++;
    }
    return s;
}

str str_new(char *s, Allocator alloc)
{
	str__assert_allocator(alloc);
	return (str) {.value=strlib_dup(s, alloc), .len = strlib_len(s)};
}

str str_dup(str string, Allocator alloc)
{
	str__assert_allocator(alloc);
	return (str) {.value=strlib_dup(string.value, alloc), .len=string.len};
}

char* str_to_buffer(str s, char *buffer, size_t buffer_size)
{
	if (buffer == NULL || s.len >= buffer_size) return NULL;
	return strlib_ncpy(s.value, s.len, buffer);
}

StrAlloc str str_merge(str_array strings, Allocator alloc)
{
    str__assert_allocator(alloc);
    size_t length = 0;
    for (size_t i=0; i<strings.count; ++i){
        length += strings.items[i].len;
    }
    char *value = str__alloc(alloc, length+1);
    char *w = value;
    for (size_t i=0; i<strings.count; ++i){
        str item = strings.items[i];
        w = strlib_ncpy(item.value, item.len, w);
    }
    return (str) {.value=value, .len=length};
}

str str_sub(str string, size_t from, size_t to, Allocator alloc)
{
	str__assert_allocator(alloc);
	if (to > string.len || from >= to) return (str) {0};
	size_t length = to-from;
	char *value = str__alloc(alloc, length+1);
	strlib_ncpy(string.value+from, length, value);
	return (str) {.value=value, .len=length};
}

str_pair str_split(str string, char del, Allocator alloc)
{
	str__assert_allocator(alloc);
	if (string.len == 0 || string.value == NULL) return (str_pair) {0};
	for (size_t i=0; i<string.len; ++i){
		if (string.value[i] == del){
			return (str_pair) {str_sub(string, 0, i, alloc), str_sub(string, i+1, string.len, alloc)};
		}
	}
	return (str_pair) {str_dup(string, alloc), {0}};
}

str_pair str_split_str(str string, str del, Allocator alloc)
{
	str__assert_allocator(alloc);
	if (string.len == 0 || string.value == NULL) return (str_pair) {0};
	int i = str_find_str(string, del);
	if (i >= 0){
		return (str_pair) {str_sub(string, 0, i, alloc), str_sub(string, i+del.len, string.len, alloc)};
	}
	return (str_pair) {str_dup(string, alloc), {0}};
}

str_array str_split_all(str string, char del, Allocator alloc)
{
    str__assert_allocator(alloc);
    if (string.len == 0 || string.value == NULL) return (str_array) {0};
    size_t count = str_count(string, del);
    str *array = str__alloc(alloc, (count+1)*sizeof(str));
    char *r = string.value;
    char *w;
    for (size_t i=0; i<count; ++i){
        int size = str_find(str(r), del);
        if (size == STR_NOT_FOUND){
            str_error("string was changed during runtime!");
            return (str_array) {0};
        }
        w = str__alloc(alloc, size+1);
        strlib_ncpy(r, size, w);
        array[i] = (str) {.value=w, .len=size};
        r += size+1;
    }
    size_t rem = string.value+string.len-r;
    w = str__alloc(alloc, rem+1);
    strlib_ncpy(r, rem, w);
    array[count] = (str) {.value=w, .len=rem};
    return (str_array) {.items=array, .count=count+1};
}

str_array str_split_all_str(str string, str del, Allocator alloc)
{
    str__assert_allocator(alloc);
    if (string.len == 0 || string.value == NULL) return (str_array) {0};
    size_t count = str_count_str(string, del);
    str *array = str__alloc(alloc, (count+1)*sizeof(str));
    char *r = string.value;
    char *w;
    for (size_t i=0; i<count; ++i){
        int size = str_find_str(str(r), del);
        if (size == STR_NOT_FOUND){
            str_error("string was changed during runtime!");
            return (str_array) {0};
        }   
        w = str__alloc(alloc, size+1);
        strlib_ncpy(r, size, w);
        array[i] = (str) {.value=w, .len=size};
        r += size+del.len;
    }
    size_t rem = string.value+string.len-r;
    w = str__alloc(alloc, rem+1);
    strlib_ncpy(r, rem, w);
    array[count] = (str) {.value=w, .len=rem};
    return (str_array) {.items=array, .count=count+1};
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
		if (str_starts_with_str(str_from(string, i), query)) return i;
	}
	return STR_NOT_FOUND;
}

bool str_starts_with_str(str base, str start)
{
	if (start.len > base.len) return false;
	for (size_t i=0; i<start.len; ++i){
		if (start.value[i] != base.value[i]) return false;
	}
	return true;
}

bool str_ends_with_str(str base, str end)
{
	if (end.len > base.len) return false;
	size_t offset = end.len - base.len;
	for (size_t i=0; i<end.len; ++i){
		if (base.value[offset+i] != end.value[i]) return false;
	}
	return true;
}

bool str_starts_with(str string, char c)
{
    if (string.value == NULL || string.len < 1) return false;
    return *string.value == c;
}

bool str_ends_with(str string, char c)
{
    if (string.value == NULL || string.len < 1) return false;
    return *(string.value+string.len-1) == c;
}

size_t str_hash(str s)
{
	size_t hash = 5381; 
    for (size_t i = 0; i < s.len; ++i) {
        hash = ((hash << 5) + hash) + s.value[i];
    }
    return hash;
}

bool str_equals_hashed(str a, str b)
{
	if (a.len != b.len) return false;
	if (str_hash(a) != str_hash(b)) return false;
	char *pa = a.value;
	char *pb = b.value;
	while (a.len-- > 0){
		if (*pa++ != *pb++) return false;
	}
	return true;
}

bool str_equals(str a, str b)
{
	if (a.len != b.len) return false;
	char *pa = a.value;
	char *pb = b.value;
	while (a.len-- > 0){
		if (*pa++ != *pb++) return false;
	}
	return true;
}

bool str_contains(str string, char c)
{
    for (size_t i=0; i<string.len; ++i){
        if (*(string.value+i) == c) return true;
    }
    return false;
}

bool str_contains_str(str string, str s)
{
    char *r = string.value;
    while (r-string.value < string.len){
        if (str_starts_with_str(str(r++), s)) return true;
    }
    return false;
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

str* str_replace_mod(str *string, char a, char b)
{
    if (string == NULL) return NULL;
	char *p;
	for (size_t i=0; i<string->len; ++i){
		p = string->value + i;
		if (*p == a) *p=b;
	}
    return string;
}

str* str_replace_str_mod(str *string, str a, str b)
{
    if (string == NULL) return NULL;
	if (b.len > a.len){
		str_error("cannot replace string of length %u with string of length %u!", a.len, b.len);
		return string;
	}
	size_t count = str_count_str(*string, a);
	size_t length = string->len + count*(b.len-a.len);
	if (count == 0) return string;
	char *r = string->value;
	char *w = r;
	for (size_t i=0; i<count; ++i){
		char *n = r + str_find_str(str(r), a);
		if (n-r > 0){
			w = strlib_ncpy(r, n-r, w);
		}
		w = strlib_ncpy(b.value, b.len, w);
		r = n+a.len;
	}
	if (w-string->value < length){
		w = strlib_ncpy(r, length-(w-string->value)+1, w);
	}
	*w = '\0';
    string->len = w-string->value-1;
    return string;
}

str str_replace(str string, char a, char b, Allocator alloc)
{
	str__assert_allocator(alloc);
	char *value = strlib_dup(string.value, alloc);
	char *rw = value;
	for (size_t i=0; i<string.len; ++i){
		if (*rw == a) *rw = b;
		rw++;
	}
	return (str) {.value=value, .len=string.len};
}

str str_replace_str(str string, str a, str b, Allocator alloc)
{
	str__assert_allocator(alloc);
	size_t count = str_count_str(string, a);
	if (count == 0) return str_dup(string, alloc);
	size_t delta = b.len - a.len;
	size_t length = string.len + delta*count;
	char *value = str__alloc(alloc, length+1);
	char *w = value;
	char *r = string.value;
	for (size_t i=0; i<count; ++i){
		char *n = r + str_find_str(str(r), a);
		if (n-r > 0){
			w = strlib_ncpy(r, n-r, w);
		}
		w = strlib_ncpy(b.value, b.len, w);
		r = n+a.len;
	}
	if (r-string.value < length){
		strlib_ncpy(r, length-(r-string.value), w);
	}
	return (str) {.value=value, .len=length};
}

str str_remove(str string, char c, Allocator alloc)
{
	str__assert_allocator(alloc);
	size_t count = str_count(string, c);
	if (count == 0) return str_dup(string, alloc);
	size_t length = string.len - count;
	if (length <= 0) return (str) {0};
	char *value = str__alloc(alloc, length+1);
	char *r = string.value;
	char *w = value;
	for (size_t i=0; i<string.len; ++i){
		char cs = *r++;
		if (cs != c) *w++ = cs; 
	}
	*w = '\0';
	return (str) {.value=value, .len=length};
}

str str_remove_str(str string, str s, Allocator alloc)
{
	str__assert_allocator(alloc);
	size_t count = str_count_str(string, s);
	if (count == 0) return str_dup(string, alloc);
	size_t length = string.len - count*s.len;
	if (length == 0) return (str) {0};
	char *value = str__alloc(alloc, length+1);
	char *r = string.value;
	char *w = value;
	for (size_t i=0; i<count; ++i){
		int next = str_find_str(string, s);
		if (next == STR_NOT_FOUND){
            str_error("string was changed during runtime!");
            return (str) {0};
        }
		char *n = r + next;
		if (next > 0){
			w = strlib_ncpy(r, next, w);
		}
		r = n + s.len;
	}
	if (w - value < length){
		strlib_ncpy(r, length-(w-value), w);
	}
	return (str) {.value=value, .len=length};
}

str* str_remove_mod(str *string, char c)
{
    if (string == NULL) return NULL;
    if (string->value == NULL) return string;
    char *r = string->value;
    char *w = r;
    while (r-string->value <= string->len){
        if (*r == c) r++;
        else{
            *w++ = *r++;
        }
    }
    while (w < r){
        string->len --;
        *w++ = '\0';
    }
    return string;
}

str* str_remove_str_mod(str *string, str s)
{
    if (string == NULL) return NULL;
    if (string->value == NULL || s.value == NULL, s.len > string->len) return string;
    char *r = string->value;
    char *w = r;
    while (r-string->value <= string->len){
        if (str_starts_with_str(str(r), s)){
            r += s.len;
        }
        else{
            *w++ = *r++;
        }
    }
    while (w < r){
        string->len --;
        *w++ = '\0';
    }
    return string;
}

str str_insert(str string, str s, size_t index, Allocator alloc)
{
	str__assert_allocator(alloc);
	if (index >= string.len) return str_dup(string, alloc);
	size_t length = string.len + s.len;
	char *value = str__alloc(alloc, length + 1);
	char *w = strlib_ncpy(string.value, index, value);
	w = strlib_ncpy(s.value, s.len, w);
	w = strlib_ncpy(string.value+index, string.len-index, w);
	*w = '\0';
	return (str) {.value=value, .len=length};
}

str str_reverse(str string, Allocator alloc)
{
    str__assert_allocator(alloc);
    if (string.len <= 1) return str_dup(string, alloc); 
    char *value = strlib_dup(string.value, alloc);
    char *r1 = value;
    char *r2 = value+string.len-1;
    char c;
    while (r1 <= r2){
        c = *r1;
        *r1++ = *r2;
        *r2-- = c;
    }
    return (str){.value=value, .len=string.len};
}

str str_to_upper(str string, Allocator alloc)
{
    str__assert_allocator(alloc);
    if (string.value == NULL || string.len == 0) return str_dup(string, alloc);
    char *value = strlib_dup(string.value, alloc);
    return (str) {.value=strlib_to_upper(value), .len=string.len};
}

str str_to_lower(str string, Allocator alloc)
{
    str__assert_allocator(alloc);
    if (string.value == NULL || string.len == 0) return str_dup(string, alloc);
    char *value = strlib_dup(string.value, alloc);
    return (str) {.value=strlib_to_lower(value), .len=string.len};  
}

str* str_to_upper_mod(str *string)
{
    if (string == NULL) return NULL;
    if (string->value == NULL || string->len == 0) return string;
    strlib_to_upper(string->value);
    return string;
}

str* str_to_lower_mod(str *string)
{
    if (string == NULL) return NULL;
    if (string->value == NULL || string->len == 0) return string;
    strlib_to_lower(string->value);
    return string;
}

str str_trim_left(str string, char c, Allocator alloc)
{
    str__assert_allocator(alloc);
    if (string.value == NULL) return (str) {0};
    if (string.len == 0) return str_new("", alloc);
    char *r = string.value;
    while (*r == c){r++;}
    size_t length = string.value + string.len - r;
    if (length == 0) return str_new("", alloc);
    char *value = str__alloc(alloc, length+1);
    strlib_ncpy(r, length, value);
    return (str) {.value=value, .len=length};
}

str str_trim_left_str(str string, str s, Allocator alloc)
{
    str__assert_allocator(alloc);
    if (string.value == NULL) return (str) {0};
    if (s.len > string.len) return str_new("", alloc);
    char *r = string.value;
    while (str_starts_with_str(str(r), s)) r += s.len;
    size_t length = string.value + string.len - r;
    if (length == 0) return str_new("", alloc);
    char *value = str__alloc(alloc, length+1);
    strlib_ncpy(r, length, value);
    return (str) {.value=value, .len=length};
}

str str_trim_right(str string, char c, Allocator alloc)
{
    str__assert_allocator(alloc);
    if (string.value == NULL) return (str) {0};
    if (string.len == 0) return str_new("", alloc);
    char *r = string.value + string.len - 1;
    while (*r == c && r>=string.value) {r--;}
    size_t length = r-string.value+1;
    if (length == 0) return str_new("", alloc);
    char *value = str__alloc(alloc, length+1);
    strlib_ncpy(string.value, length, value);
    return (str) {.value=value, .len=length};
}

str str_trim_right_str(str string, str s, Allocator alloc)
{
    str__assert_allocator(alloc);
    if (string.value == NULL) return (str) {0};
    if (s.len > string.len) return str_new("", alloc);
    char *r = string.value + string.len - s.len;
    while (r >= string.value && str_starts_with_str(str(r), s)) {r -= s.len;}
    size_t length = r-string.value+2;
    if (length == 0) return str_new("", alloc);
    char *value = str__alloc(alloc, length+1);
    strlib_ncpy(string.value, length, value);
    return (str) {.value=value, .len=length};
}

str str_trim(str string, char c, Allocator alloc)
{
    str__assert_allocator(alloc);
    if (string.value == NULL) return (str) {0};
    if (string.len == 0) return str_new("", alloc);
    char *r1 = string.value;
    char *r2 = r1 + string.len - 1;
    while (r1 <= r2 && (*r1 == c || *r2 == c)) {
        if (*r1 == c) r1++;
        if (*r2 == c) r2--;
    }
    int length = r2-r1+1;
    if (length <= 0) return str_new("", alloc);
    char *value = str__alloc(alloc, length+1);
    strlib_ncpy(r1, length, value);
    return (str) {.value=value, .len=length};
}

str str_trim_str(str string, str s, Allocator alloc)
{
    str__assert_allocator(alloc);
    if (string.value == NULL) return (str) {0};
    if (string.len == 0 || s.len > string.len) return str_new("", alloc);
    char *r1 = string.value;
    char *r2 = string.value + string.len - s.len;
    bool f1 = false;
    bool f2 = false;
    while (r1 <= r2 && ((f1 = str_starts_with_str(str(r1), s)) || (f2 = str_starts_with_str(str(r2), s)))){
        if (f1) r1 += s.len;
        if (f2) r2 -= s.len;
    }
    int length = r2-r1+2;
    if (length <= 0) return str_new("", alloc);
    char *value = str__alloc(alloc, length+1);
    strlib_ncpy(r1, length, value);
    return (str) {.value=value, .len=length};
}

str str_join(str_array strings, char delimiter, Allocator alloc)
{
    str__assert_allocator(alloc);
    if (strings.count == 0) return str_new("", alloc);
    size_t length = strings.count-1;
    for (size_t i=0; i<strings.count; ++i){
        length += strings.items[i].len;
    }
    char *value = str__alloc(alloc, length+1);
    char *w = value;
    for (size_t i=0; i<strings.count; ++i){
        str item = strings.items[i];
        w = strlib_ncpy(item.value, item.len, w);
        if (i < strings.count-1)*w++ = delimiter;
    }
    return (str) {.value=value, .len=length};
}

str str_join_str(str_array strings, str delimiter, Allocator alloc)
{
    str__assert_allocator(alloc);
    if (strings.count == 0) return str_new("", alloc);
    size_t length = (strings.count-1)*delimiter.len;
    for (size_t i=0; i<strings.count; ++i){
        length += strings.items[i].len;
    }
    char *value = str__alloc(alloc, length+1);
    char *w = value;
    for (size_t i=0; i<strings.count; ++i){
        str item = strings.items[i];
        w = strlib_ncpy(item.value, item.len, w);
        if (i < strings.count-1){
            w = strlib_ncpy(delimiter.value, delimiter.len, w);
        }
    }
    return (str) {.value=value, .len=length};
}

void str_print_array(str_array arr)
{
    putchar('{');
    for (size_t i=0; i<arr.count-1; ++i){
       printf("\"%s\", ", arr.items[i].value);
    }
    printf("\"%s\"}\n", arr.items[arr.count-1].value);
}

void str_free(str string, Deallocator dealloc)
{
    str__assert_deallocator(dealloc);
    str__free(string, dealloc);
}

void str_free_pair(str_pair pair, Deallocator dealloc)
{
    str__assert_deallocator(dealloc);
    str__free(pair.a, dealloc);
    str__free(pair.b, dealloc);
}

void str_free_array(str_array array, Deallocator dealloc)
{
    str__assert_deallocator(dealloc);
    for (size_t i=0; i<array.count; ++i){
        str__free(array.items[i], dealloc);
    }
    dealloc(array.items);
}

void* str__alloc(Allocator alloc, size_t n)
{
    void *p = alloc(n);
    str__assert_alloc(p);
    strlib_memset(p, 0, n);
    return p;
}

#endif // STRLIB_IMPLEMENTATION