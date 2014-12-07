#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "file.h"

unsigned file_exists(char *path)
{
	return access(path, F_OK) != -1;
}

char *file_get_content(char *path)
{
	char *buffer = 0;
	long length;
	FILE *f = fopen(path, "rb");

	if (!f)
	{
	  	err(1, "The file could not be found [%s].", path);
	}

	fseek (f, 0, SEEK_END);
	length = ftell (f);
	fseek (f, 0, SEEK_SET);
	buffer = malloc (length);
	if (buffer)
	{
		// Freaking -Werror
		int tmp = fread (buffer, 1, length, f);
		tmp++;
	}
	fclose (f);

	return buffer;
}

void file_put_content(char *path, char *content)
{
	FILE *f = fopen(path, "w");
	if (f == NULL)
	{
	    err(1, "The file could not be opened.");
	    exit(1);
	}

	fprintf(f, "%s", content);
	fclose(f);
}
