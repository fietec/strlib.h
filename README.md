# strlib.h

A simple string library for C.
The library uses a `str` struct which contains the c-string and its length.
Dynamic memory allocation is done via a custom `Allocator` function, which is passed by the user.
Therefore, custom memory management systems, such as arenas are supported. 
The use of dynamic allocation is documented by the presence of `StrAlloc` in a function's declaration.
Functions that modify the content of a given `str` are labeled with `StrMod`.

## Features

### c-string functions
For convenience, custom string functions are also included, namely:
``` c
size_t strlib_len(char *s);
char* strlib_ncpy(char *s, size_t n, char *d);
char* strlib_dup(char *s, Allocator alloc);
void strlib_memset(char *d, char v, size_t n);
```

### str functions
```c 
StrAlloc str str_new(char *s, Allocator alloc);
StrAlloc str str_dup(str string, Allocator alloc);
StrAlloc str str_sub(Allocator alloc, str string, size_t from, size_t to);
StrAlloc str_pair str_split(Allocator alloc, str string, char del);
StrAlloc str str_concat(Allocator alloc, (str)(...));
StrAlloc str str_replace(str string, char a, char b, Allocator alloc);
StrAlloc str str_replace_str(str string, str a, str b, Allocator alloc);
StrAlloc str str_remove(str string, char c, Allocator alloc);
StrAlloc str str_remove_str(str string, str s, Allocator alloc);

char *str_to_buffer(str s, char *buffer, size_t buffer_size);
int str_find(str string, char c);
int str_find_str(str string, str query);
bool str_starts_with(str base, str start);
bool str_ends_with(str base, str end);
bool str_equals(str a, str b);
bool str_equals_hashed(str a, str b);
str str_from(str string, size_t from);
str str_peek(str string, size_t from, size_t to);
size_t str_count(str string, char c);
size_t str_count_str(str string, str s);

StrMod void str_replace_mod(str string, char a, char b);
StrMod void str_replace_str_mod(str string, str a, str b);

// helper macros
str str(c-string)
void str_print(str)
char str_at(str, int)
bool str_empty(str)
```
