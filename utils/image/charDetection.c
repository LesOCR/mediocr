#include <SDL/SDL.h>

#include "image.h"
#include "convolution.h"
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
			    struct ImageChar *imageChar, unsigned startX)
{
	int topX = surface->w;
	int topY = surface->h;
	int bottomX, bottomY;
	bottomX = bottomY = -1;
	unsigned detectedChar = 0;
	for (int x = startX; x < (int)imageLine.endX; x++) {
		unsigned emptyColumn = 1;
		for (int y = imageLine.startY; y < (int)imageLine.endY; y++) {
			if (image_getPixelBool(surface, x, y)) {
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

		// We're out of the current line, let's get out
		if (emptyColumn && detectedChar)
			break;
	}

	// If we didn't detect anything
	if (detectedChar == 0)
		return 0;

	// We detected something, hura!
	imageChar->startX = topX;
	imageChar->startY = topY;
	imageChar->endX = ++bottomX;
	imageChar->endY = ++bottomY;

	return 1;
}


ImageBlockArray charDetection_blocks(SDL_Surface *surface)
{
	int matrix[9] = {
		1, 1, 1,
		1, 1, 1,
		1, 1, 1
	};
	SDL_Surface *blurredSurface = convolution_apply(surface, &matrix[0], 9);

	ImageBlockArray blockArray = new_ImageBlockArray(1);
	struct ImageBlock imageBlock;

	unsigned first = 1;
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
				if(first)
				{
					first = 0;
					break;
				}

				imageBlock.startX = topX;
				imageBlock.endX   = bottomX;
				imageBlock.startY = topY;
				imageBlock.endY   = bottomY;

				imageBlock.lines = charDetection_go(
					surface, topX, topY, bottomX, bottomY
				);

				printf("blocks: %d\n", blockArray.size);
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
		unsigned startX = 0;

		while (charDetection_char(surface, imageLine, &imageChar,
					  startX) == 1) {
			startX = imageChar.endX;
			push_ImageCharArray(&charArray, imageChar);
		}

		imageLine.chars = charArray;

		topY = imageLine.endY;
		push_ImageLineArray(&lineArray, imageLine);
	}

	return lineArray;
}
