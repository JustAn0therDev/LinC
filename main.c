#include <stdio.h>
#include <stdlib.h>

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

#define ARRAY_SIZE 10
// Linq Methods

// TODO: maybe its better to return a struct that contains the int pointer and a size?
LincResult* select_linc(int* array, size_t array_size, int (*conditional_func)(int value))
{
	LincResult* result = allocate_linc_result();
	if (result == 0)
	{
		puts("Unable to allocate memory for LincResult struct.");
		exit(1);
	}

	result->collection = calloc(1, sizeof(int));

	if (result->collection == 0)
	{
		puts("Unable to allocate memory for return array in select.");
		exit(1);
	}

	size_t collection_index = 0;
	size_t collection_size = 1;

	for (size_t i = 0; i < array_size; i++)
	{
		if (conditional_func(*(array + i)))
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
	// this is collection_size - 1 because the increment works in a way that is ready to reallocate more memory if needed.
	result->size = collection_size - 1;
	return result;
}

int filter_select(int value)
{
	return value >= 3;
}

int main(void)
{
	int array[ARRAY_SIZE] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	LincResult* result = select_linc(array, ARRAY_SIZE, filter_select);

	for (size_t i = 0; i < result->size; i++)
	{
		printf("%i\n", result->collection[i]);
	}

	free_linc_result(result);

	return 0;
}
