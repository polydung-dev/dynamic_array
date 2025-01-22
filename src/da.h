#ifndef DA_H
#define DA_H

/**
 * @file
 *
 * @see	https://en.cppreference.com/w/cpp/container/vector
 */

#include <stdlib.h>
#include <stdbool.h>

/**
 * Typed object.
 */
#define da(type)                                                              \
struct DynamicArray_##type {                                                  \
	type* data;                                                           \
	size_t size;                                                          \
	size_t capacity;                                                      \
	size_t elem_size;                                                     \
}

/**
 * Generic object.
 */
typedef da(void) da_type;

/*///////////////////////////////////////////////////////////////////////////*/
/* DynamicArray                                                              */
/*///////////////////////////////////////////////////////////////////////////*/

/**
 * Initialises the dynamic array object and allocates initial buffer.
 *
 * @param	da	an uninitialised dynamic array object
 *
 * @see	`da_free()`
 */
#define da_init(da, type) da(type)* da = (void*)da_create(sizeof(*da->data))
da_type* da_create(size_t elem_size);

/**
 * Free's the internal array and "zero"'s the object.
 *
 * @param	da	a valid dynamic array object
 *
 * @see	`da_init()`
 */
#define da_free(da) da_destroy((void*)da)
void da_destroy(da_type* da);

/**
 * Copies `sz` elements from `arr` into the array.
 *
 * @param	da	a valid dynamic array object
 * @param	arr	a pointer to an array with at least `sz` elements
 * @param	sz	number of elements to copy
 */
#define da_assign(da, arr, sz)                                                \
do {                                                                          \
	__typeof__((da)->data) tmp = arr;                                     \
	da_assign_((void*)da, tmp, sz);                                       \
} while (0)
void da_assign_(da_type* da, void* arr, size_t sz);

/*///////////////////////////////////////////////////////////////////////////*/
/* Element Access                                                            */
/*///////////////////////////////////////////////////////////////////////////*/

/**
 * Returns the first value in the array.
 *
 * @param	da	a valid dynamic array object
 */
#define da_front(da) (da)->data[0]

/**
 * Returns the last value in the array.
 *
 * @param	da	a valid dynamic array object
 */
#define da_back(da) (da)->data[(da)->size - 1]

/**
 * Returns the underlying array.
 *
 * @param	da	a valid dynamic array object
 */
#define da_data(da) ((__typeof__((da)->data))(da_data_((void*)(da))))
void* da_data_(da_type* da);

/**
 * Returns a pointer to the value at the given index (bounds checked).
 *
 * A pointer is returned instead of the value to act as an "optional" type as
 * C does not support exceptions.
 *
 * @param	da	a pointer to a valid object
 * @param	index	array index to fetch [0, da->size)
 *
 * @returns	on success	a pointer to an element
 * @returns	on failure	NULL
 */
#define da_at(da, index) ((__typeof((da)->data))(da_at_((void*)(da), index)))
void* da_at_(da_type* da, size_t index);

/*///////////////////////////////////////////////////////////////////////////*/
/* Capacity                                                                  */
/*///////////////////////////////////////////////////////////////////////////*/

/**
 * Checks if the array is empty.
 *
 * @param	da	a valid dynamic array object
 */
#define da_empty(da) ((da)->size == 0)

/**
 * Returns the number of elements in the array.
 *
 * @param	da	a valid dynamic array object
 */
#define da_size(da) da_size_((void*)(da))
size_t da_size_(da_type* da);
/**
 * Returns the current capacity of the array.
 *
 * @param	da	a valid dynamic array object
 */
#define da_capacity(da) da_capacity_((void*)(da))
size_t da_capacity_(da_type* da);
/**
 * Allocates additional memory. Called automatically as required.
 *
 * @param	da	a pointer to a valid object
 * @param	new_cap	new capacity of the array
 *
 * @see	`da_append()`
 */
void da_reserve(da_type* da, size_t new_cap);

/*///////////////////////////////////////////////////////////////////////////*/
/* Modifiers                                                                 */
/*///////////////////////////////////////////////////////////////////////////*/

/**
 * Clears the array, but does not resize.
 *
 * @param	da	a valid dynamic array object
 */
#define da_clear(da) da_clear_((void*)(da))
void da_clear_(da_type* da);

/**
 * Inserts (copies) a value into the array at the given index.
 *
 * @param	da	a valid dynamic array object
 * @param	index	array index to fetch [0, da->size)
 * @param	value	a pointer to the value to add
 */
#define da_insert(da, index, value)                                           \
do {                                                                          \
	__typeof__((da)->data[0]) tmp = value;                                \
	da_insert_((void*)(da), index, &tmp);                                 \
} while (0)
void da_insert_(da_type* da, size_t index, void* value);

/**
 * Removes a value from the array at the given index.
 *
 * @param	da	a valid dynamic array object
 * @param	index	array index to fetch [0, da->size)
 */
#define da_erase(da, index) da_erase_((void*)(da), index)
void da_erase_(da_type* da, size_t index);

/**
 * Appends (copies) a value to the end of the array.
 *
 * @param	da	a valid dynamic array object
 * @param	value	a pointer to the value to add
 */
#define da_append(da, value)                                                  \
do {                                                                          \
	__typeof__((da)->data[0]) tmp = value;                                \
	da_insert_((void*)(da), (da)->size, &tmp);                            \
} while (0)

#endif /* DA_H */
