#ifndef DA_H
#define DA_H

/**
 * @file
 *
 * @see	https://en.cppreference.com/w/cpp/container/vector
 */

#include <stdlib.h>

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

void da_destroy(da_type* da);

/*///////////////////////////////////////////////////////////////////////////*/
/* Element Access                                                            */
/*///////////////////////////////////////////////////////////////////////////*/

/* void* da_at(da_type* da, size_t index); */

void* da_data(da_type* da);

/* void* da_front(da_type* da); */

void* da_back(da_type* da);

/*///////////////////////////////////////////////////////////////////////////*/
/* Capacity                                                                  */
/*///////////////////////////////////////////////////////////////////////////*/

size_t da_capacity(da_type* da);

size_t da_size(da_type* da);

void da_reserve(da_type* da, size_t new_cap);


/*///////////////////////////////////////////////////////////////////////////*/
/* Modifiers                                                                 */
/*///////////////////////////////////////////////////////////////////////////*/

void da_append(da_type* da, void* value);

#endif /* DA_H */
