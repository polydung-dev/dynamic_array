#include "da.h"

#include <string.h>
#include <stdlib.h>

#define DA_INITIAL_CAP 2
#define DA_SCALE_FACTOR 1.5
#define DA_BIAS 8

/*///////////////////////////////////////////////////////////////////////////*/
/* DynamicArray                                                              */
/*///////////////////////////////////////////////////////////////////////////*/

void* da_init(size_t elem_size) {
	void* tmp = NULL;
	size_t cap = DA_INITIAL_CAP;
	size_t size = 0;

	tmp = malloc(2*sizeof(size_t) + DA_INITIAL_CAP*elem_size);
	if (tmp == NULL) {
		return NULL;
	}
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
	if (arr == NULL) { return 0; }

	return ((size_t*)(arr))[-1];
}

size_t da_capacity_(void* arr) {
	if (arr == NULL) { return 0; }

	return ((size_t*)(arr))[-2];
}

void da_reserve_(void** arr, size_t count, size_t elem_size) {
	void* tmp;
	size_t new_size;

	if (*arr == NULL) { *arr = da_init(elem_size); }

	new_size = count * elem_size + (2 * sizeof(size_t));
	tmp = realloc((size_t*)(*arr) - 2, new_size);
	if (tmp == NULL) {
		return;
	}
	memcpy(tmp, &count, sizeof(size_t));
	*arr = (size_t*)tmp + 2;
}

/*///////////////////////////////////////////////////////////////////////////*/
/* Modifiers                                                                 */
/*///////////////////////////////////////////////////////////////////////////*/

void da_append_(void** arr, void* value, size_t elem_size) {
	void* dst;

	if (*arr == NULL) { *arr = da_init(elem_size); }

	if (da_size(*arr) == da_capacity(*arr)) {
		size_t count = da_capacity(*arr) * DA_SCALE_FACTOR + DA_BIAS;
		da_reserve_(arr, count, elem_size);
	}

	dst = (char*)*arr + elem_size * da_size(*arr);
	memcpy(dst, value, elem_size);
	++(((size_t*)(*arr))[-1]);
}
