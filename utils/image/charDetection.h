#include <SDL/SDL.h>
#include "../types/structArrays.h"

#ifndef UTILS_IMAGE_LINEDETECTION_
#define UTILS_IMAGE_LINEDETECTION_

struct ImageChar {
	unsigned startX;
	unsigned startY;
	unsigned endX;
	unsigned endY;

	unsigned space;

	char content;
};

struct ImageLine {
	unsigned startX;
	unsigned startY;
	unsigned endX;
	unsigned endY;

	ImageCharArray chars;

	char *content;
};

struct ImageBlock {
	unsigned startX;
	unsigned startY;
	unsigned endX;
	unsigned endY;

	ImageLineArray lines;
};

struct BlockStack {
	int x;
	int y;
};

unsigned charDetection_line(SDL_Surface *surface, struct ImageLine *imageLine,
	unsigned startX, unsigned stopX, unsigned startY, unsigned stopY);
unsigned charDetection_char(SDL_Surface *surface, struct ImageLine imageLine,
			    struct ImageChar *imageChar, unsigned startX, unsigned start,
				unsigned spaceSize);
void charDetection_blockRec(unsigned *map, int width, int height, int x, int y,
	int *topX, int *topY, int *bottomX, int *bottomY);
ImageBlockArray charDetection_blocks(SDL_Surface *surface);
ImageLineArray charDetection_go(SDL_Surface *surface, unsigned topX, unsigned topY,
	unsigned bottomX, unsigned bottomY);

#endif
