
#include <stdio.h>
#include <err.h>
#include <assert.h>
#include <SDL/SDL.h>

#include "image.h"
#include "charDetection.h"
#include "../helpers/file.h"

SDL_Surface *image_load(char *path)
{
	printf("Loading image [%s]\n", path);

	// Assert is better than err or warn in this case, as it gives
	// better informations.
	assert(file_exists(path));

	return SDL_LoadBMP(path);
}

SDL_Surface *image_scale(SDL_Surface *Surface, Uint16 Width, Uint16 Height)
{
	if(!Surface || !Width || !Height)
		return 0;

	SDL_Surface *_ret = SDL_CreateRGBSurface(Surface->flags, Width, Height,
		Surface->format->BitsPerPixel, Surface->format->Rmask,
		Surface->format->Gmask, Surface->format->Bmask, Surface->format->Amask);

	double _stretch_factor_x = ((double)Width) / ((double)Surface->w);
	double _stretch_factor_y = ((double)Height) / ((double)Surface->h);
	printf("%d\n", Surface->w);
	printf("%d\n", Surface->h);

	for(Sint32 y = 0; y < Surface->h; y++)
		for(Sint32 x = 0; x < Surface->w; x++)
			for(Sint32 o_y = 0; o_y < _stretch_factor_y; ++o_y)
				for(Sint32 o_x = 0; o_x < _stretch_factor_x; ++o_x)
					image_putPixel(_ret, (_stretch_factor_x * x) + o_x,
						(_stretch_factor_y * y) + o_y,
						image_getPixelUint32(Surface, x, y));
	return _ret;
}

SDL_Color image_getPixelColor(SDL_Surface *surface, unsigned x,
	unsigned y)
{
	Uint32 intColor = image_getPixelUint32(surface, x, y);
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	    SDL_Color color={(intColor & 0x00ff0000) / 0x10000, (intColor &
			0x0000ff00) / 0x100,(intColor & 0x000000ff), 0};
	#else
	    SDL_Color color={(intColor & 0x000000ff), (intColor &
			0x0000ff00) / 0x100,(intColor & 0x00ff0000) / 0x10000,0};
	#endif

	return color;
}

Uint32 image_getPixelUint32(SDL_Surface *surface, unsigned x,
	unsigned y)
{
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch
	         + x * surface->format->BytesPerPixel;

	switch(surface->format->BytesPerPixel) {
	case 1:
	    return *p;
	    break;

	case 2:
	    return *(Uint16 *)p;
	    break;

	case 3:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN //
	    return p[0] << 16 | p[1] << 8 | p[2];
#else //
	    return p[0] | p[1] << 8 | p[2] << 16;
#endif //
	    break;

	case 4:
	    return *(Uint32 *)p;
	    break;

	default:
	    return 0;
	    break; //
	}
}

unsigned image_getGreyscale(SDL_Color color)
{
	return 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
}

double image_getGreyscaleRatio(SDL_Color color)
{
	return image_getGreyscale(color) / 255;
}

unsigned image_getPixelBool(SDL_Surface *surface, unsigned x,
	unsigned y)
{
	return image_getGreyscaleRatio(image_getPixelColor(surface, x, y)) < 0.5;
}

static inline
Uint8* pixelref(SDL_Surface *surf, unsigned x, unsigned y) {
  int bpp = surf->format->BytesPerPixel;
  return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

void image_putPixel(SDL_Surface *surface, unsigned x, unsigned y,
	Uint32 pixel) {
	Uint8 *p = pixelref(surface, x, y);
	switch(surface->format->BytesPerPixel) {
	case 1:
		*p = pixel;
		break;
	case 2:
		*(Uint16 *)p = pixel;
		break;
	case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		} else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;
	case 4:
		*(Uint32 *)p = pixel;
		break;
	}
}

void image_renderConsoleFromTo(SDL_Surface *surface, unsigned x1, unsigned y1,
 	unsigned x2, unsigned y2)
{
	for(unsigned y = y1; y < y2; y++)
	{
		for(unsigned x = x1; x < x2; x++)
		{
			if(image_getPixelBool(surface, x, y))
			{
				printf("█");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}

void image_renderConsole(SDL_Surface *surface)
{
	image_renderConsoleFromTo(surface, 0, 0, surface->w, surface->h);
}

void image_renderConsoleFromLine(SDL_Surface *surface,
	struct ImageLine imageLine)
{
	image_renderConsoleFromTo(surface, imageLine.startX,
		imageLine.startY, imageLine.endX, imageLine.endY);
}

void image_renderConsoleFromChar(SDL_Surface *surface,
	struct ImageChar imageChar)
{
	image_renderConsoleFromTo(surface, imageChar.startX,
		imageChar.startY, imageChar.endX, imageChar.endY);
}
