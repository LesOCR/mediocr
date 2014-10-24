#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <time.h>
#include "SDL/SDL.h"

#include "utils/neuralNetwork/neuralNetwork.h"
#include "utils/neuralNetwork/charRecognition.h"
#include "utils/types/arrays.h"
#include "utils/image/image.h"
#include "utils/image/charDetection.h"
#include "utils/types/structArrays.h"

int setup()
{
	srand(time(NULL));

	return 1;
}

int startNeuralNetwork()
{
	struct NeuralNetwork *myNeuralNetwork = neuralNetwork_main(2, 2, 1);

	unsignedArray2D input = new_unsignedArray2D(4, 2);
	unsignedArray2D output = new_unsignedArray2D(4, 1);

	input.elements[0].elements[0] = 0;
	input.elements[0].elements[1] = 0;
	input.elements[1].elements[0] = 0;
	input.elements[1].elements[1] = 1;
	input.elements[2].elements[0] = 1;
	input.elements[2].elements[1] = 0;
	input.elements[3].elements[0] = 1;
	input.elements[3].elements[1] = 1;

	output.elements[0].elements[0] = 0;
	output.elements[1].elements[0] = 1;
	output.elements[2].elements[0] = 1;
	output.elements[3].elements[0] = 0;

	NeuralNetwork_train(myNeuralNetwork, input, output, 0.000001, 0.5, 0);

	NeuralNetwork_test(myNeuralNetwork, input);

	char *serializedInput = NeuralNetwork_serializeWeightsInput(myNeuralNetwork);
	char *serializedOutput = NeuralNetwork_serializeWeightsOutput(myNeuralNetwork);
	printf("Serialized input weights: \n%s\n", serializedInput);
	printf("Serialized output weights: \n%s\n", serializedOutput);
	printf("Unserializing:\n");
	NeuralNetwork_loadWeightInput(myNeuralNetwork, serializedInput);

	return 1;
}

int startNeuralNetworkChar()
{
	SDL_Surface *surface = image_load("data/text/abcd.bmp");
	char characters[4] = {
		'a',
		'b',
		'c',
		'd'
	};

	struct charRecognitionList *charRecog = charRecognition_learn(surface,
		characters, 4);

	ImageLineArray imageLine = charDetection_go(surface);
	printf("Recognized string: \n");
	for(unsigned i = 0; i < imageLine.size; i++)
	{
		for(unsigned j = 0; j < imageLine.elements[i].chars.size; j++)
		{
			SDL_Surface *s = image_scale(image_extractChar(surface,
					&imageLine.elements[i].chars.elements[j]), 16, 16);

			printf("%c", charRecognition_getChar(charRecog, s));
		}
	}
	printf("\n");

	return 1;
}

int startImageProcessing()
{
	SDL_Surface *surface = image_load("data/text/3lines.bmp");
	ImageLineArray imageLine = charDetection_go(surface);

	for (unsigned i = 0; i < imageLine.size; i++)
		for (unsigned j = 0; j < imageLine.elements[i].chars.size; j++) {
			SDL_Surface *s = image_extractChar(
				surface, &imageLine.elements[i].chars.elements[j]);
			image_renderConsole(image_scale(s, 16, 16));
			printf("\n");
		}

	return 1;
}

int main(int argc, char *argv[])
{
	printf("MediOCR started! \n");

	if (!setup())
		err(1, "Error during the initial setup.");

	// if(!startNeuralNetwork())
	// 	err(1, "Error during the neural network instance.");

	// if(!startImageProcessing())
	// 	err(1, "Error during the image processing.");

	if (!startNeuralNetworkChar())
		err(1, "Error during the real neural network instance");

	printf("MediOCR ended! \n");

	return EXIT_SUCCESS;
}
