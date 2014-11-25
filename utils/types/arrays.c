#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include "arrays.h"

doubleArray new_doubleArray(unsigned i)
{
	double *x = malloc(i * sizeof(double));

	for (unsigned k = 0; k < i; k++)
		x[k] = 0;

	doubleArray myArray;
	myArray.sizeX = i;
	myArray.elements = x;

	return myArray;
}
doubleArray2D new_doubleArray2D(unsigned i, unsigned j)
{
	doubleArray *x = malloc(i * sizeof(doubleArray));

	for (unsigned k = 0; k < i; k++)
		x[k] = new_doubleArray(j);

	doubleArray2D myArray;
	myArray.sizeX = i;
	myArray.sizeY = j;
	myArray.elements = x;

	return myArray;
}

unsignedArray new_unsignedArray(unsigned i)
{
	unsigned *x = malloc(i * sizeof(unsigned));

	for (unsigned k = 0; k < i; k++)
		x[k] = 0;

	unsignedArray myArray;
	myArray.sizeX = i;
	myArray.elements = x;

	return myArray;
}
unsignedArray2D new_unsignedArray2D(unsigned i, unsigned j)
{
	unsignedArray *x = malloc(i * sizeof(unsignedArray));

	for (unsigned k = 0; k < i; k++)
		x[k] = new_unsignedArray(j);

	unsignedArray2D myArray;
	myArray.sizeX = i;
	myArray.sizeY = j;
	myArray.elements = x;

	return myArray;
}

unsignedArray fill_unsignedArray(unsignedArray myArray, unsigned x)
{
	for (unsigned i = 0; i < myArray.sizeX; i++)
		myArray.elements[i] = x;

	return myArray;
}
doubleArray fill_doubleArray(doubleArray myArray, double x)
{
	for (unsigned i = 0; i < myArray.sizeX; i++)
		myArray.elements[i] = x;

	return myArray;
}

void print_unsignedArray(unsignedArray myArray)
{
	printf("[");

	for (unsigned i = 0; i < myArray.sizeX; i++) {
		if (i > 0)
			printf(", ");

		printf("%d", myArray.elements[i]);
	}

	printf("]");
}
void print_doubleArray(doubleArray myArray)
{
	printf("[");

	for (unsigned i = 0; i < myArray.sizeX; i++) {
		if (i > 0)
			printf(", ");

		printf("%f", myArray.elements[i]);
	}

	printf("]");
}
void print_unsignedArray2D(unsignedArray2D myArray)
{
	printf("[");

	for (unsigned i = 0; i < myArray.sizeX; i++) {
		if (i > 0)
			printf(", \n");

		print_unsignedArray(myArray.elements[i]);
	}

	printf("]");
}
void print_doubleArray2D(doubleArray2D myArray)
{
	printf("[");

	for (unsigned i = 0; i < myArray.sizeX; i++) {
		if (i > 0)
			printf(", \n");

		print_doubleArray(myArray.elements[i]);
	}

	printf("]");
}
