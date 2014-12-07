#include <stdio.h>
#include <err.h>
#include <assert.h>
#include <SDL/SDL.h>

#include "image.h"
#include "charDetection.h"
#include "../helpers/file.h"

void wait_for_keypressed(void) {
	SDL_Event             event;
	// Infinite loop, waiting for event
	for (;;) {
		// Take an event
		SDL_PollEvent( &event );
		// Switch on event type
		switch (event.type) {
			// Someone pressed a key -> leave the function
			case SDL_KEYDOWN: return;
			default: break;
		}
		// Loop until we got the expected event
	}
}

SDL_Surface* image_display(SDL_Surface *img)
{
	SDL_Surface          *screen;
	// Set the window to the same size as the image
	screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
	if ( screen == NULL ) {
		// error management
		errx(1, "Couldn't set %dx%d video mode: %s\n",
		img->w, img->h, SDL_GetError());
	}

	/* Blit onto the screen surface */
	if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
		warnx("BlitSurface error: %s\n", SDL_GetError());

	// Update the screen
	SDL_UpdateRect(screen, 0, 0, img->w, img->h);

	// wait for a key
	wait_for_keypressed();

	// return the screen for further uses
	return screen;
}

SDL_Surface *image_copy(SDL_Surface *surf)
{
	SDL_Surface *res;

	res = SDL_CreateRGBSurface(surf->flags, surf->w, surf->h,
		surf->format->BitsPerPixel, surf->format->Rmask,
		surf->format->Gmask, surf->format->Bmask,
		surf->format->Amask);

	if(res != NULL) {
		SDL_BlitSurface(surf, NULL, res, NULL);
	}

	return res;
}

SDL_Surface *image_load(char *path)
{
	printf("Loading image [%s]\n", path);

	// Assert is better than err or warn in this case, as it gives
	// better informations.
	assert(file_exists(path));

	SDL_Surface *s = SDL_LoadBMP(path);
	float maxSize = 2000;

	if(s->w > maxSize || s->h > maxSize)
	{
		if(s->w > s->h)
		{
			return image_scale(s, maxSize, s->h * (maxSize / s->w));
		}

		return image_scale(s, s->w * (maxSize / s->h), maxSize);
	}

	return SDL_LoadBMP(path);
}

SDL_Surface *image_scale(SDL_Surface *Surface, Uint16 Width, Uint16 Height)
{
	if (!Surface || !Width || !Height)
		return 0;

	SDL_Surface *_ret = SDL_CreateRGBSurface(
	    Surface->flags, Width, Height, Surface->format->BitsPerPixel,
	    Surface->format->Rmask, Surface->format->Gmask,
	    Surface->format->Bmask, Surface->format->Amask);

	double _stretch_factor_x = ((double)Width) / ((double)Surface->w);
	double _stretch_factor_y = ((double)Height) / ((double)Surface->h);

	for (Sint32 y = 0; y < Surface->h; y++)
		for (Sint32 x = 0; x < Surface->w; x++)
			for (Sint32 o_y = 0; o_y < _stretch_factor_y; ++o_y)
				for (Sint32 o_x = 0; o_x < _stretch_factor_x;
				     ++o_x)
					image_putPixel(
					    _ret, (_stretch_factor_x * x) + o_x,
					    (_stretch_factor_y * y) + o_y,
					    image_getPixelUint32(Surface, x,
								 y));
	return _ret;
}

SDL_Surface *image_extractChar(SDL_Surface *surface, struct ImageChar *c)
{
	int w = c->endX - c->startX;
	int h = c->endY - c->startY;

	SDL_Surface *newSurface = SDL_CreateRGBSurface(
	    surface->flags, w, h, surface->format->BitsPerPixel,
	    surface->format->Rmask, surface->format->Gmask,
	    surface->format->Bmask, surface->format->Amask);

	SDL_Rect rectMask = {.x = (Sint16)c->startX,
			     .y = (Sint16)c->startY,
			     .w = (Sint16)w,
			     .h = (Sint16)h};

	int r = SDL_BlitSurface(surface, &rectMask, newSurface, NULL);
	if (r)
		return 0;

	return image_crop(newSurface);
}

