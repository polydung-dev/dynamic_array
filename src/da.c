#include "da.h"

#include <string.h> /* memcpy */

#define DA_INITIAL_CAP 2
#define DA_SCALE_FACTOR 1.5
#define DA_BIAS 1

struct DynamicArray {
	void* data;
	size_t size;
	size_t capacity;
	size_t elem_size;
};

/*///////////////////////////////////////////////////////////////////////////*/
/* DynamicArray                                                              */
/*///////////////////////////////////////////////////////////////////////////*/

da_type* da_create(size_t elem_size) {
	da_type* da = malloc(sizeof(*da));

	da->data = malloc(DA_INITIAL_CAP * elem_size);
	da->size = 0;
	da->capacity = DA_INITIAL_CAP;
	da->elem_size = elem_size;

	return da;
}

void da_destroy(da_type* da) {
	free(da->data);
	free(da);
}

/*///////////////////////////////////////////////////////////////////////////*/
/* Element Access                                                            */
/*///////////////////////////////////////////////////////////////////////////*/

void* da_data(da_type* da) {
	return da->data;
}

void* da_back(da_type* da) {
	return (char*)da->data + ((da->size - 1) * da->elem_size);
}

/*///////////////////////////////////////////////////////////////////////////*/
/* Capacity                                                                  */
/*///////////////////////////////////////////////////////////////////////////*/

size_t da_capacity(da_type* da) {
	return da->capacity;
}

size_t da_size(da_type* da) {
	return da->size;
}

void da_reserve(da_type* da, size_t new_cap) {
	da->data = realloc(da->data, new_cap * da->elem_size);
	da->capacity = new_cap;
}

/*///////////////////////////////////////////////////////////////////////////*/
/* Modifiers                                                                 */
/*///////////////////////////////////////////////////////////////////////////*/


void da_append(da_type* da, void* value) {
	void* dst = NULL;

	if (da->size == da->capacity) {
		da_reserve(da, da->capacity * DA_SCALE_FACTOR + DA_BIAS);
	}

	dst = (char*)da->data + (da->size * da->elem_size);
	memcpy(dst, value, da->elem_size);
	++da->size;
}
