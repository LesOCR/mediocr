#include <SDL/SDL.h>
#include <math.h>

#include "../helpers/strings.h"
#include "../helpers/file.h"
#include "neuralNetwork.h"
#include "../types/arrays.h"
#include "../image/image.h"
#include "charRecognition.h"

struct charRecognition *charRecognition_learn(char *rootPath,
						  char chars[], size_t size, size_t variants)
{
	struct charRecognition *charReg =
		malloc(sizeof(struct charRecognition));

	struct NeuralNetwork *myNeuralNetwork =
		neuralNetwork_main(256, 350, size);

	unsignedArray2D input = new_unsignedArray2D(size * variants, 256);
	unsignedArray2D output = new_unsignedArray2D(size * variants, size);

	unsigned count = 0;
	for(unsigned i = 0; i < size; i++) {
		for(unsigned j = 0; j < variants; j++) {
			char toAscii[15];
			sprintf(toAscii, "%d", (int)chars[i]);
			char *path = string_concat(rootPath, toAscii);
			path = string_concat(path, "/");
			char filename[5];
			sprintf(filename, "%d", j);
			path = string_concat(path, filename);
			path = string_concat(path, ".bmp");
			SDL_Surface *s = image_scale(
				image_crop(image_load(path)),
				16, 16);

			for (unsigned k = 0; k < 16; k++)
				for (unsigned l = 0; l < 16; l++)
					input.elements[count].elements[k + l * 16] =
						image_getPixelBool(s, k, l);

			for(unsigned k = 0; k < size; k++)
				output.elements[count].elements[k] = ((count % size) == k);

			count++;
		}
	}

	NeuralNetwork_train(myNeuralNetwork, input, output, 0.000000001, 0.1,
				0.9);
	charReg->letters = chars;
	charReg->size    = size;
	charReg->network = myNeuralNetwork;

	return charReg;
}

struct charRecognition *charRecognition_learnWeights(char *pathIn, char *pathOut,
	size_t size)
{
	struct charRecognition *charReg =
		malloc(sizeof(struct charRecognition));

	struct NeuralNetwork *myNeuralNetwork =
		neuralNetwork_main(256, 6, size);

	NeuralNetwork_loadWeightInput(myNeuralNetwork, file_get_contents(pathIn));
	NeuralNetwork_loadWeightOutput(myNeuralNetwork, file_get_contents(pathOut));

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
