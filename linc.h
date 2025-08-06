#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// The main struct that contains the array and its size. Every function uses this struct to represent an input and/or output.
typedef struct linc_array
{
	int* array;
	size_t size;
} LincArray;

// Allocates whatever is needed of memory for a LincArray struct. The caller should be the one to handle the freeing.
LincArray* linc_alloc();
// Frees the allocated LincArray struct.
void linc_free(LincArray* linc_result);
// Makes a map out of the input array based on the function pointer.
LincArray* linc_select(const LincArray* input, int (*func)(const int value));
// Filters the input array based on the provided predicate.
LincArray* linc_where(const LincArray* input, int (*predicate)(const int value));
// Flattens an array of LincArray* into a single LincArray* based on the function pointer.
LincArray* linc_selectmany(const LincArray** input_list, const size_t num_of_arrays, int (*func)(const int value));
// Intersects an array with another, returning a new array containing the common elements of the two.
LincArray* linc_intersect(const LincArray* input_one, const LincArray* input_two);
// Takes the first N elements from the input array and returns it in a new array.
LincArray* linc_take(const LincArray* input, const int amount);
// Takes elements from the array while the predicate is true.
LincArray* linc_takewhile(const LincArray* input, int(*predicate)(const int value));
// Skips the first N elements of the array and returns the rest.
LincArray* linc_skip(const LincArray* input, const int amount);
// Skips elements in the array while the predicate function is true.
LincArray* linc_skipwhile(const LincArray* input, int(*predicate)(const int value));
// Returns a new array containing only unique elements from the input array.
LincArray* linc_distinct(const LincArray* input);
// Modifies each element of the input using the provided function pointer.
void linc_foreach(const LincArray* input, int(*func)(int value));

LincArray* linc_alloc()
{
	LincArray* linc_result = (LincArray*)calloc(1, sizeof(LincArray));
	assert(linc_result != 0);
	return linc_result;
}

void linc_free(LincArray* linc_result)
{
	if (linc_result == 0)
		return;

	if (linc_result->array != 0)
	{
		free(linc_result->array);
	}

	free(linc_result);
}

LincArray* linc_select(const LincArray* input, int (*func)(const int value))
{
	assert(input != 0);
	assert(input->array != 0);
	assert(func != 0);

	LincArray* result = linc_alloc();

	result->size = input->size;

	result->array = (int*)calloc(result->size, sizeof(int));
	assert(result->array != 0);

	size_t array_index = 0;

	do
	{
		*(result->array + (array_index)) = func(*(input->array + array_index));
	} while (++array_index < input->size);

	return result;
}

