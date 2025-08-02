#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 10

typedef struct linc_array
{
	size_t size;
	int* collection;
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

	if (linc_result->collection != 0)
	{
		free(linc_result->collection);
	}

	free(linc_result);
}

// Linq Methods - The caller is always responsible for freeing the allocated LincArray.
// SelectMany - TODO
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

// Makes a map out of the input array
// TODO: change the parameters to accept a LincArray object
LincArray* select_linc(int* array, const size_t array_size, int (*func)(const int value))
{
	LincArray* result = allocate_linc_array();
	if (result == 0)
	{
		puts("Unable to allocate memory for LincArray struct.");
		exit(1);
	}

	result->size = array_size;
	result->collection = (int*)calloc(array_size, sizeof(int));

	if (result->collection == 0)
	{
		puts("Unable to allocate memory for return array in select function.");
		exit(1);
	}

	size_t collection_index = 0;

	do
	{
		*(result->collection + (collection_index)) = func(*(array + collection_index));
	}
	while (++collection_index < array_size);

	return result;
}

// Filters the input array.
// TODO: change the parameters to accept a LincArray object
LincArray* where_linc(int* array, const size_t array_size, int (*predicate)(const int value)) 
{
	LincArray* result = allocate_linc_array();
	if (result == 0)
	{
		puts("Unable to allocate memory for LincArray struct.");
		exit(1);
	}

	size_t collection_index = 0;
	size_t collection_size = 1;

	result->collection = (int*)calloc(collection_size, sizeof(int));

	if (result->collection == 0)
	{
		puts("Unable to allocate memory for return array in where function.");
		exit(1);
	}

	for (size_t i = 0; i < array_size; i++)
	{
		if (predicate(*(array + i)))
		{
			*(result->collection + (collection_index++)) = *(array + i);
			int* collection_tmp = realloc(result->collection, sizeof(int) * ++collection_size);
			if (collection_tmp == 0)
			{
				puts("Unable to reallocate memory for array.");
				exit(1);
			}

			result->collection = collection_tmp;
		}
	}

	if ((collection_size - 1) == 0)
	{
		result->size = 0;
		free(result->collection);
		result->collection = 0;
	}

	result->size = collection_size - 1;
	return result;
}

// TODO: Change the parameter to accept an array of LincArray object
LincArray* selectmany_linc(int** array_of_arrays, const size_t array_size, int (*func)(const int value))
{
	LincArray* result = allocate_linc_array();
	if (result == 0)
	{
		puts("Unable to allocate memory for LincArray struct.");
		exit(1);
	}

	if (array_of_arrays == 0)
	{
		puts("There are no arrays to check.");
		free_linc_array(result);
		return 0;
	}

	result->collection = 0;
	size_t collection_index = 0;

	for (size_t i = 0; i < array_size; i++)
	{
		size_t array_index = 0;
		int* array = *(array_of_arrays + i);
		// This should use the array size of each array in the LincArray object.
		result->size += sizeof(array) / sizeof(array[0]);

		int* collection_tmp = (int*)realloc(result->collection, result->size * sizeof(int));

		if (collection_tmp == 0)
		{
			puts("Unable to allocate memory with realloc on selectmany_linc");
			exit(1);
		}

		result->collection = collection_tmp;

		do
		{
			*(result->collection + (collection_index)) = func(*(array + array_index++));
		}
		while (++collection_index < result->size); 
	}

	return result;
}

// Temporary driver code

int test_func(int value)
{
	return value >= 3;
}

int main(void)
{
	int array[ARRAY_SIZE] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int another_array[ARRAY_SIZE] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

	int** array_of_arrays[2] = { array, another_array };

	LincArray* result = selectmany_linc(array_of_arrays, 2, test_func);

	for (size_t i = 0; i < result->size; i++)
	{
		printf("%i\n", result->collection[i]);
	}

	free_linc_array(result);

	return 0;
}
