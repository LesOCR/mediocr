
#include <SDL/SDL.h>

#include "image.h"
#include "charDetection.h"

unsigned charDetection_line(SDL_Surface *surface, struct ImageLine *imageLine,
	unsigned startY)
{
	int topX = surface->w;
	int topY = surface->h;
	int bottomX, bottomY = -1;
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
