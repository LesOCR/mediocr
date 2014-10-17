
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

	/*image_renderConsole(surface);*/

	ImageLineArray imageLine = charDetection_go(surface);

	SDL_Surface *newSurface = SDL_CreateRGBSurface(surface->flags, 16, 16,
		surface->format->BitsPerPixel, surface->format->Rmask,
		surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
	Sint16 rektWidth = imageLine.elements[0].chars.elements[2].endX -
		imageLine.elements[0].chars.elements[2].startX;
	Sint16 rektHeight = imageLine.elements[0].chars.elements[2].endY -
		imageLine.elements[0].chars.elements[2].startY;
	printf("%d\n", rektHeight);
	SDL_Rect rekt = {
		.x = (Sint16)imageLine.elements[0].chars.elements[2].startX,
		.y = (Sint16)imageLine.elements[0].chars.elements[2].startY,
		.w = rektWidth,
		.h = rektHeight
	};
	SDL_Rect newRekt = {
		.x = 5,
		.y = 5
	};

	int r = SDL_BlitSurface(surface, &rekt, newSurface, &newRekt);
	printf("%d\n", r);
	image_renderConsole(newSurface);
	/*image_renderConsoleFromChar(*/
			/*image_scale(surface, 16, 16)*/
			/*, imageLine.elements[x].chars.elements[y]);*/
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
