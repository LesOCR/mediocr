
#include <SDL/SDL.h>

#ifndef UTILS_IMAGE_CONVOLUTION_
#define UTILS_IMAGE_CONVOLUTION_

SDL_Surface *convolution_apply(SDL_Surface *s, int *matrix, unsigned matrixSize);
SDL_Color convolution_dowork(SDL_Surface *s, int *matrix, unsigned matrixSize,
	unsigned x, unsigned y);

#endif
