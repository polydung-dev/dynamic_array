#include "da.h"

#include <string.h> /* memcpy */

#define DA_INITIAL_CAP 2
#define DA_SCALE_FACTOR 1.5
#define DA_BIAS 1

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

void da_assign_(da_type* da, void* arr, size_t sz) {
	if (da->data != NULL) {
		da_clear(da);
	}

	if (sz >= da->capacity) {
		da_reserve(da, sz * da->elem_size);
	}

	memcpy(da->data, arr, sz * da->elem_size);
	da->size = sz;
}

/*///////////////////////////////////////////////////////////////////////////*/
/* Element Access                                                            */
/*///////////////////////////////////////////////////////////////////////////*/

void* da_data_(da_type* da) {
	return da->data;
}

void* da_at_(da_type* da, size_t index) {
	if (index >= da->size) {
		return NULL;
	}

	return (char*)da->data + (index * da->elem_size);
}

/*///////////////////////////////////////////////////////////////////////////*/
/* Capacity                                                                  */
/*///////////////////////////////////////////////////////////////////////////*/

size_t da_size_(da_type* da) {
	return da->size;
}

size_t da_capacity_(da_type* da) {
	return da->capacity;
}

void da_reserve(da_type* da, size_t new_cap) {
	da->data = realloc(da->data, new_cap * da->elem_size);
	da->capacity = new_cap;
}

/*///////////////////////////////////////////////////////////////////////////*/
/* Modifiers                                                                 */
/*///////////////////////////////////////////////////////////////////////////*/

void da_clear_(da_type* da) {
	free(da->data);
	da->data = NULL;
	da->size = 0;
}

void da_insert_(da_type* da, size_t index, void* value) {
	void* src = NULL;
	void* dst = NULL;
	size_t sz = 0;

	/* reserve as required */
	if (da->size == da->capacity) {
		da_reserve(da, da->capacity * DA_SCALE_FACTOR + DA_BIAS);
	}

	/* shift elements up */
	if (index <= da->size) {
		dst = (char*)da->data + ((index + 1) * da->elem_size);
		src = (char*)da->data + (index * da->elem_size);
		sz = (da->size - index) * da->elem_size;
		memmove(dst, src, sz);
	}

	/* insert new elements */
	dst = (char*)da->data + (index * da->elem_size);
	memcpy(dst, value, da->elem_size);
	++da->size;
}

void da_erase_(da_type* da, size_t index) {
	void* src = NULL;
	void* dst = NULL;
	size_t sz = 0;

	/* shift elements down */
	if (index <= da->size) {
		dst = (char*)da->data + (index * da->elem_size);
		src = (char*)da->data + ((index + 1) * da->elem_size);
		sz = (da->size - (index + 1)) * da->elem_size;

		memmove(dst, src, sz);
	}

	/* delete last element */
	/* dst = (char*)da->data + ((da->size - 1) * da->elem_size);
	memset(dst, 0, da->elem_size); */
	--da->size;
}
