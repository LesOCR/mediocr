
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
	if(i >= array->sizeX)
	{
		// We need to realloc some memory
		array = realloc(array, i * 2 * sizeof(struct ImageLine));
		array->sizeX = i * 2;

		for(unsigned k = i; k < i * 2; k++)
		{
			struct ImageLine el;
			array->elements[k] = el;
		}
	}

	array->elements[i] = imageLine;
}

void add_ImageCharArray(ImageCharArray *array, unsigned i,
	struct ImageChar imageChar)
{
	if(i >= array->sizeX)
	{
		// We need to realloc some memory
		array = realloc(array, i * 2 * sizeof(struct ImageChar));
		array->sizeX = i * 2;

		for(unsigned k = i; k < i * 2; k++)
		{
			struct ImageChar el;
			array->elements[k] = el;
		}
	}

	array->elements[i] = imageChar;
}
