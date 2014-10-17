
#include <stdio.h>
#include <err.h>
#include <assert.h>
#include "SDL/SDL.h"

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

void image_renderConsoleFromTo(SDL_Surface *surface, unsigned x1, unsigned y1,
 	unsigned x2, unsigned y2)
{
	for(unsigned y = y1; y < y2; y++)
	{
		for(unsigned x = x1; x < x2; x++)
		{
			if(image_getPixelBool(surface, x, y))
			{
				printf("0");
			}
			else
			{
				printf(".");
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
