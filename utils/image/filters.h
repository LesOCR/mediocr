
#include <SDL/SDL.h>

#ifndef UTILS_IMAGE_FILTERS_
#define UTILS_IMAGE_FILTERS_

SDL_Surface *filter_blur(SDL_Surface *s);
SDL_Surface *filter_sharpen(SDL_Surface *s);
SDL_Surface *filter_edge(SDL_Surface *s);
SDL_Surface *filter_binary(SDL_Surface *s, int threshold);
SDL_Surface *filter_createGroup(SDL_Surface *s);
SDL_Color _filter_createGroup_work(SDL_Surface *s, unsigned matrixSize, unsigned threshold,
	unsigned x, unsigned y);

#endif
