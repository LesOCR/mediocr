
#include <SDL/SDL.h>

#include "charDetection.h"


#ifndef UTILS_IMAGE_IMAGE_
#define UTILS_IMAGE_IMAGE_

SDL_Surface *image_load(char *path);
SDL_Surface *image_scale(SDL_Surface *Surface, Uint16 Width, Uint16 Height);
SDL_Surface *image_extractChar(SDL_Surface *surface, struct ImageChar *c);

Uint32 image_getPixelUint32(SDL_Surface *surface, unsigned x, unsigned y);
SDL_Color image_getPixelColor(SDL_Surface *surface, unsigned x, unsigned y);
unsigned image_getGreyscale(SDL_Color color);
double image_getGreyscaleRatio(SDL_Color color);
unsigned image_getPixelBool(SDL_Surface *surface, unsigned x,
	unsigned y);

void image_putPixel(SDL_Surface *surface, unsigned x, unsigned y,
	Uint32 pixel);

void image_renderConsoleFromTo(SDL_Surface *surface, unsigned x1, unsigned y1,
	unsigned x2, unsigned y2);
void image_renderConsole(SDL_Surface *surface);
void image_renderConsoleFromLine(SDL_Surface *surface,
	struct ImageLine imageLine);
void image_renderConsoleFromChar(SDL_Surface *surface,
	struct ImageChar imageChar);

#endif
