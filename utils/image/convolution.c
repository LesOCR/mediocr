
#include <SDL/SDL.h>
#include <math.h>

#include "image.h"
#include "../helpers/maths.h"
#include "convolution.h"

SDL_Surface *convolution_apply(SDL_Surface *s, int *matrix, unsigned matrixSize,
	unsigned divider)
{
	SDL_Surface *new = image_copy(s);

	for(int y = 0; y < s->h; y++) {
		for(int x = 0; x < s->w; x++) {
			SDL_Color color = convolution_dowork(s, matrix, matrixSize, divider, x, y);

			image_putPixel(new, x, y, SDL_MapRGBA(new->format, color.r, color.g, color.b, 255));
		}
	}

	return new;
}

SDL_Color convolution_dowork(SDL_Surface *s, int *matrix, unsigned matrixSize,
	unsigned divider, unsigned x, unsigned y)
{
	Uint16 rtotal = 0;
	Uint16 gtotal = 0;
	Uint16 btotal = 0;

	unsigned offset = matrixSize / 2;
	for(unsigned i = 0; i < matrixSize; i++) {
		for(unsigned j = 0; j < matrixSize; j++) {
			int xloc = maths_between(x + i - offset, 0, s->w - 1);
			int yloc = maths_between(y + j - offset, 0, s->h - 1);

			SDL_Color color = image_getPixelColor(s, xloc, yloc);

			rtotal += (color.r) * matrix[i + matrixSize * j];
			gtotal += (color.g) * matrix[i + matrixSize * j];
			btotal += (color.b) * matrix[i + matrixSize * j];
		}
	}

	SDL_Color finalcolor = {
		.r = maths_between(rtotal / divider, 0, 255),
		.g = maths_between(gtotal / divider, 0, 255),
		.b = maths_between(btotal / divider, 0, 255)
	};

	return finalcolor;
}
