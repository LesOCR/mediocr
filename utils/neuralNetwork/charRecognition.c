
#include <SDL/SDL.h>
#include <math.h>

#include "NeuralNetwork.h"
#include "../types/arrays.h"
#include "../image/image.h"
#include "charRecognition.h"

struct charRecognitionList *charRecognition_learn(SDL_Surface *surface,
	char chars[], size_t size)
{
	size++;
	struct charRecognitionList *charRegList =
		malloc(sizeof(struct charRecognitionList));

	struct charRecognitionList *firstList = charRegList;

	for(unsigned h = 0; h < size - 1; h++)
	{
		struct NeuralNetwork *myNeuralNetwork = neuralNetwork_main(128, 2, 1);

		unsignedArray2D input  = new_unsignedArray2D(size, 128);
		unsignedArray2D output = new_unsignedArray2D(size, 1);

		ImageLineArray imageLine = charDetection_go(surface);
		for(unsigned i = 0; i < imageLine.size; i++) {
			for(unsigned j = 0; j < imageLine.elements[i].chars.size; j++) {
				SDL_Surface *s = image_scale(image_extractChar(surface,
						&imageLine.elements[i].chars.elements[j]), 16, 16);
				for(unsigned k = 0; k < 16; k++)
				{
					for(unsigned l = 0; l < 16; l++)
					{
						input.elements[j + i * 16].elements[k + l * 16] =
							image_getPixelBool(s, k, l);

						output.elements[j + i * 16].elements[0] = (j + i * 16 == h);
					}
				}
			}
		}

		NeuralNetwork_train(myNeuralNetwork, input, output, 10000, 0.1, 0);


		struct charRecognition *charReg = malloc(sizeof(struct charRecognition));
		charReg->network = myNeuralNetwork;
		charReg->letter  = chars[h];

		charRegList->current = charReg;

		struct charRecognitionList *newCharRegList =
			malloc(sizeof(struct charRecognitionList));
		charRegList->next = newCharRegList;

		charRegList = newCharRegList;
		charRegList->next = NULL;

		printf("char %c learned!\n", chars[h]);
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

	printf("Generated input of the image\n");

	while(list->next != NULL)
	{
		doubleArray ratio =
			NeuralNetwork_testDouble(list->current->network, input);
		if(fabs(ratio.elements[0]) > bestRatio)
		{
			bestRatio = fabs(ratio.elements[0]);
			bestChar  = list->current->letter;
		}

		list = list->next;
	}

	return bestChar;
}
