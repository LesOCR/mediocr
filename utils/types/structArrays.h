#ifndef UTILS_TYPES_STRUCTARRAYS_
#define UTILS_TYPES_STRUCTARRAYS_

typedef struct {
	unsigned capacity;
	unsigned size;
	struct ImageChar *elements;
} ImageCharArray;

typedef struct {
	unsigned capacity;
	unsigned size;
	struct ImageLine *elements;
} ImageLineArray;

typedef struct {
	unsigned capacity;
	unsigned size;
	struct ImageBlock *elements;
} ImageBlockArray;

ImageBlockArray new_ImageBlockArray(unsigned i);
ImageLineArray new_ImageLineArray(unsigned i);
ImageCharArray new_ImageCharArray(unsigned i);
void push_ImageBlockArray(ImageBlockArray *array, struct ImageBlock imageBlock);
void add_ImageBlockArray(ImageBlockArray *array, unsigned i,
			struct ImageBlock imageBlock);
void push_ImageLineArray(ImageLineArray *array, struct ImageLine imageLine);
void add_ImageLineArray(ImageLineArray *array, unsigned i,
			struct ImageLine imageLine);
void push_ImageCharArray(ImageCharArray *array, struct ImageChar imageChar);
void add_ImageCharArray(ImageCharArray *array, unsigned i,
			struct ImageChar imageChar);

#endif
