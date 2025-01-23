#ifndef DA_H
#define DA_H

#include <stddef.h>

/**
 * @file
 *
 * This is a "drop in" dynamic array which "hides" the metadata before the
 * array pointer.
 *
 * ```c
 * +------+------+----------+
 * | cap  | size | array    |
 * +------+------+----------+
 *   ^             ^
 *   header        data pointer
 * ```
 *
 * All functions will take/return the `data pointer`. This pointer can be
 * passed to any function expecting a standard array, as long as the pointer is
 * not `realloc`'d, `free`'d, etc. Memory management _must_ be done with the
 * supplied library functions.
 *
 * Manually calling `da_init()` is not required; As long as the pointer is
 * initialised to `NULL`, calling `da_append` will work fine.
 */

/*///////////////////////////////////////////////////////////////////////////*/
/* DynamicArray                                                              */
/*///////////////////////////////////////////////////////////////////////////*/

/**
 * Initialises the array + header.
 *
 * Note: The array can also be "initialised" by setting the pointer to `NULL`
 *
 * @param	elem_size	number of bytes per element
 *
 * @returns	on success	a pointer to an array
 * @returns	on failure	NULL
 *
 * **Errors**
 * - ENOMEM: Out of memory; set via `malloc`.
 *
 * @see	`da_free()`
 */
void* da_init(size_t elem_size);

/**
 * Free's the array.
 *
 * @param	arr	a valid `data pointer`
 *
 * @see	`da_init()`
 */
void da_free(void* arr);

/*///////////////////////////////////////////////////////////////////////////*/
/* Element Access                                                            */
/*///////////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////////////*/
/* Capacity                                                                  */
/*///////////////////////////////////////////////////////////////////////////*/

/**
 * Returns the current number of elements in the array.
 *
 * @param	arr	a valid `data pointer` or `NULL`
 *
 * @returns	the current number of elements
 * @returns	0 if arr == `NULL`
 */
#define da_size(arr) da_size_(arr)
size_t da_size_(void* arr);

/**
 * Returns the maximum number of elements that the array can hold.
 *
 * Note: Increase the array cpacity with `da_reserve()`
 *
 * @param	arr	a valid `data pointer` or `NULL`
 *
 * @returns	the maximum number of elements
 * @returns	0 if arr == `NULL`
 *
 * @see	`da_reserve()`
 */
#define da_capacity(arr) da_capacity_(arr)
size_t da_capacity_(void* arr);

/**
 * Reserves additional memory for the array.
 *
 * If the array is not initialised (i.e. `NULL` is passed) then the array will
 * be initialised first.
 *
 * @param	arr	a valid `data pointer` or `NULL`
 * @param	count	number of elements to allocate memory for
 *
 * **Errors**
 * - ENOMEM: Out of memory; set via `realloc`.
 */
#define da_reserve(arr, count) \
	da_reserve_((void**)&(arr), count, sizeof(*arr))
void da_reserve_(void** arr, size_t count, size_t elem_size);

/*///////////////////////////////////////////////////////////////////////////*/
/* Modifiers                                                                 */
/*///////////////////////////////////////////////////////////////////////////*/

/**
 * Copies the value to the end of the array, reallocating if required.
 *
 * @param	arr	a valid `data pointer` or `NULL`
 * @param	value	the value to append
 *
 * **Errors**
 * - ENOMEM: Out of memory; set via `da_reserve()`.
 *
 * @see	`da_reserve()`
 */
#define da_append(arr, value)                                                 \
do {                                                                          \
	__typeof__(*arr) tmp = value;                                         \
	da_append_((void**)&arr, &tmp, sizeof(tmp));                          \
} while (0)
void da_append_(void** arr, void* value, size_t elem_size);

#endif /* DA_H */
