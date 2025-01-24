#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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
void test_2(void);
void test_3(void);

int main(void) {
	test_1();
	test_2();
	test_3();

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
	printf("== Test 1 : Demonstration. ===============================\n");
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
	assert(sum == 1266);

	printf("\n");
	da_free(arr);
}

void test_2(void) {
	printf("== Test 2 : General. =====================================\n");
	char* arr = NULL;
	DEBUG_DUMP(arr);

	printf("-- da_append; --------------------------------------------\n");
	da_append(arr, 'x');
	DEBUG_DUMP(arr);
	PRINT_ARRAY("'%c'", arr);
	assert(memcmp(arr, "x", 1) == 0);

	printf("-- da_assign; --------------------------------------------\n");
	da_assign(arr, "SPAM", 4);
	DEBUG_DUMP(arr);
	PRINT_ARRAY("'%c'", arr);
	assert(memcmp(arr, "SPAM", 4) == 0);

	printf("-- da_front; ---------------------------------------------\n");
	da_front(arr) = 'F';
	DEBUG_DUMP(arr);
	PRINT_ARRAY("'%c'", arr);
	assert(memcmp(arr, "FPAM", 4) == 0);

	printf("-- da_back; ----------------------------------------------\n");
	da_back(arr) = 'B';
	DEBUG_DUMP(arr);
	PRINT_ARRAY("'%c'", arr);
	assert(memcmp(arr, "FPAB", 4) == 0);

	printf("-- da_at; ------------------------------------------------\n");
	*da_at(arr, 1) = 'A';
	DEBUG_DUMP(arr);
	PRINT_ARRAY("'%c'", arr);
	assert(memcmp(arr, "FAAB", 4) == 0);

	printf("-- da_clear; ---------------------------------------------\n");
	da_clear(arr);
	DEBUG_DUMP(arr);
	PRINT_ARRAY("'%c'", arr);
	assert(da_size(arr) == 0);

	da_assign(arr, "1234", 4);

	printf("-- da_insert; --------------------------------------------\n");
	da_insert(arr, 0, 'A');
	DEBUG_DUMP(arr);
	PRINT_ARRAY("'%c'", arr);
	assert(memcmp(arr, "A1234", 5) == 0);

	printf("-- da_insert; --------------------------------------------\n");
	da_insert(arr, 3, 'B');
	DEBUG_DUMP(arr);
	PRINT_ARRAY("'%c'", arr);
	assert(memcmp(arr, "A12B34", 6) == 0);

	printf("-- da_insert; --------------------------------------------\n");
	da_insert(arr, da_size(arr), 'C');
	DEBUG_DUMP(arr);
	PRINT_ARRAY("'%c'", arr);
	assert(memcmp(arr, "A12B34C", 7) == 0);

	printf("-- da_erase; ---------------------------------------------\n");
	for (size_t i = 0; i < da_size(arr); /**/) {
		if (arr[i] == '2' || arr[i] == '3') {
			da_erase(arr, i);
		} else {
			++i;
		}
	}
	DEBUG_DUMP(arr);
	PRINT_ARRAY("'%c'", arr);
	assert(memcmp(arr, "A1B4C", 5) == 0);

	printf("-- printf; (%%.*s) ---------------------------------------\n");
	printf("`%.*s`\n", (int)da_size(arr), arr);

	printf("\n");
	da_free(arr);
}

void test_3(void) {
	printf("== Test 3 : Uninitialised. ===============================\n");
	char* arr = NULL;

	printf("-- da_init; ----------------------------------------------\n");
	arr = da_init(0);
	assert(arr == NULL);

	printf("-- da_free; ----------------------------------------------\n");
	arr = da_init(sizeof(*arr));
	da_free(arr);
	assert(arr == NULL);

	printf("-- da_assign; --------------------------------------------\n");
	da_assign(arr, "SPAM", 4);
	assert(memcmp(arr, "SPAM", 4) == 0);

	/* `da_front` cannot take `NULL` */
	/* `da_back`  cannot take `NULL` */

	printf("-- da_at; ------------------------------------------------\n");
	assert(da_at(arr, 69) == NULL); /* out of bounds */
	da_free(arr);

	assert(arr == NULL);
	assert(da_at(arr, 0) == NULL); /* arr == NULL */

	printf("-- da_size; ----------------------------------------------\n");
	assert(arr == NULL);
	assert(da_size(arr) == 0);

	printf("-- da_capacity; ------------------------------------------\n");
	assert(arr == NULL);
	assert(da_capacity(arr) == 0);

	printf("-- da_reserve; -------------------------------------------\n");
	assert(arr == NULL);
	da_reserve(arr, 10);
	assert(da_capacity(arr) == 10);
	da_free(arr);

	printf("-- da_clear; ---------------------------------------------\n");
	assert(arr == NULL);
	da_clear(arr);

	printf("-- da_insert; --------------------------------------------\n");
	assert(arr == NULL);
	da_insert(arr, 0, 'A'); /* arr == NULL */
	assert(memcmp(arr, "A", 1) == 0);
	da_insert(arr, 69, 'A'); /* out of bounds */
	assert(da_size(arr) == 1);

	printf("-- da_erase; ---------------------------------------------\n");
	da_erase(arr, 69); /* out of bounds */
	assert(da_size(arr) == 1);
	da_free(arr);
	da_erase(arr, 0); /* arr == NULL */

	printf("-- da_append; --------------------------------------------\n");
	da_append(arr, 'x');
	assert(memcmp(arr, "x", 1) == 0);

	printf("\n");
	da_free(arr);
}
