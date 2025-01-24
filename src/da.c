#include "da.h"

#include <string.h>
#include <stdlib.h>

#define DA_INITIAL_CAP 2
#define DA_SCALE_FACTOR 1.5
#define DA_BIAS 8

#define da_var_size(da) ((size_t*)(da))[-1]
#define da_var_capacity(da) ((size_t*)(da))[-2]

#define da_head_to_data(p) (size_t*)(p) + 2
#define da_data_to_head(p) (size_t*)(p) - 2

/*///////////////////////////////////////////////////////////////////////////*/
/* DynamicArray                                                              */
/*///////////////////////////////////////////////////////////////////////////*/

void* da_init(size_t sz) {
	void* tmp = NULL;

	if (sz == 0) {
		return NULL;
	}

	tmp = malloc(2*sizeof(size_t) + DA_INITIAL_CAP*sz);
	if (tmp == NULL) {
		return NULL;
	}

	tmp = da_head_to_data(tmp);
	da_var_size(tmp) = 0;
	da_var_capacity(tmp) = DA_INITIAL_CAP;

	return tmp;
}

void da_free_(void* da) {
	if (da == NULL) {
		return;
	}

	free(da_data_to_head(da));
}

void da_assign_(void** da, void* src, size_t cnt, size_t sz) {
	if (*da == NULL) {
		*da = da_init(sz);
	}

	if (cnt >= da_capacity(*da)) {
		da_reserve_(da, cnt, sz);
	}

	memcpy(*da, src, cnt * sz);
	da_var_size(*da) = cnt;
}

/*///////////////////////////////////////////////////////////////////////////*/
/* Element Access                                                            */
/*///////////////////////////////////////////////////////////////////////////*/

void* da_at_(void* da, size_t idx, size_t sz) {
	if (da == NULL) {
		return NULL;
	}

	if (idx >= da_size(da)) {
		return NULL;
	}

	return (char*)da + (idx * sz);
}

/*///////////////////////////////////////////////////////////////////////////*/
/* Capacity                                                                  */
/*///////////////////////////////////////////////////////////////////////////*/

size_t da_size_(void* da) {
	if (da == NULL) {
		return 0;
	}

	return da_var_size(da);
}

size_t da_capacity_(void* da) {
	if (da == NULL) {
		return 0;
	}

	return da_var_capacity(da);
}

void da_reserve_(void** da, size_t cnt, size_t sz) {
	void* tmp;
	size_t new_size;

	if (*da == NULL) {
		*da = da_init(sz);
	}

	new_size = cnt * sz + (2 * sizeof(size_t));
	tmp = realloc(da_data_to_head(*da), new_size);
	if (tmp == NULL) {
		return;
	}
	*da = da_head_to_data(tmp);
	da_var_capacity(*da) = cnt;
}

/*///////////////////////////////////////////////////////////////////////////*/
/* Modifiers                                                                 */
/*///////////////////////////////////////////////////////////////////////////*/

void da_clear_(void* da) {
	if (da == NULL) {
		return;
	}

	da_var_size(da) = 0;
}

void da_insert_(void** da, size_t idx, void* val, size_t sz) {
	void* dst;
	void* src;

	if (*da == NULL) {
		*da = da_init(sz);
	}

	if (idx >= da_capacity(*da)) {
		return;
	}

	if (da_size(*da) == da_capacity(*da)) {
		size_t cnt = da_capacity(*da) * DA_SCALE_FACTOR + DA_BIAS;
		da_reserve_(da, cnt, sz);
	}

	/* shift elements */
	if (idx <= da_size(*da)) {
		dst = (char*)*da + sz * (idx + 1);
		src = (char*)*da + sz * idx;
		memmove(dst, src, (da_size(*da) - idx) * sz);
	}

	dst = (char*)*da + sz * idx;
	memcpy(dst, val, sz);
	++(da_var_size(*da));
}

void da_erase_(void** da, size_t idx, size_t sz) {
	void* dst;
	void* src;

	if (*da == NULL) {
		return;
	}

	if (idx >= da_size(*da)) {
		return;
	}

	if (da_size(*da) == da_capacity(*da)) {
		size_t cnt = da_capacity(*da) * DA_SCALE_FACTOR + DA_BIAS;
		da_reserve_(da, cnt, sz);
	}

	/* shift elements */
	if (idx <= da_size(*da)) {
		dst = (char*)*da + sz * idx;
		src = (char*)*da + sz * (idx + 1);
		memmove(dst, src, (da_size(*da) - (idx + 1)) * sz);
	}

	--(da_var_size(*da));
}

void da_append_(void** da, void* val, size_t sz) {
	void* dst;

	if (*da == NULL) { *da = da_init(sz); }

	if (da_size(*da) == da_capacity(*da)) {
		size_t cnt = da_capacity(*da) * DA_SCALE_FACTOR + DA_BIAS;
		da_reserve_(da, cnt, sz);
	}

	dst = (char*)*da + sz * da_size(*da);
	memcpy(dst, val, sz);
	++(da_var_size(*da));
}
