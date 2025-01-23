#include "da.h"

#include <string.h>
#include <stdlib.h>

#define DA_INITIAL_CAP 1
#define DA_SCALE_FACTOR 1.5
#define DA_BIAS 1

/*///////////////////////////////////////////////////////////////////////////*/
/* DynamicArray                                                              */
/*///////////////////////////////////////////////////////////////////////////*/

void* da_init(size_t elem_size) {
	void* tmp = malloc(2*sizeof(size_t) + DA_INITIAL_CAP*elem_size);
	size_t cap = DA_INITIAL_CAP;
	size_t size = 0;
	memcpy(tmp, &cap, sizeof(size_t));
	memcpy((size_t*)tmp + 1, &size, sizeof(size_t));
	return (size_t*)tmp + 2;
}

void da_free(void* arr) {
	free((size_t*)arr - 2);
}

/*///////////////////////////////////////////////////////////////////////////*/
/* Element Access                                                            */
/*///////////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////////////*/
/* Capacity                                                                  */
/*///////////////////////////////////////////////////////////////////////////*/

size_t da_size_(void* arr) {
	return ((size_t*)(arr))[-1];
}

size_t da_capacity_(void* arr) {
	return ((size_t*)(arr))[-2];
}

void* da_reserve_(void* da, size_t new_cap, size_t elem_size) {
	void* p = (size_t*)da - 2;
	size_t new_sz = new_cap * elem_size + (2 * sizeof(size_t));
	void* tmp = realloc(p, new_sz);

	memcpy(tmp, &new_cap, sizeof(size_t));

	return (size_t*)tmp + 2;
}

/*///////////////////////////////////////////////////////////////////////////*/
/* Modifiers                                                                 */
/*///////////////////////////////////////////////////////////////////////////*/

void da_append_(void** arr, void* value, size_t elem_size) {
	void* dst;

	if (*arr == NULL) {
		*arr = da_init(elem_size);
	}

	if (da_size(*arr) == da_capacity(*arr)) {
		*arr = da_reserve_(*arr, da_capacity(*arr) * 2, elem_size);
	}

	dst = (char*)*arr + elem_size * da_size(*arr);
	memcpy(dst, value, elem_size);
	++(((size_t*)(*arr))[-1]);
}
