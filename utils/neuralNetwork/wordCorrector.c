
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <limits.h>

#include "wordCorrector.h"

char *wordCorrector_correct(char *pathWordlist, char *word)
{
	if(strcmp(pathWordlist, "") == 0 || strlen(word) < 3)
		return word;

	FILE * fp;
   	char * line = NULL;
   	size_t len = 0;
   	int read;

   	fp = fopen(pathWordlist, "r");
   	if (fp == NULL)
       	err(1, "Dictionary file could not be found.");

	int lvBest = INT_MAX;
	unsigned bestWordLength = strlen(word);
	char *bestWord = malloc((bestWordLength + 1) * sizeof(char));
	strcpy(bestWord, word);

   	while ((read = getline_perso(&line, &len, fp)) != -1) {
		if(line[read - 1] == '\n')
			line[read-- - 1] = '\0';

		if(strlen(line) < 4)
			continue;

		int lv = wordCorrector_levenshtein(word, line);
		if(lv < lvBest) {
			lvBest = lv;

			if(strlen(line) > bestWordLength) {
				bestWordLength = strlen(line);
				bestWord = realloc(bestWord, (bestWordLength + 1) * sizeof(char));
			}

			strcpy(bestWord, line);
		}

		if(lv == 0)
			break;
   	}

   	fclose(fp);

	if(lvBest > 5)
		return word;

	return bestWord;
}


#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

int wordCorrector_levenshtein(char *s1, char *s2)
{
    unsigned int s1len, s2len, x, y, lastdiag, olddiag;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int column[s1len+1];
    for (y = 1; y <= s1len; y++)
        column[y] = y;
    for (x = 1; x <= s2len; x++) {
        column[0] = x;
        for (y = 1, lastdiag = x-1; y <= s1len; y++) {
            olddiag = column[y];
            column[y] = MIN3(column[y] + 1, column[y-1] + 1, lastdiag + (s1[y-1] == s2[x-1] ? 0 : 1));
            lastdiag = olddiag;
        }
    }
    return(column[s1len]);
}

size_t getline_perso(char **lineptr, size_t *n, FILE *stream) {
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;

    if (lineptr == NULL) {
    	return -1;
    }
    if (stream == NULL) {
    	return -1;
    }
    if (n == NULL) {
    	return -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) {
    	return -1;
    }
    if (bufptr == NULL) {
    	bufptr = malloc(128);
    	if (bufptr == NULL) {
    		return -1;
    	}
    	size = 128;
    }
    p = bufptr;
    while(c != EOF) {
    	if ((unsigned long)(p - bufptr) > (size - 1)) {
    		size = size + 128;
    		bufptr = realloc(bufptr, size);
    		if (bufptr == NULL) {
    			return -1;
    		}
    	}
    	*p++ = c;
    	if (c == '\n') {
    		break;
    	}
    	c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}