SDL_Surface *image_crop(SDL_Surface *surface)
{
	unsigned topX = _image_cropLeft(surface);
	unsigned topY = _image_cropTop(surface);
	unsigned bottomY = _image_cropBottom(surface) + 1;
	unsigned bottomX = _image_cropRight(surface) + 1;
	unsigned width = bottomX - topX;
	unsigned height = bottomY - topY;

	SDL_Surface *newSurface = SDL_CreateRGBSurface(
	    surface->flags, width, height, surface->format->BitsPerPixel,
	    surface->format->Rmask, surface->format->Gmask,
	    surface->format->Bmask, surface->format->Amask);

	SDL_Rect rectMask = {.x = (Sint16)topX,
			     .y = (Sint16)topY,
			     .w = (Sint16)width,
			     .h = (Sint16)height};

	int r = SDL_BlitSurface(surface, &rectMask, newSurface, NULL);
	if (r)
		return 0;

	return newSurface;
}
unsigned _image_cropTop(SDL_Surface *surface)
{
	unsigned top = 0;

	for(unsigned y = 0; y < (unsigned)surface->h; y++)
	{
		for(unsigned x = 0; x < (unsigned)surface->w; x++)
		{
			if(image_getPixelBool(surface, x, y))
				return top;
		}

		top++;
	}

	return top;
}
unsigned _image_cropBottom(SDL_Surface *surface)
{
	unsigned bottom = surface->h - 1;

	for(unsigned y = surface->h - 1; y > 0; y--)
	{
		for(unsigned x = 0; x < (unsigned)surface->w; x++)
		{
			if(image_getPixelBool(surface, x, y))
			{
				return bottom;
			}
		}

		bottom--;
	}

	return bottom;
}
unsigned _image_cropLeft(SDL_Surface *surface)
{
	unsigned left = 0;

	for(unsigned x = 0; x < (unsigned)surface->w; x++)
	{
		for(unsigned y = 0; y < (unsigned)surface->h; y++)
		{
			if(image_getPixelBool(surface, x, y))
				return left;
		}

		left++;
	}

	return left;
}
unsigned _image_cropRight(SDL_Surface *surface)
{
	unsigned right = surface->w - 1;

	for(unsigned x = surface->w - 1; x > 0; x--)
	{
		for(unsigned y = 0; y < (unsigned)surface->h; y++)
		{
			if(image_getPixelBool(surface, x, y))
			{
				return right;
			}
		}

		right--;
	}

	return right;
}

SDL_Color image_getPixelColor(SDL_Surface *surface, unsigned x, unsigned y)
{
	Uint32 intColor = image_getPixelUint32(surface, x, y);
#if SDL_BYTEORDER != SDL_BIG_ENDIAN
	SDL_Color color = {(intColor & 0x00ff0000) / 0x10000,
			   (intColor & 0x0000ff00) / 0x100,
			   (intColor & 0x000000ff), 0};
#else
	SDL_Color color = {(intColor & 0x000000ff),
			   (intColor & 0x0000ff00) / 0x100,
			   (intColor & 0x00ff0000) / 0x10000, 0};
#endif

	return color;
}

unsigned image_getGreyscale(SDL_Color color)
{
	return 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
}

double image_getGreyscaleRatio(SDL_Color color)
{
	return image_getGreyscale(color) / 255;
}

unsigned image_getPixelBool(SDL_Surface *surface, unsigned x, unsigned y)
{
	SDL_Color color = image_getPixelColor(surface, x, y);

	return (color.r + color.g + color.b) / 3 < 128;
}

static inline Uint8 *pixelref(SDL_Surface *surf, unsigned x, unsigned y)
{
	SDL_LockSurface(surf);
	int bpp = surf->format->BytesPerPixel;
	SDL_UnlockSurface(surf);
	return (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 image_getPixelUint32(SDL_Surface *surface, unsigned x, unsigned y)
{
	Uint8 *p = pixelref(surface, x, y);
	switch(surface->format->BytesPerPixel) {
		case 1:
		return *p;
		case 2:
		return *(Uint16 *)p;
		case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		case 4:
			return *(Uint32 *)p;
	}
	return 0;
}

void image_putPixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
	SDL_LockSurface(surface);
	Uint8 *p = pixelref(surface, x, y);
	switch (surface->format->BytesPerPixel) {
	case 1:
		*p = pixel;
		break;
	case 2:
		*(Uint16 *)p = pixel;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
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
	SDL_UnlockSurface(surface);
}

void image_renderConsoleFromTo(SDL_Surface *surface, unsigned x1, unsigned y1,
			       unsigned x2, unsigned y2)
{
	for (unsigned y = y1; y < y2; y++) {
		for (unsigned x = x1; x < x2; x++) {
			if (image_getPixelBool(surface, x, y))
				printf("█");
			else
				printf(" ");
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
	image_renderConsoleFromTo(surface, imageLine.startX, imageLine.startY,
				  imageLine.endX, imageLine.endY);
}

void image_renderConsoleFromChar(SDL_Surface *surface,
				 struct ImageChar imageChar)
{
	image_renderConsoleFromTo(surface, imageChar.startX, imageChar.startY,
				  imageChar.endX, imageChar.endY);
}
