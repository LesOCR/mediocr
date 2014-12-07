
#include <stdlib.h>
#include <stdio.h>

#ifndef UTILS_NEURALNETWORK_WORDCORRECTOR_
#define UTILS_NEURALNETWORK_WORDCORRECTOR_

	char *wordCorrector_correct(char *pathWordlist, char *word);
	int wordCorrector_levenshtein(char *s1, char *s2);
	size_t getline_perso(char **lineptr, size_t *n, FILE *stream);

#endif
