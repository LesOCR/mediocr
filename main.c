#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <time.h>
#include <SDL/SDL.h>
#include <unistd.h>
#include <getopt.h>

#include "utils/neuralNetwork/neuralNetwork.h"
#include "utils/neuralNetwork/charRecognition.h"
#include "utils/types/arrays.h"
#include "utils/image/image.h"
#include "utils/image/convolution.h"
#include "utils/image/charDetection.h"
#include "utils/types/structArrays.h"

int setup()
{
	srand(time(NULL));

	return 1;
}

int testMultiNeuralNetwork()
{
	struct NeuralNetwork *myNeuralNetwork = neuralNetwork_main(4, 2, 2);

	unsignedArray2D input = new_unsignedArray2D(4, 4);
	unsignedArray2D output = new_unsignedArray2D(4, 2);

	input.elements[0].elements[0] = 1;
	input.elements[0].elements[1] = 0;
	input.elements[0].elements[2] = 0;
	input.elements[0].elements[3] = 0;

	input.elements[1].elements[0] = 0;
	input.elements[1].elements[1] = 1;
	input.elements[1].elements[2] = 0;
	input.elements[1].elements[3] = 0;

	input.elements[2].elements[0] = 0;
	input.elements[2].elements[1] = 0;
	input.elements[2].elements[2] = 1;
	input.elements[2].elements[3] = 0;

	input.elements[3].elements[0] = 0;
	input.elements[3].elements[1] = 0;
	input.elements[3].elements[2] = 0;
	input.elements[3].elements[3] = 1;

	output.elements[0].elements[0] = 1;
	output.elements[0].elements[1] = 0;
	output.elements[1].elements[0] = 1;
	output.elements[1].elements[1] = 0;
	output.elements[2].elements[0] = 0;
	output.elements[2].elements[1] = 1;
	output.elements[3].elements[0] = 0;
	output.elements[3].elements[1] = 1;

	NeuralNetwork_train(myNeuralNetwork, input, output, 0.00001, 0.5, 0.1);

	NeuralNetwork_test(myNeuralNetwork, input);

	char *serializedInput =
	NeuralNetwork_serializeWeightsInput(myNeuralNetwork);
	char *serializedOutput =
	NeuralNetwork_serializeWeightsOutput(myNeuralNetwork);
	printf("Serialized input weights: \n%s\n", serializedInput);
	printf("Serialized output weights: \n%s\n", serializedOutput);
	printf("Unserializing:\n");
	NeuralNetwork_loadWeightInput(myNeuralNetwork, serializedInput);
	printf("Done! (loaded into the neural network)\n");

	return 1;
}

int startNeuralNetworkChar(char *charsInput, char *characters, char *read)
{
	SDL_Surface *surface = image_load(charsInput);

	struct charRecognition *charRecog =
	    charRecognition_learn(surface, characters, strlen(characters));

	SDL_Surface *text = image_load(read);
	ImageLineArray imageLine = charDetection_go(text);
	printf("Recognized string: \n");
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
	printf("\n");

	return 1;
}

int startImageProcessing(char *path)
{
	SDL_Surface *surface = image_load(path);

	image_display(surface);

	int matrix[9] = {
		1, 1, 1,
		1, 1, 1,
		1, 1, 1
	};
	surface = convolution_apply(surface, &matrix[0], 9);

	image_display(surface);

	// ImageLineArray imageLine = charDetection_go(surface);
	//
	// for (unsigned i = 0; i < imageLine.size; i++)
	// 	for (unsigned j = 0; j < imageLine.elements[i].chars.size;
	// 	     j++) {
	// 		SDL_Surface *s = image_extractChar(
	// 		    surface, &imageLine.elements[i].chars.elements[j]);
	// 		image_renderConsole(image_scale(s, 16, 16));
	// 		printf("\n");
	// 	}

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
	printf(" -m: Mode: [live|neuralnetwork|chardetection]\n");
	printf(" -f: Path of the file that needs to be processed.\n");
	printf(" -c: Path of the file used by the neural network to learn.\n");
	printf(" -s: String containing the chars in the file used to learn.\n");
	printf(" -h: Show this wonderful help.\n");
	printf("---------------------------------------------------------------"
	       "---------\n");

	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	int c;
	char *mode = "live";
	char *filePath = NULL;
	char *charPath = "data/text/caps.bmp";
	char *charList = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	while ((c = getopt(argc, argv, "m:f:c:s:h")) != -1) {
		switch (c) {
		case 'm':
			mode = optarg;
			break;
		case 'f':
			filePath = optarg;
			break;
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

	if (strcmp(mode, "neuralnetwork") == 0) {
		if (!testMultiNeuralNetwork())
			err(1, "Error during the neural network instance.");
	} else if (strcmp(mode, "chardetection") == 0) {
		if (filePath == NULL)
			filePath = "data/text/big.bmp";

		if (!startImageProcessing(filePath))
			err(1, "Error during the image processing.");
	} else {
		if (filePath == NULL)
			filePath = "data/text/awesome.bmp";

		if (!startNeuralNetworkChar(charPath, charList, filePath))
			err(1, "Error during the real neural network instance");
	}

	printf("MediOCR ended! \n");

	return EXIT_SUCCESS;
}
