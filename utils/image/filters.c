
#include <SDL/SDL.h>
#include "../helpers/maths.h"
#include "image.h"
#include "convolution.h"
#include "filters.h"

SDL_Surface *filter_blur(SDL_Surface *s)
{
	int convolution[9] = {
		1, 1, 1,
		1, 1, 1,
		1, 1, 1
	};

	return convolution_apply(s, &convolution[0], 3, 9);
}

SDL_Surface *filter_sharpen(SDL_Surface *s)
{
	int convolution[9] = {
		0, -2, 0,
		-2, 11, -2,
		0, -2, 0
	};

	return convolution_apply(s, &convolution[0], 3, 3);
}

SDL_Surface *filter_edge(SDL_Surface *s)
{
	int convolution[9] = {
		0, 0, 0,
		-1, 1, 0,
		0, 0, 0
	};

	return convolution_apply(s, &convolution[0], 3, 3);
}

SDL_Surface *filter_binary(SDL_Surface *s, int threshold)
{
	SDL_Surface *new = image_copy(s);

	for(int y = 0; y < new->h; y++) {
		for(int x = 0; x < new->w; x++) {
			SDL_Color color = image_getPixelColor(new, x, y);
			SDL_Color newColor;
			if((color.r + color.g + color.b) / 3 > threshold)
				newColor = (SDL_Color){
					.r = 255,
					.g = 255,
					.b = 255
				};
			else
				newColor = (SDL_Color){
					.r = 0,
					.g = 0,
					.b = 0
				};

			image_putPixel(new, x, y, SDL_MapRGBA(new->format, newColor.r, newColor.g, newColor.b, 255));
		}
	}

	return new;
}

SDL_Surface *filter_createGroup(SDL_Surface *s)
{
	SDL_Surface *new = image_copy(s);

	for(int y = 0; y < s->h; y++) {
		for(int x = 0; x < s->w; x++) {
			SDL_Color color = _filter_createGroup_work(s, 25, 1, x, y);

			image_putPixel(new, x, y, SDL_MapRGBA(new->format, color.r, color.g, color.b, 255));
		}
	}

	return new;
}

SDL_Color _filter_createGroup_work(SDL_Surface *s, unsigned matrixSize, unsigned threshold,
	unsigned x, unsigned y)
{
	unsigned nbAround = 0;

	unsigned offset = matrixSize / 2;
	for(unsigned i = 0; i < matrixSize; i++) {
		for(unsigned j = 0; j < matrixSize; j++) {
			int xloc = maths_between(x + i - offset, 0, s->w - 1);
			int yloc = maths_between(y + j - offset, 0, s->h - 1);

			if (image_getPixelBool(s, xloc, yloc))
			{
				nbAround++;
			}

		}
	}

	SDL_Color finalcolor = {
		.r = (nbAround >= threshold) ? 0 : 255,
		.g = (nbAround >= threshold) ? 0 : 255,
		.b = (nbAround >= threshold) ? 0 : 255
	};

	return finalcolor;
}
