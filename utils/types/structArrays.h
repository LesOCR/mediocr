
#include "../image/charDetection.h"

#ifndef UTILS_TYPES_STRUCTARRAYS_
#define UTILS_TYPES_STRUCTARRAYS_

typedef struct
{
	unsigned sizeX;
	struct ImageChar *elements;
} ImageCharArray;

typedef struct
{
	unsigned sizeX;
	struct ImageLine *elements;
} ImageLineArray;


ImageLineArray new_ImageLineArray(unsigned i);
ImageCharArray new_ImageCharArray(unsigned i);
void add_ImageLineArray(ImageLineArray *array, unsigned i,
	struct ImageLine imageLine);
void add_ImageCharArray(ImageCharArray *array, unsigned i,
	struct ImageChar imageChar);

#endif
