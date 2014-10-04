
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <time.h>
#include "utils/neuralNetwork/neuralNetwork.h"
#include "utils/types/arrays.h"


void setup()
{
	srand(time(NULL));
}

int main()
{
	printf("MediOCR started! \n");

	setup();

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

	NeuralNetwork_train(myNeuralNetwork, input, output, 100000, 0.5, 0.1);

	NeuralNetwork_test(myNeuralNetwork, input);

	return 0;
}
