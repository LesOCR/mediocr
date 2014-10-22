
#include <SDL/SDL.h>

#include "NeuralNetwork.h"
#include "../types/arrays.h"
#include "../image/image.h"
#include "charRecognition.h"

struct charRecognitionList *charRecognition_learn(SDL_Surface *surface,
	char chars[], size_t size)
{
	struct charRecognitionList *charRegList =
		malloc(sizeof(struct charRecognitionList));

	struct charRecognitionList *firstList = charRegList;

	for(unsigned h = 0; h < size; h++)
	{
		struct NeuralNetwork *myNeuralNetwork = neuralNetwork_main(128, 2, 1);

		unsignedArray2D input  = new_unsignedArray2D(size, 128);
		unsignedArray2D output = new_unsignedArray2D(size, 1);

		ImageLineArray imageLine = charDetection_go(surface);
		for(unsigned i = 0; i < imageLine.size; i++) {
			for(unsigned j = 0; j < imageLine.elements[i].chars.size; j++) {
				SDL_Surface *s = image_extractChar(surface,
						&imageLine.elements[i].chars.elements[j]);
				for(unsigned k = 0; k < 16; k++)
				{
					for(unsigned l = 0; l < 16; l++)
					{
						input.elements[j + i * 16].elements[k + l * 16] =
							image_getPixelBool(s, k, l);

						output.elements[j + i * 15].elements[0] = (j + i * 16 == 0);
					}
				}
			}
		}

		NeuralNetwork_train(myNeuralNetwork, input, output, 100, 0.1, 0);

		struct charRecognition charReg;
		charReg.network = myNeuralNetwork;
		charReg.letter  = chars[h];

		charRegList->current = charReg;

		struct charRecognitionList *newCharRegList =
			malloc(sizeof(struct charRecognitionList));
		charRegList->next = newCharRegList;

		charRegList = newCharRegList;
		charRegList->next = NULL;
	}

	return firstList;
}

char charRecognition_getChar(struct charRecognitionList *list,
	SDL_Surface *surface)
{
	double bestRatio = 0;
	char   bestChar = '\0';

	unsignedArray input  = new_unsignedArray(128);

	for(unsigned k = 0; k < 16; k++)
	{
		for(unsigned l = 0; l < 16; l++)
		{
			input.elements[k + l * 16] =
				image_getPixelBool(surface, k, l);
		}
	}

	while(list != NULL)
	{
		doubleArray ratio =
			NeuralNetwork_testDouble(list->current.network, input);
		print_doubleArray(ratio);
		if(abs(ratio.elements[0]) > bestRatio)
		{
			bestRatio = abs(ratio.elements[0]);
			bestChar  = list->current.letter;
		}

		list = list->next;
	}

	return bestChar;
}
