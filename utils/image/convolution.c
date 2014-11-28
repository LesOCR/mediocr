
#include <SDL/SDL.h>

#include "image.h"
#include "../helpers/maths.h"
#include "convolution.h"

SDL_Surface *convolution_apply(SDL_Surface *s, int *matrix, unsigned matrixSize)
{
	SDL_Surface *new = image_copy(s);

	for(int y = 0; y < s->w; y++) {
		for(int x = 0; x < s->h; x++) {
			SDL_Color color = convolution_dowork(s, matrix, matrixSize, x, y);
			image_putPixel(new, x, y, SDL_MapRGBA(s->format, color.r, color.g, color.b, 255));
		}
	}

	return new;
}

SDL_Color convolution_dowork(SDL_Surface *s, int *matrix, unsigned matrixSize,
	unsigned x, unsigned y)
{
	Uint16 rtotal = 0;
	Uint16 gtotal = 0;
	Uint16 btotal = 0;

	unsigned offset = matrixSize / 2;
	for(unsigned i = 0; i < matrixSize; i++) {
		for(unsigned j = 0; j < matrixSize; j++) {
			int xloc = maths_between(x + i - offset, 0, s->w);
			int yloc = maths_between(y + j - offset, 0, s->h);

			SDL_Color color = image_getPixelColor(s, xloc, yloc);

			rtotal += (255 - color.r) * matrix[i + matrixSize * j];
			gtotal += (255 - color.g) * matrix[i + matrixSize * j];
			btotal += (255 - color.b) * matrix[i + matrixSize * j];
		}
	}

	SDL_Color finalcolor = {
		.r = 255 - (int)maths_between(rtotal, 0, 255),
		.g = 255 - (int)maths_between(gtotal, 0, 255),
		.b = 255 - (int)maths_between(btotal, 0, 255)
	};

	return finalcolor;
}
