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
