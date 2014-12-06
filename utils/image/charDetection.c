#include <SDL/SDL.h>

#include "image.h"
#include "convolution.h"
#include "filters.h"
#include "../types/structArrays.h"
#include "charDetection.h"

unsigned charDetection_line(SDL_Surface *surface, struct ImageLine *imageLine,
	unsigned startX, unsigned stopX, unsigned startY, unsigned stopY)
{
	int topX = surface->w;
	int topY = surface->h;
	int bottomX, bottomY;
	bottomX = bottomY = -1;
	unsigned detectedLine = 0;
	for (int y = startY; y < (int)stopY; y++) {
		unsigned emptyLine = 1;
		for (int x = startX; x < (int)stopX; x++) {
			if (image_getPixelBool(surface, x, y)) {
				emptyLine = 0;
				detectedLine = 1;
				if (topX > x)
					topX = x;
				if (topY > y)
					topY = y;
				if (bottomX < x)
					bottomX = x;
				if (bottomY < y)
					bottomY = y;
			}
		}

		// We're out of the current line, let's get out
		if (emptyLine && detectedLine)
			break;
	}

	// If we didn't detect anything
	if (detectedLine == 0)
		return 0;

	// We detected something, hura!
	imageLine->startX = topX;
	imageLine->startY = topY;
	imageLine->endX = ++bottomX;
	imageLine->endY = ++bottomY;

	return 1;
}

unsigned charDetection_char(SDL_Surface *surface, struct ImageLine imageLine,
			    struct ImageChar *imageChar, unsigned startX, unsigned start)
{
	int topX = surface->w;
	int topY = surface->h;
	int bottomX, bottomY;
	bottomX = bottomY = -1;
	unsigned detectedChar = 0;
	unsigned spaceCount = 0;
	for (int x = startX; x < (int)imageLine.endX; x++) {
		unsigned emptyColumn = 1;
		for (int y = imageLine.startY; y < (int)imageLine.endY; y++) {
			if (image_getPixelBool(surface, x, y)) {
				spaceCount = 0;
				emptyColumn = 0;
				detectedChar = 1;
				if (topX > x)
					topX = x;
				if (topY > y)
					topY = y;
				if (bottomX < x)
					bottomX = x;
				if (bottomY < y)
					bottomY = y;
			}
		}

		if(emptyColumn && !start) {
			spaceCount++;
		}

		// We're out of the current line, let's get out
		if ((emptyColumn && detectedChar) || spaceCount > 5)
			break;
	}

	// If we didn't detect anything
	if (detectedChar == 0 && spaceCount <= 5)
		return 0;

	// We detected something, hura!
	imageChar->space = 0;
	imageChar->startX = topX;
	imageChar->startY = topY;
	imageChar->endX = ++bottomX;
	imageChar->endY = ++bottomY;

	if(spaceCount > 5) {
        imageChar->space = 1;
        imageChar->endX  = startX + spaceCount + 1;
	}

	return 1;
}


ImageBlockArray charDetection_blocks(SDL_Surface *surface)
{
	SDL_Surface *blurredSurface = filter_createGroup(surface);
	blurredSurface = filter_blur(blurredSurface);
	blurredSurface = filter_createGroup(blurredSurface);
	blurredSurface = filter_blur(blurredSurface);
	blurredSurface = filter_createGroup(blurredSurface);

	ImageBlockArray blockArray = new_ImageBlockArray(1);
	struct ImageBlock imageBlock;

	for(unsigned x = 0; x < (unsigned)blurredSurface->w; x++)
	{
		for(unsigned y = 0; y < (unsigned)blurredSurface->h; y++)
		{
			int topX = surface->w;
			int topY = surface->h;
			int bottomX = -1;
			int bottomY = -1;

			charDetection_blockRec(blurredSurface, x, y, &topX, &topY,
				&bottomX, &bottomY);

			// The following ifs are wonderful hacks
			if(topX != surface->w || topY != surface->h || bottomX != -1 || bottomY != -1)
			{
				imageBlock.startX = topX;
				imageBlock.endX   = bottomX;
				imageBlock.startY = topY;
				imageBlock.endY   = bottomY;

				imageBlock.lines = charDetection_go(
					surface, topX, topY, bottomX, bottomY
				);

				push_ImageBlockArray(&blockArray, imageBlock);
			}
		}
	}

	return blockArray;
}

void charDetection_blockRec(SDL_Surface *surface, int startX, int startY,
	int *topX, int *topY, int *bottomX, int *bottomY)
{
	if(startX < 0 || startX > surface->w || startY < 0 || startY > surface->h ||
		!image_getPixelBool(surface, startX, startY))
		return;

	image_putPixel(surface, startX, startY, SDL_MapRGBA(surface->format,
		255, 255, 255, 0));


	if(startX < *topX)
		*topX = startX;
	if(startY < *topY)
		*topY = startY;
	if(startX > *bottomX)
		*bottomX = startX;
	if(startY > *bottomY)
		*bottomY = startY;

	charDetection_blockRec(surface, startX + 1, startY, topX, topY, bottomX, bottomY);
	charDetection_blockRec(surface, startX - 1, startY, topX, topY, bottomX, bottomY);
	charDetection_blockRec(surface, startX    , startY + 1, topX, topY, bottomX, bottomY);
	charDetection_blockRec(surface, startX    , startY - 1, topX, topY, bottomX, bottomY);
}


ImageLineArray charDetection_go(SDL_Surface *surface, unsigned topX, unsigned topY,
	unsigned bottomX, unsigned bottomY)
{
	ImageLineArray lineArray = new_ImageLineArray(1);

	struct ImageLine imageLine;

	while (charDetection_line(surface, &imageLine, topX, bottomX, topY, bottomY) == 1) {
		ImageCharArray charArray = new_ImageCharArray(1);

		struct ImageChar imageChar;
		unsigned startX = topX;
		unsigned start = 1;

		while (charDetection_char(surface, imageLine, &imageChar,
					  startX, start) == 1) {
			startX = imageChar.endX;
			start = 0;
			push_ImageCharArray(&charArray, imageChar);
		}

		imageLine.chars = charArray;

		topY = imageLine.endY;
		push_ImageLineArray(&lineArray, imageLine);
	}

	return lineArray;
}
