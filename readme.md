# Dynamic Array

A dynamic array implentation using `void*` ,loosely based on the `std::vector`
from C++.

As with the macro based dynamic array, there is no error checking here as this
is simply a proof of concept and not intended for use.

See: [macro_dynamic_array](https://github.com/polydung-dev/macro_dynamic_array)

## Differences from the macro approach

The dynamic array is now an "opaque" type; The programmer cannot modify the
object directly and so, if the "library" functions are written correctly, the
object will always be in a valid state (excluding hardware failures and such).

Unfortunately, the trade off for this is _type erasure_; It is up to the
programmer to ensure that the correct data types are used when interacting with
the array. Failure to do so may result in a memory access violation or, at the
very least, erroneous interpretation of the data.

In addition, as the values are passed via pointers, the syntax for appending a
literal value is verbose and requires C99 or later (`compound literal`), though
the "library" functions themselves do not.

Example:

```c
da_append(da, &(int){420});
```

With some further work, it may be possible to mitigate these limitations.

## Potential Failure States

Again, `malloc` and friends are assumed to always succeed. It is also assumed
that the pointer being passed to these functions is a pointer to a valid
object.

If either of these assumptions are false, the program will fail.

## Macro Wrappers

Type casting can be managed with macros which wrap the functions, thanks to
`__typeof__()`. I believe that this is a compiler extension, however, every
compiler that I have tried (`gcc`, `clang`, `tcc`, `zig cc`) supports this and
a C compiler _must_ be able to deduce types in order to implement `sizeof`.

Now, `da_back()` can be implemented using a simple array access instead of
manual pointer arithmetic.

Some functions (such as `da_size`) are implemented as wrappers around the
original function as this will result in an `r-value` and prevent the value
from being directly modified.

Note: This moves the "type erasure" from the underlying array to the dynamic
array object itself. Passing a pointer to something that is not a dynamic array
will result in an error.
