
#include <SDL/SDL.h>

#include "image.h"
#include "../types/structArrays.h"
#include "charDetection.h"

unsigned charDetection_line(SDL_Surface *surface, struct ImageLine *imageLine,
	unsigned startY)
{
	int topX = surface->w;
	int topY = surface->h;
	int bottomX, bottomY;
	bottomX = bottomY = -1;
	unsigned detectedLine = 0;
	for(int y = startY; y < surface->h; y++)
	{
		unsigned emptyLine = 1;
		for(int x = 0; x < surface->w; x++)
		{
			if(image_getPixelBool(surface, x, y))
			{
				emptyLine = 0;
				detectedLine = 1;
				if(topX > x)
				{
					topX = x;
				}
				if(topY > y)
				{
					topY = y;
				}
				if(bottomX < x)
				{
					bottomX = x;
				}
				if(bottomY < y)
				{
					bottomY = y;
				}
			}
		}

		// We're out of the current line, let's get out
		if(emptyLine && detectedLine)
		{
			break;
		}
	}

	// If we didn't detect anything
	if(detectedLine == 0)
	{
		return 0;
	}

    // We detected something, hura!
    imageLine->startX = topX;
    imageLine->startY = topY;
    imageLine->endX   = ++bottomX;
    imageLine->endY   = ++bottomY;

	return 1;
}

unsigned charDetection_char(SDL_Surface *surface, struct ImageLine imageLine,
	struct ImageChar *imageChar, unsigned startX)
{
	int topX = surface->w;
	int topY = surface->h;
	int bottomX, bottomY;
	bottomX = bottomY = -1;
	unsigned detectedChar = 0;
	for(int x = startX; x < (int)imageLine.endX; x++)
	{
		unsigned emptyColumn = 1;
		for(int y = imageLine.startY; y < (int)imageLine.endY; y++)
		{
			if(image_getPixelBool(surface, x, y))
			{
				emptyColumn = 0;
				detectedChar = 1;
				if(topX > x)
				{
					topX = x;
				}
				if(topY > y)
				{
					topY = y;
				}
				if(bottomX < x)
				{
					bottomX = x;
				}
				if(bottomY < y)
				{
					bottomY = y;
				}
			}
		}

		// We're out of the current line, let's get out
		if(emptyColumn && detectedChar)
		{
			break;
		}
	}

	// If we didn't detect anything
	if(detectedChar == 0)
	{
		return 0;
	}

	// We detected something, hura!
	imageChar->startX = topX;
	imageChar->startY = topY;
	imageChar->endX   = ++bottomX;
	imageChar->endY   = ++bottomY;

	return 1;
}

ImageLineArray charDetection_go(SDL_Surface *surface)
{
	ImageLineArray lineArray = new_ImageLineArray(4);

	struct ImageLine imageLine;
	unsigned startY = 0;
	unsigned x = 0;

	while(charDetection_line(surface, &imageLine, startY) == 1)
	{
		ImageCharArray charArray = new_ImageCharArray(4);

		struct ImageChar imageChar;
		unsigned startX = 0;
		unsigned y = 0;

		while(charDetection_char(surface, imageLine, &imageChar, startX) == 1)
		{
			startX = imageChar.endX;
			add_ImageCharArray(&charArray, y++, imageChar);
		}

		imageLine.chars = charArray;

		startY = imageLine.endY;
		add_ImageLineArray(&lineArray, x++, imageLine);
	}

	return lineArray;
}
