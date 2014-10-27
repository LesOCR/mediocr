#include <stddef.h>
#include <SDL/SDL.h>

#include "neuralNetwork.h"

#ifndef UTILS_NEURALNETWORK_CHARRECOGNITION_
#define UTILS_NEURALNETWORK_CHARRECOGNITION_

struct charRecognition {
	char letter;
	struct NeuralNetwork *network;
};

struct charRecognitionList {
	struct charRecognition *current;
	struct charRecognitionList *next;
};

struct charRecognitionList *charRecognition_learn(SDL_Surface *surface,
						  char chars[], size_t size);
char charRecognition_getChar(struct charRecognitionList *list,
			     SDL_Surface *surface);

#endif
