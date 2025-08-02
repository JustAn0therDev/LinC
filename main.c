#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 10

typedef struct linc_array
{
	int* array;
	size_t size;
} LincArray;

// Allocates whatever is needed of memory for a LincArray struct. The caller should be the one to handle the freeing.
LincArray* allocate_linc_array()
{
	LincArray* linc_result = (LincArray*)calloc(1, sizeof(LincArray));
	return linc_result;
}

// Frees the whole LincArray struct allocated.
void free_linc_array(LincArray* linc_result)
{
	if (linc_result == 0)
		return;	

	if (linc_result->array != 0)
	{
		free(linc_result->array);
	}

	free(linc_result);
}

// Linq Methods - The caller is always responsible for freeing the allocated LincArray.
// SelectMany - DONE
// Intersect - TODO
// OrderByDescending - TODO
// OrderBy - TODO
// Join - TODO
// Take - TODO
// TakeWhile - TODO
// Skip - TODO
// SkipWhile - TODO
// Distinct - TODO
// Union - TODO

// Makes a map out of the input array based on the provided function pointer.
LincArray* select_linc(LincArray* input, int (*func)(const int value))
{
	if (input == 0 || func == 0)
	{
		return 0;
	}

	LincArray* result = allocate_linc_array();
	if (result == 0)
	{
		puts("Unable to allocate memory for LincArray struct.");
		exit(1);
	}

	result->size = input->size;
	result->array = (int*)calloc(result->size, sizeof(int));

	if (result->array == 0)
	{
		puts("Unable to allocate memory for return array in select function.");
		exit(1);
	}

	size_t array_index = 0;

	do
	{
		*(result->array + (array_index)) = func(*(input->array + array_index));
	}
	while (++array_index < input->size);

	return result;
}

// Filters the input array based on provided function pointer.
LincArray* where_linc(LincArray* input, int (*predicate)(const int value)) 
{
	if (input == 0 || predicate == 0)
	{
		return 0;
	}

	LincArray* result = allocate_linc_array();
	if (result == 0)
	{
		puts("Unable to allocate memory for LincArray struct.");
		exit(1);
	}

	size_t array_index = 0;
	size_t array_size = 1;

	result->array = (int*)calloc(array_size, sizeof(int));

	if (result->array == 0)
	{
		puts("Unable to allocate memory for return array in where function.");
		exit(1);
	}

	for (size_t i = 0; i < input->size; i++)
	{
		if (predicate(*(input->array + i)))
		{
			*(result->array + (array_index++)) = *(input->array + i);
			int* array_tmp = realloc(result->array, sizeof(int) * ++array_size);
			if (array_tmp == 0)
			{
				puts("Unable to reallocate memory for array.");
				exit(1);
			}

			result->array = array_tmp;
		}
	}

	if ((array_size - 1) == 0)
	{
		result->size = 0;
		free(result->array);
		result->array = 0;
	}

	result->size = array_size - 1;
	return result;
}

// Flattens a array of LincArray* objects into a single one using the provided function pointer.
LincArray* selectmany_linc(LincArray** input_list, const size_t num_of_arrays, int (*func)(const int value))
{
	if (input_list == 0 || func == 0)
	{
		return 0;
	}

	LincArray* result = allocate_linc_array();
	if (result == 0)
	{
		puts("Unable to allocate memory for LincArray struct.");
		exit(1);
	}

	result->array = 0;
	size_t result_array_index = 0;

	for (size_t i = 0; i < num_of_arrays; i++)
	{
		size_t array_index = 0;

		LincArray* current = *(input_list + i);
		if (current == 0)
		{
			continue;
		}

		result->size += current->size;
		int* array_tmp = (int*)realloc(result->array, result->size * sizeof(int));

		if (array_tmp == 0)
		{
			puts("Unable to allocate memory with realloc on selectmany_linc");
			exit(1);
		}

		result->array = array_tmp;

		do
		{
			*(result->array + (result_array_index++)) = func(*(current->array + array_index));
		}
		while (++array_index < current->size); 
	}

	return result;
}

// Temporary driver code

int test_func(int value)
{
	return value;
}

int main(void)
{
	int array[ARRAY_SIZE] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int another_array[ARRAY_SIZE - 2] = { 10, 11, 12, 13, 14, 15, 16, 19 };

	LincArray array_one = { array, ARRAY_SIZE };
	LincArray array_two = { another_array, ARRAY_SIZE - 2 };

	LincArray* input[2] = { &array_one, &array_two };

	LincArray* result = selectmany_linc(input, 2, test_func);

	for (size_t i = 0; i < result->size; i++)
	{
		printf("%i\n", result->array[i]);
	}

	free_linc_array(result);

	return 0;
}
