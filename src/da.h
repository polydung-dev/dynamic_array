#ifndef DA_H
#define DA_H

#include <stddef.h>

/**
 * @file
 *
 * This is a "drop in" dynamic array which "hides" the metadata before the
 * array pointer.
 *
 * +------+------+----------+
 * | cap  | size | array    |
 * +------+------+----------+
 *
 * Manually calling `da_init` is not required; As long as the pointer is
 * initialised to `NULL`, calling `da_append` will work fine.
 */

/*///////////////////////////////////////////////////////////////////////////*/
/* DynamicArray                                                              */
/*///////////////////////////////////////////////////////////////////////////*/

void* da_init(size_t elem_size);
void da_free(void* arr);

/*///////////////////////////////////////////////////////////////////////////*/
/* Element Access                                                            */
/*///////////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////////////*/
/* Capacity                                                                  */
/*///////////////////////////////////////////////////////////////////////////*/

#define da_size(arr) da_size_(arr)
size_t da_size_(void* arr);

#define da_capacity(arr) da_capacity_(arr)
size_t da_capacity_(void* arr);

void* da_reserve_(void* da, size_t new_cap, size_t elem_size);

/*///////////////////////////////////////////////////////////////////////////*/
/* Modifiers                                                                 */
/*///////////////////////////////////////////////////////////////////////////*/

#define da_append(arr, val)                                                   \
do {                                                                          \
	__typeof__(*arr) tmp = val;                                           \
	da_append_((void**)&arr, &tmp, sizeof(tmp));                          \
} while (0)
void da_append_(void** arr, void* value, size_t elem_size);

#endif /* DA_H */
