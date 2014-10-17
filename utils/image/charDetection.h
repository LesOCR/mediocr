
#include <SDL/SDL.h>
#include "../types/structArrays.h"

#ifndef UTILS_IMAGE_LINEDETECTION_
#define UTILS_IMAGE_LINEDETECTION_

struct ImageChar
{
	unsigned startX;
	unsigned startY;
	unsigned endX;
	unsigned endY;

	char *content;
};

struct ImageLine
{
	unsigned startX;
	unsigned startY;
	unsigned endX;
	unsigned endY;

	ImageCharArray chars;
};

unsigned charDetection_line(SDL_Surface *surface, struct ImageLine *imageLine,
	unsigned startY);
unsigned charDetection_char(SDL_Surface *surface, struct ImageLine imageLine,
	struct ImageChar *imageChar, unsigned startX);
ImageLineArray charDetection_go(SDL_Surface *surface);

#endif
