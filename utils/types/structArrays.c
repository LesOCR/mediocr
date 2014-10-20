#include <err.h>

#include "../image/charDetection.h"
#include "structArrays.h"

ImageLineArray new_ImageLineArray(unsigned i)
{
	struct ImageLine *x;
	ImageLineArray newArray;
	x = calloc(i, i*sizeof(struct ImageLine));

	newArray.capacity = i;
	newArray.size = 0;
	newArray.elements = x;

	return newArray;
}

ImageCharArray new_ImageCharArray(unsigned i)
{
	struct ImageChar *x;
	ImageCharArray newArray;
	x = calloc(i, i*sizeof(struct ImageChar));

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
		array->elements = realloc(array->elements, array->capacity*
			sizeof(struct ImageLine));
		if(array->elements == NULL)
			errx(1, "ImageLineArray: Could not reallocate memory\n");
	}

	array->elements[i] = imageLine;
	array->size++;
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
		array->elements = realloc(array->elements, array->capacity*
			sizeof(struct ImageChar));
		if(array->elements == NULL)
			errx(1, "ImageCharArray: Could not reallocate memory\n");
	}

	array->elements[i] = imageChar;
	array->size++;
}
