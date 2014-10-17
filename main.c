
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <time.h>
#include "SDL/SDL.h"

#include "utils/neuralNetwork/neuralNetwork.h"
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
	struct NeuralNetwork myNeuralNetwork = neuralNetwork_main(2, 2, 1);

	unsignedArray2D input  = new_unsignedArray2D(4, 2);
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

	NeuralNetwork_train(myNeuralNetwork, input, output, 1000, 0.5, 0);

	NeuralNetwork_test(myNeuralNetwork, input);

	printf("Serialized input weights: \n%s\n",
		NeuralNetwork_serializeWeightsInput(myNeuralNetwork));
	printf("Serialized output weights: \n%s\n",
		NeuralNetwork_serializeWeightsOutput(myNeuralNetwork));
	printf("Unserializing:\n");
	NeuralNetwork_loadWeightInput(myNeuralNetwork,
		NeuralNetwork_serializeWeightsInput(myNeuralNetwork));

	return 1;
}

int startImageProcessing()
{
	SDL_Surface *surface = image_load("data/letters/alphabet.bmp");

	//image_renderConsole(surface);

	ImageLineArray imageLine = charDetection_go(surface);

	for(unsigned x = 0; x < imageLine.sizeX; x++)
	{
		for(unsigned y = 0; y < imageLine.elements[x].chars.sizeX; y++)
		{
			image_renderConsoleFromChar(
				image_scale(surface, 16, 16),
				imageLine.elements[x].chars.elements[y]);
		}
	}

	return 1;
}

int main(int argc, char *argv[])
{
	printf("MediOCR started! \n");

	if(!setup())
	{
		err(1, "Error during the initial setup.");
	}

	if(!startNeuralNetwork())
	{
		err(1, "Error during the neural network instance.");
	}

	// if(!startImageProcessing())
	// {
	// 	err(1, "Error during the image processing.");
	// }

	printf("MediOCR ended! \n");


	return EXIT_SUCCESS;
}
