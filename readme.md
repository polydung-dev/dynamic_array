# Dynamic Array

A set of macros and functions to support easy dynmaic arrays.

This is the 3rd iteration of this library. This will be a brief overview of
each iteration.

## Pure Macro Magic

```c
struct DA_int {
	int* data;
	size_t size;
	size_t capacity;
};

int main(void) {
	struct DA_int da = {0};
	da_create(da);
	da_append(da, 69);
	int* x = da_at(da, 0);
	assert(*x == 69);
	da_destroy(da);
}
```

The first implementation consisted of a handful of macros which worked on a
struct with a common layout, and grouped the few required statements together
in an attempt to maintain a valid object state. As a macro is a relatively
simple copy/paste process, the "body" of the macro worked with any type of
array, making for an easy "generic" solution.

This approach had some drawbacks, mainly that a macro is a relatively simple
copy/paste process; This leads to a rather high density of parentheses which,
along with other wrangling efforts required to ensure that they behave, impacts
the reading and writing of these "functions".

A "function-like" macro is only function like to a certain degree;

- It doesn't create its own scope like a function does; Common variable names must be avoided (e.g. nested loop with C89 variable declarations).
- The "body" of the function needs to be wrapped in a `do { /* ... */ } while (0)` loop in order to behave as a single statement.
- There is no return value, only in-out "parameters".

## `void*` Type Erasure

```c
/* da.c */
struct DynamicArray {
	void* data;
	size_t size;
	size_t capacity;
	size_t elem_size;
};

/* da.h */
typedef struct DynamicArray da_type;

/* main.c */
int main(void) {
	da_type* da = da_create(sizeof(int))
	da_append(da, &(int){69});
	int* x = (int*)da_at(da, 0, sizeof(int));
	assert(*x == 69);
	da_destroy(da);
}

```

A different approach is to use a `void*` as the array and doing the pointer
arithmetic manually. This allows for a single object type to represent any type
of array, and the opportunity to hide the internals of the type with a typedef,
further helping to ensure that the dynamic array object remains in a valid
state (excluding hardware failures and such).

The dynamic array object can now be initialised directly. Although values must
now be passed by pointer, a C99 "compound literal" can be used in place of an
actual variable.

Directly using these functions results in verbose syntax and repeated casting
being littered throughout; These issues can be solved via simple macros and the
use of `__typeof__`.

```c
int main(void) {
	da_init(da, int);
	da_append(da, 69);
	int* x = da_at(da, 0);
	assert(*x == 69);
	da_free(da);
}
```

## "Pascal" (length-prefix'd) Dynamic Array

```c
int main(void) {
	int* da = NULL;
	da_append(da, 69);
	int* x = da_at(da, 0);
	assert(*x == 69);
	da_free(da);
}
```

This approach is very different from the others; Instead of providing some
dynamic array object, these functions work with a standard array type. A chunk
of memory is allocated for the array plus a header, and a pointer to the array
is returned. Functions exist to retrieve the data from the header, but the data
cannot be modified directly.

These functions were also written to handle taking a `NULL` pointer, such that
the array does not need to be explicitly initialised. On top of this, `da_free`
will set the pointer to `NULL` after free'ing the memory.

### Alignment

As the `data` pointer was not directly returned by `malloc`, care must be taken
to ensure that the `data` pointer is properly aligned for the data type. By
coincidence, two `size_t`'s at 8-bytes each (64-bit OS) will align the `data`
pointer at 16-bytes which is the maximum alignment (a `long double`).

This may not always be the case, so the header size is calculated.

## Next

The next thing to implement would be some error handling, in particular, the
`da_reserve` function has no way to signal an error has occured. This will
likely be done in a similar fashion to `errno` and friends. It is also still
possible to pass any pointer to these functions, a system to keep track of
valid dynamic array pointers may be slightly useful.

Additionally, an "iterator" like system similar to the C++ `std::vector`
functions may be worth consideration, or perhaps not.
