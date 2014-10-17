
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include "../helpers/maths.h"
#include "../types/arrays.h"
#include "neuralNetwork.h"


struct NeuralNetwork neuralNetwork_main(unsigned ni, unsigned nh, unsigned no)
{
	printf("Neural network started! \n");

	// Initialize our structure
	struct NeuralNetwork myNeuralNetwork;

	// Initialize values of our structure
	myNeuralNetwork.numberInput  = ++ni; // + 1 for bias node
	myNeuralNetwork.numberHidden = nh;
	myNeuralNetwork.numberOutput = no;

	// Activations for nodes
    myNeuralNetwork.activationInput  =
		new_doubleArray(myNeuralNetwork.numberInput);
    myNeuralNetwork.activationHidden =
	 	new_doubleArray(myNeuralNetwork.numberHidden);
    myNeuralNetwork.activationOutput =
		new_doubleArray(myNeuralNetwork.numberOutput);

	// Fill them with their default values
	fill_doubleArray(myNeuralNetwork.activationInput, 1);
	fill_doubleArray(myNeuralNetwork.activationHidden, 1);
	fill_doubleArray(myNeuralNetwork.activationOutput, 1);

	// Create weights
	myNeuralNetwork.weightInput  =
		new_doubleArray2D(myNeuralNetwork.numberInput,
			myNeuralNetwork.numberHidden);
	myNeuralNetwork.weightOutput =
		new_doubleArray2D(myNeuralNetwork.numberHidden,
			myNeuralNetwork.numberOutput);

	// Assign some random values
	for(unsigned i = 0; i < myNeuralNetwork.numberInput; i++)
	{
		for(unsigned j = 0; j < myNeuralNetwork.numberHidden; j++)
		{
			myNeuralNetwork.weightInput.elements[i].elements[j] =
				maths_rand(-0.2, 0.2);
		}
	}

	for(unsigned j = 0; j < myNeuralNetwork.numberHidden; j++)
	{
		for(unsigned k = 0; k < myNeuralNetwork.numberOutput; k++)
		{
			myNeuralNetwork.weightOutput.elements[j].elements[k] =
				maths_rand(-2.0, 2.0);
		}
	}

	myNeuralNetwork.changeInput  =
		new_doubleArray2D(myNeuralNetwork.numberInput,
			myNeuralNetwork.numberHidden);
	myNeuralNetwork.changeOutput =
		new_doubleArray2D(myNeuralNetwork.numberHidden,
			myNeuralNetwork.numberOutput);

	return myNeuralNetwork;
}

doubleArray NeuralNetwork_update(struct NeuralNetwork neuralNetwork,
	unsignedArray input)
{
	if(input.sizeX != neuralNetwork.numberInput - 1)
	{
		err(1, "Wrong number of inputs, %d - expected %d\n",
			input.sizeX, neuralNetwork.numberInput - 1);
	}

	// Input activation
	for(unsigned i = 0; i < neuralNetwork.numberInput - 1; i++)
	{
		neuralNetwork.activationInput.elements[i] = input.elements[i];
	}

	// Hidden activation
	for(unsigned j = 0; j < neuralNetwork.numberHidden; j++)
	{
		double sum = 0;
		for(unsigned i = 0; i < neuralNetwork.numberInput; i++)
		{
			sum += neuralNetwork.activationInput.elements[i] *
				neuralNetwork.weightInput.elements[i].elements[j];
		}
		neuralNetwork.activationHidden.elements[j] = maths_sigmoid(sum);
	}

	// Output activation
	for(unsigned k = 0; k < neuralNetwork.numberOutput; k++)
	{
		double sum = 0;
		for(unsigned j = 0; j < neuralNetwork.numberHidden; j++)
		{
			sum += neuralNetwork.activationHidden.elements[j] *
				neuralNetwork.weightOutput.elements[j].elements[k];
		}
		neuralNetwork.activationOutput.elements[k] = maths_sigmoid(sum);
	}

	return neuralNetwork.activationOutput;
}

