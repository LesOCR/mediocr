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
			    struct ImageChar *imageChar, unsigned startX, unsigned start,
				unsigned spaceSize)
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
		if ((emptyColumn && detectedChar) || spaceCount > spaceSize)
			break;
	}

	// If we didn't detect anything
	if (detectedChar == 0 && spaceCount <= spaceSize)
		return 0;

	// We detected something, hura!
	imageChar->space = 0;
	imageChar->startX = topX;
	imageChar->startY = topY;
	imageChar->endX = ++bottomX;
	imageChar->endY = ++bottomY;

	if(spaceCount > spaceSize) {
        imageChar->space = 1;
        imageChar->endX  = startX + spaceCount + 1;
	}

	return 1;
}

ImageBlockArray charDetection_blocks(SDL_Surface *s)
{
	ImageBlockArray blockArray = new_ImageBlockArray(1);

    s                          = filter_binary(s, 128);
    SDL_Surface *binarySurface = filter_createGroup(s);

	unsigned *map = calloc(s->w * s->h, sizeof(unsigned));
	for(int y = 0; y < s->h; y++)
	{
		for(int x = 0; x < s->w; x++)
		{
			map[x + y * s->w] = image_getPixelBool(binarySurface, x, y);
		}
	}

	for(int y = 0; y < s->h; y++)
	{
		for(int x = 0; x < s->w; x++)
		{
			if(map[x + y * s->w])
			{
				int topX = binarySurface->w;
				int topY = binarySurface->h;
				int bottomX = -1;
				int bottomY = -1;

				charDetection_blockRec(map, binarySurface->w, binarySurface->h,
					x, y, &topX, &topY, &bottomX, &bottomY);

				struct ImageBlock imageBlock;
				imageBlock.startX = topX;
				imageBlock.endX   = bottomX;
				imageBlock.startY = topY;
				imageBlock.endY   = bottomY;
				imageBlock.lines = charDetection_go(
					s, topX, topY, bottomX, bottomY
				);

				push_ImageBlockArray(&blockArray, imageBlock);
			}
		}
	}

	return blockArray;
}

void charDetection_blockRec(unsigned *map, int width, int height, int x, int y,
	int *topX, int *topY, int *bottomX, int *bottomY)
{
	int curSize = 4;
	int curPosition = 3;
	struct BlockStack *stack = malloc(curSize * sizeof(struct BlockStack));
	stack[0] = (struct BlockStack){
		.x = x + 1, .y = y
	};
	stack[1] = (struct BlockStack){
		.x = x, .y = y + 1
	};
	stack[2] = (struct BlockStack){
		.x = x - 1, .y = y
	};
	stack[3] = (struct BlockStack){
		.x = x, .y = y - 1
	};
	while(curPosition >= 0)
	{
		struct BlockStack cur = stack[curPosition--];

		if(cur.x < 0 || cur.x >= width ||
			cur.y < 0 || cur.y >= height ||
			!map[cur.x + cur.y * width])
			continue;

		map[cur.x + cur.y * width] = 0;

		if(cur.x < *topX)
			*topX = x;
		if(cur.y < *topY)
			*topY = cur.y;
		if(cur.x > *bottomX)
			*bottomX = cur.x;
		if(cur.y > *bottomY)
			*bottomY = cur.y;

		if(curSize <= curPosition + 4)
		{
			curSize += 4;
			stack = realloc(stack, curSize * sizeof(struct BlockStack));
		}

		stack[++curPosition] = (struct BlockStack){
			.x = cur.x + 1, .y = cur.y
		};
		stack[++curPosition] = (struct BlockStack){
			.x = cur.x, .y = cur.y + 1
		};
		stack[++curPosition] = (struct BlockStack){
			.x = cur.x - 1, .y = cur.y
		};
		stack[++curPosition] = (struct BlockStack){
			.x = cur.x, .y = cur.y - 1
		};
	}
	free(stack);
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
		unsigned spaceSizeSum = 0;
		unsigned spaceCount = 0;

		while (charDetection_char(surface, imageLine, &imageChar,
					  startX, start, (!start) ? (spaceSizeSum / spaceCount) / 1.5 : 0) == 1) {
			spaceSizeSum += imageChar.endX - startX;
			spaceCount++;
			startX = imageChar.endX;
			start = 0;
			if(imageChar.space)
				start = 1;
			push_ImageCharArray(&charArray, imageChar);
		}

		imageLine.chars = charArray;

		topY = imageLine.endY;
		push_ImageLineArray(&lineArray, imageLine);
	}

	return lineArray;
}
