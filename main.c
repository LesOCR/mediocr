
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <time.h>
#include "SDL/SDL.h"

#include "utils/neuralNetwork/neuralNetwork.h"
#include "utils/types/arrays.h"
#include "utils/image/image.h"
#include "utils/image/charDetection.h"


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

	return 1;
}

int startImageProcessing()
{
	SDL_Surface *surface = image_load("data/text/abcd.bmp");
	struct ImageLine imageLine;
	charDetection_line(surface, &imageLine, 0);
	image_renderConsoleFromLine(surface, imageLine);

	return 1;
}

int main()
{
	printf("MediOCR started! \n");

	if(!setup())
	{
		err(1, "Error during the initial setup.");
	}

	if(!startImageProcessing())
	{
		err(1, "Error during the image processing.");
	}

	printf("MediOCR ended! \n");


	return EXIT_SUCCESS;
}