LincArray* linc_where(const LincArray* input, int (*predicate)(const int value))
{
	assert(input != 0);
	assert(input->array != 0);
	assert(predicate != 0);

	LincArray* result = linc_alloc();

	size_t array_index = 0;
	size_t array_size = 1;

	for (size_t i = 0; i < input->size; i++)
	{
		if (predicate(*(input->array + i)))
		{
			int* array_tmp = realloc(result->array, sizeof(int) * ++array_size);
			assert(array_tmp);
			result->array = array_tmp;

			*(result->array + (array_index++)) = *(input->array + i);
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

LincArray* linc_selectmany(const LincArray** input_list, const size_t num_of_arrays, int (*func)(const int value))
{
	assert(input_list != 0);
	assert(func != 0);
	assert(num_of_arrays >= 0);

	LincArray* result = linc_alloc();

	size_t result_array_index = 0;

	for (size_t i = 0; i < num_of_arrays; i++)
	{
		size_t array_index = 0;

		LincArray* current = *(input_list + i);
		if (current == 0 || current->size == 0)
		{
			continue;
		}

		result->size += current->size;

		int* array_tmp = (int*)realloc(result->array, result->size * sizeof(int));
		assert(array_tmp != 0);

		result->array = array_tmp;

		do
		{
			*(result->array + (result_array_index++)) = func(*(current->array + array_index));
		} while (++array_index < current->size);
	}

	return result;
}

LincArray* linc_intersect(const LincArray* input_one, const LincArray* input_two)
{
	assert(input_one != 0);
	assert(input_one->array != 0);
	assert(input_two != 0);
	assert(input_two->array != 0);

	LincArray* result = linc_alloc();

	size_t result_array_index = 0;
	size_t result_array_size = 1;

	LincArray* biggest = 0;
	LincArray* smallest = 0;

	int first_is_bigger = input_one->size > input_two->size;

	if (first_is_bigger)
	{
		biggest = input_one;
		smallest = input_two;
	}
	else
	{
		biggest = input_two;
		smallest = input_one;
	}

	for (size_t i = 0; i < smallest->size; i++)
	{
		if (*(smallest->array + i) == *(biggest->array + i))
		{
			int* array_tmp = realloc(result->array, sizeof(int) * ++result_array_size);
			assert(array_tmp != 0);
			result->array = array_tmp;

			*(result->array + (result_array_index++)) = *(smallest->array + i);
		}
	}

	if ((result_array_size - 1) == 0)
	{
		result->size = 0;
		free(result->array);
		result->array = 0;
	}

	result->size = result_array_size - 1;
	return result;
}

LincArray* linc_take(const LincArray* input, const int amount)
{
	assert(input != 0);
	assert(input->array != 0);
	assert(amount > 0);

	LincArray* result = linc_alloc();
	result->array = 0;

	int i;

	for (i = 0; i < amount; i++)
	{
		int* result_array_tmp = (int*)realloc(result->array, sizeof(int) * (i + 1));
		assert(result_array_tmp != 0);
		result->array = result_array_tmp;

		*(result->array + i) = *(input->array + i);
	}

	result->size = i;

	return result;
}

LincArray* linc_takewhile(const LincArray* input, int(*predicate)(const int value))
{
	assert(input != 0);
	assert(input->array != 0);
	assert(predicate != 0);

	LincArray* result = linc_alloc();

	int i;

	for (i = 0; i < input->size; i++)
	{
		if (!predicate(*(input->array + i)))
			break;

		int* result_array_tmp = (int*)realloc(result->array, sizeof(int) * (i + 1));
		assert(result_array_tmp != 0);
		result->array = result_array_tmp;

		*(result->array + i) = *(input->array + i);
	}

	result->size = i;

	return result;
}

LincArray* linc_skip(const LincArray* input, const int amount)
{
	assert(input != 0);
	assert(input->array != 0);
	assert(amount > 0);
	assert(input->size >= amount);

	LincArray* result = linc_alloc();

	int i;
	size_t result_array_size = 0;

	for (i = amount; i < input->size; i++)
	{
		result_array_size = (size_t)(i - amount);
		int* result_array_tmp = (int*)realloc(result->array, sizeof(int) * (result_array_size + 1));
		assert(result_array_tmp != 0);
		result->array = result_array_tmp;

		*(result->array + result_array_size) = *(input->array + i);
	}

	result->size = result_array_size;

	return result;
}

LincArray* linc_skipwhile(const LincArray* input, int(*predicate)(const int value))
{
	assert(input != 0);
	assert(input->array != 0);
	assert(predicate != 0);

	LincArray* result = linc_alloc();

	int i;
	size_t result_array_size = 0;
	int predicate_is_true = 1;

	for (i = 0; i < input->size; i++)
	{
		if (predicate_is_true)
			predicate_is_true = !predicate(*(input->array + i));

		if (!predicate_is_true)
		{
			int* result_array_tmp = (int*)realloc(result->array, sizeof(int) * (++result_array_size));
			assert(result_array_tmp != 0);
			result->array = result_array_tmp;

			*(result->array + result_array_size - 1) = *(input->array + i);
		}
	}

	result->size = result_array_size;

	return result;
}

LincArray* linc_distinct(const LincArray* input)
{
	assert(input != 0);
	assert(input->array != 0);

	LincArray* result = linc_alloc();

	int i;
	size_t result_array_size = 0;

	for (i = 0; i < input->size; i++)
	{
		int result_array_contains_item = 0;
		for (int j = 0; j < result_array_size; j++)
		{
			if (*(result->array + j) == *(input->array + i))
			{
				result_array_contains_item = 1;
				break;
			}
		}

		if (result_array_contains_item)
			continue;

		int* result_array_tmp = (int*)realloc(result->array, sizeof(int) * (++result_array_size));
		assert(result_array_tmp != 0);
		result->array = result_array_tmp;

		*(result->array + result_array_size - 1) = *(input->array + i);
	}

	result->size = result_array_size;

	return result;
}

void linc_foreach(const LincArray* input, int(*func)(int value))
{
	assert(input != 0);
	assert(input->array != 0);
	assert(func != 0);

	for (int i = 0; i < input->size; i++)
	{
		*(input->array + i) = func(*(input->array + i));
	}
}