#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <time.h>
#include <SDL/SDL.h>
#include <unistd.h>
#include <getopt.h>

#include "utils/helpers/strings.h"
#include "utils/neuralNetwork/neuralNetwork.h"
#include "utils/neuralNetwork/charRecognition.h"
#include "utils/types/arrays.h"
#include "utils/image/image.h"
#include "utils/image/convolution.h"
#include "utils/image/filters.h"
#include "utils/image/charDetection.h"
#include "utils/types/structArrays.h"

int setup()
{
	srand(time(NULL));

	return 1;
}


int startLearning(char *charsInput, char *characters)
{
	struct charRecognition *charRecog =
	    charRecognition_learn(charsInput, characters, strlen(characters), 1);

	printf("Serialized input weights: \n%s\n", NeuralNetwork_serializeWeightsInput(charRecog->network));
	printf("Serialized output weights: \n%s\n", NeuralNetwork_serializeWeightsOutput(charRecog->network));

	return 1;
}

int startNeuralNetworkChar(char *pathIn, char *pathOut, size_t size, char *read)
{
	struct charRecognition *charRecog = charRecognition_learnWeights(pathIn, pathOut, size);

	SDL_Surface *text = image_load(read);
	ImageBlockArray imageBlock = charDetection_blocks(text);
	printf("Recognized string: \n");
	for(unsigned h = 0; h < imageBlock.size; h++) {
		printf("\n");
		ImageLineArray imageLine = imageBlock.elements[h].lines;
		for (unsigned i = 0; i < imageLine.size; i++) {
			for (unsigned j = 0; j < imageLine.elements[i].chars.size;
				j++) {
				struct ImageChar imageChar = imageLine.elements[i].chars.elements[j];
				SDL_Surface *s = image_scale(
					image_extractChar(
						text, &imageChar),
					16, 16);

				imageChar.content = charRecognition_getChar(charRecog, s);

				printf("%c", charRecognition_getChar(charRecog, s));
			}
		}
	}

	printf("\n");

	return 1;
}

int startImageProcessing(char *path)
{
	SDL_Surface *surface = image_load(path);

	image_display(surface);

	image_display(filter_createGroup(surface));

	return 1;
}

void outputHelp()
{
	printf("----------------------------- MediOCR "
	       "----------------------------------\n");
	printf(" Authors: Manuel HUEZ, Louis-Paul DAREAU, Erenus DERMANCI, "
	       "Cyril CHEMLA\n");
	printf(" Version: 1.0.0.0\n\n");
	printf(" Options:\n");
	printf(" -m: Mode: [live|learn|chardetection]\n");
	printf(" -f: Path of the file that needs to be processed.\n");
	printf(" -w: Path of the directory containing the weights.\n");
	printf(" -c: Path to the directory containing the letters used to learn.\n");
	printf(" -s: String containing the chars in the file used to learn.\n");
	printf(" -h: Show this wonderful help.\n");
	printf("---------------------------------------------------------------"
	       "---------\n");

	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	int c;
    char *mode       = "live";
    char *filePath   = "data/text/alphabetfonts.bmp";
    char *charPath   = "data/letters/";
    char *weightsIn  = "data/weights/in.mediocr";
    char *weightsOut = "data/weights/out.mediocr";
    char *charList   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	while ((c = getopt(argc, argv, "m:f:wi:wo:c:s:h")) != -1) {
		switch (c) {
		case 'm':
			mode = optarg;
			break;
		case 'f':
			filePath = optarg;
			break;
		case 'w':
			weightsIn = string_concat(optarg, "/in.mediocr");
			weightsOut = string_concat(optarg, "/out.mediocr");
		case 'c':
			charPath = optarg;
			break;
		case 's':
			charList = optarg;
			break;
		case 'h':
		default:
			outputHelp();
		}
	}

	if (!setup())
		err(1, "Error during the initial setup.");

	if (strcmp(mode, "chardetection") == 0) {
		if (!startImageProcessing(filePath))
			err(1, "Error during the image processing.");
	} else if(strcmp(mode, "learn") == 0) {
		if (!startLearning(charPath, charList))
			err(1, "Error during the learn stage.");
	} else {
		if (!startNeuralNetworkChar(weightsIn, weightsOut, strlen(charList), filePath))
			err(1, "Error during the real neural network instance");
	}

	printf("MediOCR ended! \n");

	return EXIT_SUCCESS;
}
