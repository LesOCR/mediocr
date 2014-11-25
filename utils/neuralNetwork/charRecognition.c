#include <SDL/SDL.h>
#include <math.h>

#include "neuralNetwork.h"
#include "../types/arrays.h"
#include "../image/image.h"
#include "charRecognition.h"

struct charRecognition *charRecognition_learn(SDL_Surface *surface,
						  char chars[], size_t size)
{
	struct charRecognition *charReg =
		malloc(sizeof(struct charRecognition));

	ImageLineArray imageLineArray = charDetection_go(surface);

	struct NeuralNetwork *myNeuralNetwork =
		neuralNetwork_main(256, 16, size);

	unsignedArray2D input = new_unsignedArray2D(size, 256);
	unsignedArray2D output = new_unsignedArray2D(size, size);

	for (unsigned i = 0; i < imageLineArray.size; i++) {
		for (unsigned j = 0;
			j < imageLineArray.elements[i].chars.size; j++) {
			SDL_Surface *s = image_scale(
				image_extractChar(
				surface, &imageLineArray.elements[i]
						.chars.elements[j]),
				16, 16);

			for (unsigned k = 0; k < 16; k++)
				for (unsigned l = 0; l < 16; l++)
					input.elements[j + i * 16]
						.elements[k + l * 16] =
							image_getPixelBool(s, k, l);

			for(unsigned h = 0; h < size; h++)
				output.elements[j + i * 16].elements[h] =
					(j + i * 16 == h);
		}
	}

	NeuralNetwork_train(myNeuralNetwork, input, output, 0.001, 0.001,
				0.0001);
	charReg->letters = chars;
	charReg->size    = size;
	charReg->network = myNeuralNetwork;

	return charReg;
}

char charRecognition_getChar(struct charRecognition *charReg,
			     SDL_Surface *surface)
{
	unsignedArray input = new_unsignedArray(256);

	for (unsigned k = 0; k < 16; k++)
		for (unsigned l = 0; l < 16; l++)
			input.elements[k + l * 16] =
			    image_getPixelBool(surface, k, l);

	double bestRatio = 0;
	char bestChar    = '\0';

	doubleArray ratio =
		NeuralNetwork_testDouble(charReg->network, input);

	for(unsigned h = 0; h < charReg->size; h++) {
		if (fabs(ratio.elements[h]) > bestRatio) {
			bestRatio = fabs(ratio.elements[h]);
			bestChar = charReg->letters[h];
		}
	}

	return bestChar;
}
