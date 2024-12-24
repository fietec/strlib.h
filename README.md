# strlib.h

A simple string library for C.
The library uses a `str` struct which contains the c-string and its length.
Dynamic memory allocation is done via a custom `Allocator` function which is passed by the user.
Therefore, custom memory management systems, such as arenas are supported. 
The use of dynamic allocation is documented by the presence of an `Allocator` in a function's structure.

## Features

### c-string functions
For convenience, custom string functions are also included, namely:
``` c
size_t strlib_len(char *s);
void strlib_ncpy(char *s, size_t n, char *d);
char* strlib_dup(char *s, Allocator alloc);
void strlib_memset(char *d, char v, size_t n);
```

### str functions
`str str(char *string)` (macro)

Creates a str from a c-string.

`str str_new(char *s, Allocator alloc)`

Creates a str from a c-string, by creating a copy.

`str str_dup(str string, Allocator alloc)`

Creates a copy of a str.

`char *str_to_buffer(str s, char *buffer, size_t buffer_size`

Writes the content of a str into a buffer. Returns the pointer to the character after the end of the written string.

`str str_sub(Allocator alloc, str string, size_t from, size_t to)`

Creates a sub-str from a given str within index bounds.

`str_pair str_split(Allocator alloc, str string, char del)`

Splits a str into a str_pair based on a char delimeter.

`int str_find(str string, char c)`

Returns the index of the first instance of a character in a str. (STR_NOT_FOUND on error)

`str str_concat(Allocator alloc, (str) ...)` (macro)

Concatinates multiple strs.

`str_print(str string)`

Prints the content of a string in quotes on a new line.

```c
int str_find_str(str string, str query);
```
```c
bool str_starts_with(str base, str start);
```
```c
bool str_ends_with(str base, str end);
```
```c
bool str_equals(str a, str b);
```
```c
str str_from(str string, size_t from);
```
```c
str str_peek(str string, size_t from, size_t to);
```
```c
size_t str_count(str string, char c);
```
```c
size_t str_count_str(str string, str s);
```
```c
void str_replace(str string, char a, char b);
```
```c
StrAlloc str str_replace_str(str string, str a, str b, Allocator alloc);
```