double NeuralNetwork_backPropagate(struct NeuralNetwork neuralNetwork,
	unsignedArray targets, double learningRate, double momentumFactor)
{
	if(targets.sizeX != neuralNetwork.numberOutput)
	{
		err(1, "Wrong number of target values, %d - expected %d\n",
			targets.sizeX, neuralNetwork.numberOutput);
	}

	// Calculate error terms for output
	doubleArray outputDeltas = new_doubleArray(neuralNetwork.numberOutput);
	for(unsigned k = 0; k < neuralNetwork.numberOutput; k++)
	{
		double error = targets.elements[k] -
			neuralNetwork.activationOutput.elements[k];
		outputDeltas.elements[k] =
			maths_dsigmoid(neuralNetwork.activationOutput.elements[k]) *
			error;
	}

	// Calculate error terms for hidden
	doubleArray hiddenDeltas = new_doubleArray(neuralNetwork.numberHidden);
	for(unsigned j = 0; j < neuralNetwork.numberHidden; j++)
	{
		double error = 0;
		for(unsigned k = 0; k < neuralNetwork.numberOutput; k++)
		{
			error += outputDeltas.elements[k] *
				neuralNetwork.weightOutput.elements[j].elements[k];
		}
		hiddenDeltas.elements[j] =
			maths_dsigmoid(neuralNetwork.activationHidden.elements[j]) *
			error;
	}

	// Update output weights
	for(unsigned j = 0; j < neuralNetwork.numberHidden; j++)
	{
		for(unsigned k = 0; k < neuralNetwork.numberOutput; k++)
		{
			double change = outputDeltas.elements[k] *
			 	neuralNetwork.activationHidden.elements[j];
			neuralNetwork.weightOutput.elements[j].elements[k] +=
				(learningRate * change) +
				(momentumFactor *
				 	neuralNetwork.changeOutput.elements[j].elements[k]);
			neuralNetwork.changeOutput.elements[j].elements[k] = change;
		}
	}

	// Update input weights
	for(unsigned i = 0; i < neuralNetwork.numberInput; i++)
	{
		for(unsigned j = 0; j < neuralNetwork.numberHidden; j++)
		{
			double change = hiddenDeltas.elements[j] *
			 	neuralNetwork.activationInput.elements[i];
			neuralNetwork.weightInput.elements[i].elements[j] +=
				(learningRate * change) +
				(momentumFactor *
					neuralNetwork.changeInput.elements[i].elements[j]);
			neuralNetwork.changeInput.elements[i].elements[j] = change;
		}
	}

	// Calculate error
	double error = 0;
	for(unsigned k = 0; k < neuralNetwork.numberOutput; k++)
	{
		error += 0.5 *
			pow(targets.elements[k] -
				neuralNetwork.activationOutput.elements[k], 2);
	}

	return error;
}

void NeuralNetwork_train(struct NeuralNetwork neuralNetwork,
	unsignedArray2D input, unsignedArray2D output, unsigned iterations,
	double learningRate, double momentumFactor)
{
	for(unsigned i = 0; i < iterations; i++)
	{
		double error = 0;

		for(unsigned j = 0; j < input.sizeX; j++)
		{
			NeuralNetwork_update(neuralNetwork, input.elements[j]);
			error +=
				NeuralNetwork_backPropagate(
					neuralNetwork, output.elements[j],
					learningRate, momentumFactor);
		}

		if(i % 100 == 0)
		{
			printf("error percentage: %f\n", error * 100);
		}
	}
}

void NeuralNetwork_test(struct NeuralNetwork neuralNetwork,
	unsignedArray2D input)
{
	for(unsigned i = 0; i < input.sizeX; i++)
	{
		print_unsignedArray(input.elements[i]);
		printf(" -> ");
		print_doubleArray(
			NeuralNetwork_update(neuralNetwork, input.elements[i]));
		printf("\n");
	}
}

// char *NeuralNetwork_serializeWeightsInput(struct NeuralNetwork neuralNetwork)
// {
// 	char *seralized = (char*)malloc(30 * sizeof(char));
//
// 	for(unsigned i = 0; i < neuralNetwork.numberInput; i++)
// 	{
// 		for(unsigned j = 0; j < neuralNetwork.numberHidden; j++)
// 		{
// 			char *s;
//
// 		}
// 	}
// }
