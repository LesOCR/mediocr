#ifndef UTILS_TYPES_ARRAYS_
#define UTILS_TYPES_ARRAYS_

typedef struct {
	unsigned sizeX;
	unsigned *elements;
} unsignedArray;

typedef struct {
	unsigned sizeX;
	double *elements;
} doubleArray;

typedef struct {
	unsigned sizeX;
	unsigned sizeY;
	unsignedArray *elements;
} unsignedArray2D;

typedef struct {
	unsigned sizeX;
	unsigned sizeY;
	doubleArray *elements;
} doubleArray2D;

doubleArray new_doubleArray(unsigned i);
doubleArray2D new_doubleArray2D(unsigned i, unsigned j);
unsignedArray new_unsignedArray(unsigned i);
unsignedArray2D new_unsignedArray2D(unsigned i, unsigned j);
unsignedArray fill_unsignedArray(unsignedArray myArray, unsigned x);
doubleArray fill_doubleArray(doubleArray myArray, double x);
void print_unsignedArray(unsignedArray myArray);
void print_doubleArray(doubleArray myArray);
void print_unsignedArray2D(unsignedArray2D myArray);
void print_doubleArray2D(doubleArray2D myArray);

#endif
