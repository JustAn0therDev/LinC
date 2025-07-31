#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 10

typedef struct linc_result
{
	size_t size;
	int* collection;
} LincResult;

// Allocates whatever is needed of memory for a LincResult struct. The caller should be the one to handle the freeing.
LincResult* allocate_linc_result()
{
	LincResult* linc_result = (LincResult*)calloc(1, sizeof(LincResult));
	return linc_result;
}

// Frees the whole LincResult struct allocated.
void free_linc_result(LincResult* linc_result)
{
	if (linc_result == 0)
		return;	

	if (linc_result->collection != 0)
	{
		free(linc_result->collection);
	}

	free(linc_result);
}

// Linq Methods - The caller is always responsible for freeing the allocated LincResult.
// TODO: SelectMany
// TODO: Intersect
// TODO: OrderByDescending
// TODO: OrderBy
// TODO: Join
// TODO: GroupBy

// Makes a map out of the input array
LincResult* select_linc(int* array, const size_t array_size, int (*func)(const int value))
{
	LincResult* result = allocate_linc_result();
	if (result == 0)
	{
		puts("Unable to allocate memory for LincResult struct.");
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
LincResult* where_linc(int* array, const size_t array_size, int (*predicate)(const int value)) 
{
	LincResult* result = allocate_linc_result();
	if (result == 0)
	{
		puts("Unable to allocate memory for LincResult struct.");
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

int test_func(int value)
{
	return value >= 3;
}

int main(void)
{
	int array[ARRAY_SIZE] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	LincResult* result = select_linc(array, ARRAY_SIZE, test_func);

	for (size_t i = 0; i < result->size; i++)
	{
		printf("%i\n", result->collection[i]);
	}

	free_linc_result(result);

	return 0;
}
