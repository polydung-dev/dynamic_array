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
 * Opaque type.
 */
typedef struct DynamicArray da_type;

/*///////////////////////////////////////////////////////////////////////////*/
/* DynamicArray                                                              */
/*///////////////////////////////////////////////////////////////////////////*/

/**
 * Initialises the dynamic array object and allocates initial buffer.
 *
 * @param	elem_size	size of the element i.e. sizeof(element type)
 *
 * @returns	on success	a pointer to a valid object
 * @returns	on failure	NULL
 *
 * @see	`da_destroy()`
 */
da_type* da_create(size_t elem_size);

/**
 * Free's the internal array and destoys the object.
 *
 * @param	da	a pointer to a valid object
 *
 * @see	`da_create()`
 */
void da_destroy(da_type* da);

void da_assign(da_type* da, void* ptr, size_t sz);

/*///////////////////////////////////////////////////////////////////////////*/
/* Element Access                                                            */
/*///////////////////////////////////////////////////////////////////////////*/

/**
 * Returns a pointer to the value at the given index (bounds checked).
 *
 * @param	da	a pointer to a valid object
 * @param	index	array index to fetch [0, da->size)
 *
 * @returns	on success	a pointer to an element
 * @returns	on failure	NULL
 */
void* da_at(da_type* da, size_t index);

/**
 * Returns a pointer to the first value in the array.
 *
 * @param	da	a pointer to a valid object
 *
 * @returns	on success	a pointer to an element
 * @returns	on failure	NULL
 */
void* da_front(da_type* da);

/**
 * Returns a pointer to the last value in the array.
 *
 * @param	da	a pointer to a valid object
 *
 * @returns	on success	a pointer to an element
 * @returns	on failure	NULL
 */
void* da_back(da_type* da);

/**
 * Returns a pointer to the underlying array.
 *
 * @param	da	a pointer to a valid object
 *
 * @returns	on success	a pointer to the array
 * @returns	on failure	NULL
 */
void* da_data(da_type* da);

/**
 * `get_at()` with "automatic" type conversion.
 *
 * @see	`get_at()`
 */
#define da_get_as(da, index, type) *(type*)da_at(da, index)

/*///////////////////////////////////////////////////////////////////////////*/
/* Capacity                                                                  */
/*///////////////////////////////////////////////////////////////////////////*/

/**
 * Checks if the array is empty.
 *
 * @param	da	a pointer to a valid object
 *
 * @returns	(da->size == 0)
 */
bool da_empty(da_type* da);

/**
 * Returns the number of elements in the array.
 *
 * @param	da	a pointer to a valid object
 *
 * @returns	the number of elements in the array
 */
size_t da_size(da_type* da);

/**
 * Returns the current capacity of the array.
 *
 * @param	da	a pointer to a valid object
 *
 * @returns	the current maximum number of elements that the array can hold
 */
size_t da_capacity(da_type* da);

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
 * @param	da	a pointer to a valid object
 */
void da_clear(da_type* da);

/**
 * Inserts (copies) a value into the array at the given index.
 *
 * @param	da	a pointer to a valid object
 * @param	index	array index to fetch [0, da->size)
 * @param	value	a pointer to the value to add
 */
void da_insert(da_type* da, size_t index, void* value);

/**
 * Removes a value from the array at the given index.
 *
 * @param	da	a pointer to a valid object
 * @param	index	array index to fetch [0, da->size)
 */
void da_erase(da_type* da, size_t index);

/**
 * Appends (copies) a value to the end of the array.
 *
 * @param	da	a pointer to a valid object
 * @param	value	a pointer to the value to add
 */
#define da_append(da, value) da_insert(da, da_size(da), value)

#endif /* DA_H */
