#include <stdio.h>

#include "da.h"

#define PRINT_ARRAY(da, fmt, type)                                            \
do {                                                                          \
	printf("[");                                                          \
	for (size_t i = 0; i < da_size(da); ++i) {                            \
		printf(fmt, ((type*)da_data(da))[i]);                         \
		if (i + 1 < da_size(da)) {                                    \
			printf(", ");                                         \
		}                                                             \
	}                                                                     \
	printf("]\n");                                                        \
} while (0)

#define DEBUG_DUMP(da)                                                        \
do {                                                                          \
	printf("--------------------------\n");                               \
	printf("data     == %p\n",  da_data(da));                             \
	printf("size     == %zu\n", da_size(da));                             \
	printf("capacity == %zu\n", da_capacity(da));                         \
} while (0)

#define da_append_literal(da, val, type) da_append(da, &(type){val})

int main(void) {
	da_type* da = da_create(sizeof(int));
	DEBUG_DUMP(da);
	PRINT_ARRAY(da, "%i", int);

	/* append varaible */
	int v = 69;
	da_append(da, &v);
	DEBUG_DUMP(da);
	PRINT_ARRAY(da, "%i", int);

	/* append "immediate" via compound literal */
	da_append(da, &(int){420});
	da_append_literal(da, 777, int);
	DEBUG_DUMP(da);
	PRINT_ARRAY(da, "%i", int);

	int last = *(int*)da_back(da);
	printf("--------------------------\n");
	printf("last element == %i\n", last);

	da_destroy(da);

	return 0;
}
