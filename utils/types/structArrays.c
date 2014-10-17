
#include <err.h>

#include "../image/charDetection.h"
#include "structArrays.h"

ImageLineArray new_ImageLineArray(unsigned i)
{
	struct ImageLine *x = malloc(i * sizeof(struct ImageLine));

	for(unsigned k = 0; k < i; k++)
	{
		struct ImageLine el;
		x[k] = el;
	}

	ImageLineArray myArray;
	myArray.sizeX = i;
	myArray.elements = x;

	return myArray;
}

ImageCharArray new_ImageCharArray(unsigned i)
{
	struct ImageChar *x = malloc(i * sizeof(struct ImageChar));

	for(unsigned k = 0; k < i; k++)
	{
		struct ImageChar el;
		x[k] = el;
	}

	ImageCharArray myArray;
	myArray.sizeX = i;
	myArray.elements = x;

	return myArray;
}

void add_ImageLineArray(ImageLineArray *array, unsigned i,
	struct ImageLine imageLine)
{
	if(i > array->sizeX)
	{
		// We need to realloc some memory
		i *= 2;
		printf("ImageLineArray new size: %d\n", i);
		array->elements =
			realloc(array->elements, i * sizeof(struct ImageLine));
		if(array->elements == NULL)
		{
			errx(1, "ImageLineArray: Could not realloc\n");
		}

		array->sizeX = i;
	}

	array->elements[i] = imageLine;
}

void add_ImageCharArray(ImageCharArray *array, unsigned i,
	struct ImageChar imageChar)
{
	if(i > array->sizeX)
	{
		// We need to realloc some memory
		i *= 2;
		printf("ImageCharArray new size: %d\n", i);
		array->elements =
			realloc(array->elements, i * sizeof(struct ImageChar));
		if(array->elements == NULL)
		{
			errx(1, "ImageCharArray: Could not realloc\n");
		}

		array->sizeX = i;
	}

	array->elements[i] = imageChar;
}
