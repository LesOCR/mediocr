
#include "SDL/SDL.h"

#ifndef UTILS_IMAGE_IMAGE_
#define UTILS_IMAGE_IMAGE_

typedef struct
{
	unsigned x;
	unsigned y;

	unsigned r;
	unsigned g;
	unsigned b;
	unsigned a;
} imagePixel;

SDL_Surface *image_load(char *path);
Uint32 image_getPixelUint32(SDL_Surface *surface, unsigned x, unsigned y);
SDL_Color image_getPixelColor(SDL_Surface *surface, unsigned x, unsigned y);
unsigned image_getGreyscale(SDL_Color color);
double image_getGreyscaleRatio(SDL_Color color);
void image_renderConsole(SDL_Surface *surface);

#endif
