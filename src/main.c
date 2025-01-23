#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

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

int64_t sum_array(int* arr, size_t count);

void test_1(void);

int main(void) {
	test_1();

	return 0;
}

int64_t sum_array(int* arr, size_t count) {
	int64_t sum = 0;
	for (size_t i = 0; i < count; ++i) {
		sum += arr[i];
	}
	return sum;
}

void test_1(void) {
	printf("== Test 1 ================================================\n");
	int* arr = NULL;
	DEBUG_DUMP(arr);

	printf("-- da_append; variable -----------------------------------\n");
	int n = 69;
	da_append(arr, n);
	DEBUG_DUMP(arr);
	PRINT_ARRAY("%i", arr);

	printf("-- da_append; immediate (with realloc) -------------------\n");
	da_append(arr, 420);
	da_append(arr, 777);
	DEBUG_DUMP(arr);
	PRINT_ARRAY("%i", arr);

	printf("-- calling regular function ------------------------------\n");
	int64_t sum = sum_array(arr, da_size(arr));
	printf("sum == %"PRId64"\n", sum);

	da_free(arr);
}
