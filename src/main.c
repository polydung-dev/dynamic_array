#include <stdio.h>

#include "da.h"

#define PRINT_ARRAY(fmt, da)                                                  \
do {                                                                          \
	printf("[");                                                          \
	for (size_t i = 0; i < da_size(da); ++i) {                            \
		printf(fmt, (da)[i]);                                         \
		if (i + 1 < da_size(da)) {                                    \
			printf(", ");                                         \
		}                                                             \
	}                                                                     \
	printf("]\n");                                                        \
} while (0)

#define DEBUG_DUMP(da)                                                        \
do {                                                                          \
	if (da == NULL) { break; };                                           \
	printf("data     == %p\n",  (void*)da);                               \
	printf("size     == %zu\n", da_size(da));                             \
	printf("capacity == %zu\n", da_capacity(da));                         \
} while (0)

int main(void) {
	printf("=====\n");
	int* arr = NULL;
	DEBUG_DUMP(arr);

	printf("-----\n");
	da_append(arr, 69);
	DEBUG_DUMP(arr);
	PRINT_ARRAY("%i", arr);

	printf("-----\n");
	da_append(arr, 420);
	DEBUG_DUMP(arr);
	PRINT_ARRAY("%i", arr);

	da_free(arr);
	return 0;
}
