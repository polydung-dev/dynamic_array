#include <stdio.h>

#include "da.h"

#define PRINT_ARRAY(fmt, da)                                                  \
do {                                                                          \
	printf("[");                                                          \
	for (size_t i = 0; i < da_size(da); ++i) {                            \
		printf(fmt, da_data(da)[i]);                                  \
		if (i + 1 < da_size(da)) {                                    \
			printf(", ");                                         \
		}                                                             \
	}                                                                     \
	printf("]\n");                                                        \
} while (0)

#define DEBUG_DUMP(da)                                                        \
do {                                                                          \
	printf("data     == %p\n",  (void*)da_data(da));                      \
	printf("size     == %zu\n", da_size(da));                             \
	printf("capacity == %zu\n", da_capacity(da));                         \
} while (0)

void test_1(void);
void test_2(void);
void test_3(void);

int main(void) {
	test_1();
	test_2();
	test_3();

	return 0;
}

void test_1(void) {
	da_init(da, int);

	printf("==========================\n");
	DEBUG_DUMP(da);
	PRINT_ARRAY("%i", da);

	/* append varaible */
	int v = 69;
	da_append(da, v);
	printf("--------------------------\n");
	DEBUG_DUMP(da);
	PRINT_ARRAY("%i", da);

	/* append "immediate" */
	da_append(da, 420);
	da_append(da, 777);
	printf("--------------------------\n");
	DEBUG_DUMP(da);
	PRINT_ARRAY("%i", da);

	int last = da_back(da);
	printf("--------------------------\n");
	printf("last element == %i\n", last);

	da_free(da);
}

void test_2(void) {
	da_init(da, char);

	printf("==========================\n");
	DEBUG_DUMP(da);
	PRINT_ARRAY("%c", da);

	da_assign(da, "SPA", 3);
	printf("--------------------------\n");
	DEBUG_DUMP(da);
	PRINT_ARRAY("%c", da);

	da_insert(da, da_size(da), 'M');
	printf("--------------------------\n");
	DEBUG_DUMP(da);
	PRINT_ARRAY("%c", da);

	da_back(da) = 'S';
	printf("--------------------------\n");
	DEBUG_DUMP(da);
	PRINT_ARRAY("%c", da);

	da_append(da, da_front(da));
	printf("--------------------------\n");
	DEBUG_DUMP(da);
	PRINT_ARRAY("%c", da);

	da_front(da) = 'P';
	printf("--------------------------\n");
	DEBUG_DUMP(da);
	PRINT_ARRAY("%c", da);

	da_erase(da, 1);
	printf("--------------------------\n");
	DEBUG_DUMP(da);
	PRINT_ARRAY("%c", da);

	da_free(da);
}

void test_3(void) {
	da_init(da, char);

	printf("==========================\n");
	DEBUG_DUMP(da);
	PRINT_ARRAY("'%c'", da);

	char str[] = "dead beef";
	da_assign(da, str, sizeof(str));
	printf("--------------------------\n");
	DEBUG_DUMP(da);
	PRINT_ARRAY("'%c'", da);

	da_free(da);
}
