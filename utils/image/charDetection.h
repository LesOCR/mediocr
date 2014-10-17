
#include <SDL/SDL.h>

#ifndef UTILS_IMAGE_LINEDETECTION_
#define UTILS_IMAGE_LINEDETECTION_

struct ImageLine
{
	unsigned startX;
	unsigned startY;
	unsigned endX;
	unsigned endY;

	char *content;
};

unsigned charDetection_line(SDL_Surface *surface, struct ImageLine *imageLine,
	unsigned startY);

#endif
