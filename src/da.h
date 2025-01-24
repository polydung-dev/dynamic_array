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
 *
 * **Common Parameters**
 *
 * |       |                                         |
 * |-------|-----------------------------------------|
 * | `da`  | a data pointer                          |
 * | `idx` | an index into an array                  |
 * | `val` | a value of an element                   |
 * | `cnt` | a number of elements                    |
 * | `sz`  | a number of bytes (width of an element) |
 */

/*///////////////////////////////////////////////////////////////////////////*/
/* DynamicArray                                                              */
/*///////////////////////////////////////////////////////////////////////////*/

/**
 * Initialises the array + header.
 *
 * Note: The array can also be "initialised" by setting the pointer to `NULL`
 *
 * @returns	on success	a pointer to an array
 * @returns	on failure	NULL
 *
 * **Errors**
 * - ENOMEM: Out of memory; set via `malloc`.
 *
 * @see	`da_free()`
 */
void* da_init(size_t sz);

/**
 * Free's the array (and sets the pointer to `NULL`).
 *
 * @param	da	a valid `data pointer` (MAY be `NULL`)
 *
 * @see	`da_init()`
 */
#define da_free(da)                                                           \
do {                                                                          \
	da_free_(da);                                                         \
	(da) = NULL;                                                          \
} while (0)
void da_free_(void* da);

/**
 * Copies `cnt` elements from the `src` array.
 *
 * Allocates memory as required. If `da` == `NULL` the array is initialised.
 *
 * @param	da	a valid `data pointer` (MAY be `NULL`)
 * @param	src	pointer to an array of at least `cnt` elements
 * @param	cnt	number of elements in the `src` array
 *
 * **Errors**
 * - ENOMEM: Out of memory; set via `da_reserve()`.
 *
 * @see	`da_reserve()`
 */
#define da_assign(da, src, cnt)                                               \
do {                                                                          \
	__typeof__((da)) tmp = src;                                           \
	da_assign_((void**)&(da), tmp, cnt, sizeof(*tmp));                    \
} while (0)
void da_assign_(void** da, void* src, size_t cnt, size_t sz);

/*///////////////////////////////////////////////////////////////////////////*/
/* Element Access                                                            */
/*///////////////////////////////////////////////////////////////////////////*/

/**
 * First element in the array (lvalue).
 *
 * @param	da	a valid `data pointer` (MUST NOT be `NULL`)
 */
#define da_front(da) (da)[0]

/**
 * Last element in the array (lvalue).
 *
 * @param	da	a valid `data pointer` (MUST NOT be `NULL`)
 */
#define da_back(da) (da)[da_size(da) - 1]

/**
 * Returns a pointer to the value at the given index (with bounds checking).
 *
 * If `da` == `NULL`, `NULL` is returned.
 *
 * @param	da	a valid `data pointer` (MAY be `NULL`)
 *
 * @returns	on success	a pointer to an element in the array
 * @returns	on failure	`NULL`
 */
#define da_at(da, idx) \
	(__typeof__(da))da_at_((void*)(da), idx, sizeof(*da))
void* da_at_(void* da, size_t idx, size_t sz);

/*///////////////////////////////////////////////////////////////////////////*/
/* Capacity                                                                  */
/*///////////////////////////////////////////////////////////////////////////*/

/**
 * Returns the current number of elements in the array.
 *
 * @param	da	a valid `data pointer` (MAY be `NULL`)
 *
 * @returns	the current number of elements (may be `0`)
 * @returns	`0` if `da` == `NULL`
 */
#define da_size(da) da_size_(da)
size_t da_size_(void* da);

/**
 * Returns the maximum number of elements that the array can hold.
 *
 * Note: Increase the array cpacity with `da_reserve()`
 *
 * @param	da	a valid `data pointer` (MAY be `NULL`)
 *
 * @returns	the maximum number of elements (may be `0`)
 * @returns	`0` if `da` == `NULL`
 *
 * @see	`da_reserve()`
 */
#define da_capacity(da) da_capacity_(da)
size_t da_capacity_(void* da);

/**
 * Reserves additional memory for the array.
 *
 * If `da` == `NULL` the array is initialised.
 *
 * @param	da	a valid `data pointer` (MAY be `NULL`)
 * @param	cnt	number of elements to reserve space for
 *
 * **Errors**
 * - ENOMEM: Out of memory; set via `realloc`.
 */
#define da_reserve(da, cnt) \
	da_reserve_((void**)&(da), cnt, sizeof(*da))
void da_reserve_(void** da, size_t cnt, size_t sz);

/*///////////////////////////////////////////////////////////////////////////*/
/* Modifiers                                                                 */
/*///////////////////////////////////////////////////////////////////////////*/

/**
 * Sets the array size to 0, capacity remains unchanged.
 *
 * If `da` == `NULL`, does nothing.
 *
 * @param	da	a valid `data pointer` (MAY be `NULL`)
 */
#define da_clear(da) da_clear_((void*)(da))
void da_clear_(void* da);

/**
 * Copies the value into the array at the given index, allocating additional
 * memory if required.
 *
 * If `da` == `NULL` the array is initialised.
 * If `idx` >= `da_capacity(da)`, does nothing.
 *
 * @param	da	a valid `data pointer` (MAY be `NULL`)
 * @param	idx	index into the array
 * @param	val	the value to copy
 *
 * **Errors**
 * - ENOMEM: Out of memory; set via `da_reserve()`.
 *
 * @see	`da_reserve()`
 * @see	`da_append()`
 */
#define da_insert(da, idx, val)                                               \
do {                                                                          \
	__typeof__(*da) tmp = val;                                            \
	da_insert_((void**)&(da), idx, &tmp, sizeof(tmp));                    \
} while (0)
void da_insert_(void** da, size_t idx, void* val, size_t sz);

/**
 * Removes an element from the array.
 *
 * If `da` == `NULL`, does nothing.
 * If `idx` >= `da_size(da)`, does nothing.
 *
 * @param	da	a valid `data pointer` (MAY be `NULL`)
 * @param	idx	index into the array
 */
#define da_erase(da, idx) da_erase_((void**)&(da), idx, sizeof(*da))
void da_erase_(void** da, size_t idx, size_t sz);

/**
 * Copies the value to the end of the array, reallocating if required.
 *
 * If `da` == `NULL` the array is initialised.
 *
 * @param	da	a valid `data pointer` (MAY be `NULL`)
 * @param	val	the value to copy
 *
 * **Errors**
 * - ENOMEM: Out of memory; set via `da_reserve()`.
 *
 * @see	`da_reserve()`
 */
#define da_append(da, val)                                                    \
do {                                                                          \
	__typeof__(*da) tmp = val;                                            \
	da_append_((void**)&da, &tmp, sizeof(tmp));                           \
} while (0)
void da_append_(void** da, void* val, size_t sz);

#endif /* DA_H */
