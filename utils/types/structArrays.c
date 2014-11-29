#include <err.h>

#include "../image/charDetection.h"
#include "structArrays.h"

/* ImageBlockArray functions */

ImageBlockArray new_ImageBlockArray(unsigned i)
{
	struct ImageBlock *x;
	ImageBlockArray newArray;
	x = calloc(i, i * sizeof(struct ImageBlock));

	newArray.capacity = i;
	newArray.size = 0;
	newArray.elements = x;

	return newArray;
}

void push_ImageBlockArray(ImageBlockArray *array, struct ImageBlock imageBlock)
{
	add_ImageBlockArray(array, array->size, imageBlock);
}

void add_ImageBlockArray(ImageBlockArray *array, unsigned i,
	struct ImageBlock imageBlock)
{
	if (i >= array->capacity) {
		// Reallocate some memory
		while (i >= array->capacity)
			array->capacity <<= 1;
		array->elements =
		realloc(array->elements,
			array->capacity * sizeof(struct ImageLine));
		if (array->elements == NULL)
			errx(1, "ImageLineArray: Could not reallocate memory\n");
	}

	array->elements[i] = imageBlock;
	array->size++;
}

/* ImageLineArray functions */

ImageLineArray new_ImageLineArray(unsigned i)
{
	struct ImageLine *x;
	ImageLineArray newArray;
	x = calloc(i, i * sizeof(struct ImageLine));

	newArray.capacity = i;
	newArray.size = 0;
	newArray.elements = x;

	return newArray;
}

void push_ImageLineArray(ImageLineArray *array, struct ImageLine imageLine)
{
	add_ImageLineArray(array, array->size, imageLine);
}

void add_ImageLineArray(ImageLineArray *array, unsigned i,
			struct ImageLine imageLine)
{
	if (i >= array->capacity) {
		// Reallocate some memory
		while (i >= array->capacity)
			array->capacity <<= 1;
		array->elements =
		    realloc(array->elements,
			    array->capacity * sizeof(struct ImageLine));
		if (array->elements == NULL)
			errx(1, "ImageLineArray: Could not reallocate memory\n");
	}

	array->elements[i] = imageLine;
	array->size++;
}

/* ImageCharArray functions */

ImageCharArray new_ImageCharArray(unsigned i)
{
	ImageCharArray newArray;

	newArray.capacity = i;
	newArray.size = 0;
	newArray.elements = calloc(i, i * sizeof(struct ImageChar));

	return newArray;
}

void push_ImageCharArray(ImageCharArray *array, struct ImageChar imageChar)
{
	add_ImageCharArray(array, array->size, imageChar);
}

void add_ImageCharArray(ImageCharArray *array, unsigned i,
			struct ImageChar imageChar)
{
	if (i >= array->capacity) {
		// Reallocate some memory
		while (i >= array->capacity)
			array->capacity <<= 1;
		array->elements =
		    realloc(array->elements,
			    array->capacity * sizeof(struct ImageChar));
		if (array->elements == NULL)
			errx(1, "ImageCharArray: Could not reallocate memory\n");
	}

	array->elements[i] = imageChar;
	array->size++;
}
