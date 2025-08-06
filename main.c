#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "linc.h"

#define ARRAY_SIZE 10

// Driver code
int test_func(int value)
{
	return value + 3;
}

int main(void)
{
	int array[ARRAY_SIZE] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int another_array[ARRAY_SIZE - 2] = { 0, 1, 2, 3, 4, 5, 6, 7 };

	LincArray array_one = { array, ARRAY_SIZE };
	LincArray array_two = { another_array, ARRAY_SIZE - 2 };

	LincArray* input[2] = { &array_one, &array_two };

	LincArray* result = linc_select(&array_one, test_func);

	for (size_t i = 0; i < result->size; i++)
	{
		printf("%i\n", result->array[i]);
	}

	linc_free(result);

	return 0;
}
