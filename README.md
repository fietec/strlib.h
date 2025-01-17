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
char* strlib_to_upper(char *s);
char* strlib_to_lower(char *s);
```

### str functions
```c 
StrAlloc str str_new(char *s, Allocator alloc);
StrAlloc str str_dup(str string, Allocator alloc);
StrAlloc str str_sub(str string, size_t from, size_t to, Allocator alloc);
StrAlloc str_pair str_split(str string, char del, Allocator alloc);
StrAlloc str_pair str_split_str(str string, str del, Allocator alloc);
StrAlloc str_array str_split_all(str string, char del, Allocator alloc);
StrAlloc str_array str_split_str_all(str string, str del, Allocator alloc);
StrAlloc str str_concat(Allocator alloc, (str)(...));
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

StrMod str* str_to_upper_mod(str *string);
StrMod str* str_to_lower_mod(str *string);
StrMod str* str_replace_mod(str *string, char a, char b);
StrMod str* str_replace_str_mod(str *string, str a, str b);
StrMod str* str_remove_mod(str *string, char c);
StrMod str* str_remove_str_mod(str *string, str s);

char *str_to_buffer(str s, char *buffer, size_t buffer_size);
int str_find(str string, char c);
int str_find_str(str string, str query);
bool str_starts_with(str string, char c);
bool str_starts_with_str(str base, str start);
bool str_ends_with(str string, char c);
bool str_ends_with_str(str base, str end);
bool str_equals(str a, str b);
bool str_equals_hashed(str a, str b);
str str_from(str string, size_t from);
str str_peek(str string, size_t from, size_t to);
size_t str_count(str string, char c);
size_t str_count_str(str string, str s);

// manual memory deallocation
void str_free(str string, Deallocator dealloc);
void str_free_pair(str_pair pair, Deallocator dealloc);
void str_free_array(str_array array, Deallocator dealloc);

// printing
void str_print(str)
void str_print_pair(str_pair)
void str_print_array(str_array);

// helper macros
str str(char*)
char str_at(str, int)
bool str_empty(str)

// console output
str_info(char*, ...)
str_error(char*, ...) // define STR_COLOR_PRINT to enable printing errors in red 
```

### Missing features
```c
str_indices str_find_all(str string, char c);
str_indices str_find_all_str(str string, str s);
bool str_contains(str string, char c);
bool str_contains_str(str string, str s);
StrAlloc str str_join(str_array strings, char delimiter, Allocator alloc);
StrAlloc str str_join_str(str_array strings, str delimiter, Allocator alloc);
```
