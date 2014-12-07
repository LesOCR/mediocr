#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <time.h>
#include <SDL/SDL.h>
#include <unistd.h>
#include <getopt.h>

#include "utils/helpers/strings.h"
#include "utils/helpers/file.h"
#include "utils/neuralNetwork/neuralNetwork.h"
#include "utils/neuralNetwork/charRecognition.h"
#include "utils/image/image.h"
#include "utils/image/filters.h"
#include "utils/image/charDetection.h"

int setup()
{
	srand(time(NULL));

	return 1;
}


int startLearning(char *charsInput, char *characters, int variant, char *pathIn, char *pathOut)
{
	struct charRecognition *charRecog =
	    charRecognition_learn(charsInput, characters, strlen(characters), variant);

	printf("Serializing input weights...\n");
	char *weightsInput = NeuralNetwork_serializeWeightsInput(charRecog->network);
	printf("Serializing output weights...\n");
	char *weightsOutput = NeuralNetwork_serializeWeightsOutput(charRecog->network);

	printf("Storing...\n");

	file_put_content(pathIn, weightsInput);
	file_put_content(pathOut, weightsOutput);

	return 1;
}

int testBlock(char *read)
{
	charDetection_blocks(image_load(read));

	return 1;
}

int startNeuralTest(char *charsInput, char *characters, int variant, char *read, char *dic)
{
	struct charRecognition *charRecog =
	    charRecognition_learn(charsInput, characters, strlen(characters), variant);

	SDL_Surface *text = image_load(read);
	printf("Recognized text: \n%s", charRecognition_getText(charRecog, text, dic));

	return 1;
}

int startLive(char *pathIn, char *pathOut, char *characters, char *read, char *dic)
{
	struct charRecognition *charRecog = charRecognition_learnWeights(pathIn,
		pathOut, characters, strlen(characters));
	SDL_Surface *text = image_load(read);
	printf("%s", charRecognition_getText(charRecog, text, dic));

	return 1;
}

int startImageProcessing(char *path)
{
	SDL_Surface *surface = filter_binary(image_load(path), 128);

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
	printf(" Version: 0.2.0.0\n\n");
	printf(" Options:\n");
	printf(" -m: Mode: [live|learn|learnNprocess|imageprocessing|block]\n");
	printf(" -f: Path of the file that needs to be processed.\n");
	printf(" -d: Path of the dictionary used to correct words. Can be empty.\n");
	printf(" -w: Path of the directory containing the weights.\n");
	printf(" -c: Path to the directory containing the letters used to learn.\n");
	printf(" -s: String containing the chars in the file used to learn.\n");
	printf(" -v: Number of fonts variants to be learned.\n");
	printf(" -h: Show this wonderful help.\n");
	printf("---------------------------------------------------------------"
	       "---------\n");

	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	int c;
    char *mode       = "";
    char *filePath   = "data/text/fullhard.bmp";
	char *dictionaryPath = "data/words/french.txt";
	unsigned passedFilePath = 1;
    char *charPath   = "data/letters/";
    char *weightsIn  = "data/weights/in.mediocr";
    char *weightsOut = "data/weights/out.mediocr";
    char *charList   = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	int fontVariant  = 7;

	while ((c = getopt(argc, argv, "m:f:d:wi:wo:c:s:v:h")) != -1) {
		switch (c) {
		case 'm':
			mode = optarg;
			break;
		case 'f':
			passedFilePath = 1;
			filePath = optarg;
			break;
		case 'd':
			dictionaryPath = optarg;
			break;
		case 'w':
			weightsIn = string_concat(optarg, "/in.mediocr");
			weightsOut = string_concat(optarg, "/out.mediocr");
			break;
		case 'c':
			charPath = optarg;
			break;
		case 's':
			charList = optarg;
			break;
		case 'v':
			fontVariant = atoi(optarg);
			break;
		case 'h':
		default:
			outputHelp();
		}
	}

	if (!setup())
		err(1, "Error during the initial setup.");

	if (strcmp(mode, "imageprocessing") == 0) {
		if (!startImageProcessing(filePath))
			err(1, "Error during the image processing.");
	} else if(strcmp(mode, "block") == 0) {
		if (!testBlock(filePath))
			err(1, "Error during the block detection.");
	} else if(strcmp(mode, "learnNprocess") == 0) {
		if (!startNeuralTest(charPath, charList, fontVariant, filePath, dictionaryPath))
			err(1, "Error during the testing of the neural network.");
	} else if(strcmp(mode, "learn") == 0) {
		if (!startLearning(charPath, charList, fontVariant, weightsIn, weightsOut))
			err(1, "Error during the learn stage.");
	} else if((strcmp(mode, "live") == 0 || strcmp(mode, "") == 0) && passedFilePath) {
		if (!startLive(weightsIn, weightsOut, charList, filePath, dictionaryPath))
			err(1, "An error happened. Please make sure to use the latest version of MediOCR.");
	} else {
		outputHelp();
	}

	return EXIT_SUCCESS;
}
