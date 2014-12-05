#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "file.h"

unsigned file_exists(char *path)
{
	return access(path, F_OK) != -1;
}

char *file_get_contents(char *path)
{
	char *buffer = 0;
	long length;
	FILE *f = fopen(path, "rb");

	if (!f)
	{
	  	err(1, "The file could not be found.");
	}

	fseek (f, 0, SEEK_END);
	length = ftell (f);
	fseek (f, 0, SEEK_SET);
	buffer = malloc (length);
	if (buffer)
	{
		fread (buffer, 1, length, f);
	}
	fclose (f);

	return buffer;
}
