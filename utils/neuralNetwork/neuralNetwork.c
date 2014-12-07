#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <math.h>

#include "../helpers/strings.h"
#include "../helpers/maths.h"
#include "../types/arrays.h"
#include "neuralNetwork.h"

struct NeuralNetwork *neuralNetwork_main(unsigned ni, unsigned nh, unsigned no)
{
	// Initialize our structure
	struct NeuralNetwork *myNeuralNetwork =
	    malloc(sizeof(struct NeuralNetwork));

	// Initialize values of our structure
	myNeuralNetwork->numberInput = ++ni; // + 1 for bias node
	myNeuralNetwork->numberHidden = nh;
	myNeuralNetwork->numberOutput = no;

	// Activations for nodes
	myNeuralNetwork->activationInput =
	    new_doubleArray(myNeuralNetwork->numberInput);
	myNeuralNetwork->activationHidden =
	    new_doubleArray(myNeuralNetwork->numberHidden);
	myNeuralNetwork->activationOutput =
	    new_doubleArray(myNeuralNetwork->numberOutput);

	// Fill them with their default values
	fill_doubleArray(myNeuralNetwork->activationInput, 1);
	fill_doubleArray(myNeuralNetwork->activationHidden, 1);
	fill_doubleArray(myNeuralNetwork->activationOutput, 1);

	// Create weights
	myNeuralNetwork->weightInput = new_doubleArray2D(
	    myNeuralNetwork->numberInput, myNeuralNetwork->numberHidden);
	myNeuralNetwork->weightOutput = new_doubleArray2D(
	    myNeuralNetwork->numberHidden, myNeuralNetwork->numberOutput);

	// Assign some random values
	for (unsigned i = 0; i < myNeuralNetwork->numberInput; i++)
		for (unsigned j = 0; j < myNeuralNetwork->numberHidden; j++)
			myNeuralNetwork->weightInput.elements[i].elements[j] =
			    maths_rand(-0.2, 0.2);

	for (unsigned j = 0; j < myNeuralNetwork->numberHidden; j++)
		for (unsigned k = 0; k < myNeuralNetwork->numberOutput; k++)
			myNeuralNetwork->weightOutput.elements[j].elements[k] =
			    maths_rand(-2.0, 2.0);

	myNeuralNetwork->changeInput = new_doubleArray2D(
	    myNeuralNetwork->numberInput, myNeuralNetwork->numberHidden);
	myNeuralNetwork->changeOutput = new_doubleArray2D(
	    myNeuralNetwork->numberHidden, myNeuralNetwork->numberOutput);

	return myNeuralNetwork;
}

doubleArray NeuralNetwork_update(struct NeuralNetwork *neuralNetwork,
				 unsignedArray input)
{
	if (input.sizeX != neuralNetwork->numberInput - 1)
		err(1, "Wrong number of inputs, %d - expected %d\n",
		    input.sizeX, neuralNetwork->numberInput - 1);

	// Input activation
	for (unsigned i = 0; i < neuralNetwork->numberInput - 1; i++)
		neuralNetwork->activationInput.elements[i] = input.elements[i];

	// Hidden activation
	for (unsigned j = 0; j < neuralNetwork->numberHidden; j++) {
		double sum = 0;
		for (unsigned i = 0; i < neuralNetwork->numberInput; i++)
			sum +=
			    neuralNetwork->activationInput.elements[i] *
			    neuralNetwork->weightInput.elements[i].elements[j];

		neuralNetwork->activationHidden.elements[j] =
		    maths_sigmoid(sum);
	}

	// Output activation
	for (unsigned k = 0; k < neuralNetwork->numberOutput; k++) {
		double sum = 0;
		for (unsigned j = 0; j < neuralNetwork->numberHidden; j++)
			sum +=
			    neuralNetwork->activationHidden.elements[j] *
			    neuralNetwork->weightOutput.elements[j].elements[k];

		neuralNetwork->activationOutput.elements[k] =
		    maths_sigmoid(sum);
	}

	return neuralNetwork->activationOutput;
}

double NeuralNetwork_backPropagate(struct NeuralNetwork *neuralNetwork,
				   unsignedArray targets, double learningRate,
				   double momentumFactor)
{
	if (targets.sizeX != neuralNetwork->numberOutput)
		err(1, "Wrong number of target values, %d - expected %d\n",
		    targets.sizeX, neuralNetwork->numberOutput);

	// Calculate error terms for output
	doubleArray outputDeltas = new_doubleArray(neuralNetwork->numberOutput);
	for (unsigned k = 0; k < neuralNetwork->numberOutput; k++) {
		double error = targets.elements[k] -
			       neuralNetwork->activationOutput.elements[k];

		outputDeltas.elements[k] =
		    maths_dsigmoid(
			neuralNetwork->activationOutput.elements[k]) *
		    error;
	}

	// Calculate error terms for hidden
	doubleArray hiddenDeltas = new_doubleArray(neuralNetwork->numberHidden);
	for (unsigned j = 0; j < neuralNetwork->numberHidden; j++) {
		double error = 0;
		for (unsigned k = 0; k < neuralNetwork->numberOutput; k++)
			error +=
			    outputDeltas.elements[k] *
			    neuralNetwork->weightOutput.elements[j].elements[k];

		hiddenDeltas.elements[j] =
		    maths_dsigmoid(
			neuralNetwork->activationHidden.elements[j]) *
		    error;
	}

	// Update output weights
	for (unsigned j = 0; j < neuralNetwork->numberHidden; j++)
		for (unsigned k = 0; k < neuralNetwork->numberOutput; k++) {
			double change =
			    outputDeltas.elements[k] *
			    neuralNetwork->activationHidden.elements[j];
			neuralNetwork->weightOutput.elements[j].elements[k] +=
			    (learningRate * change) +
			    (momentumFactor *
			     neuralNetwork->changeOutput.elements[j]
				 .elements[k]);
			neuralNetwork->changeOutput.elements[j].elements[k] =
			    change;
		}

	// Update input weights
	for (unsigned i = 0; i < neuralNetwork->numberInput; i++)
		for (unsigned j = 0; j < neuralNetwork->numberHidden; j++) {
			double change =
			    hiddenDeltas.elements[j] *
			    neuralNetwork->activationInput.elements[i];
			neuralNetwork->weightInput.elements[i].elements[j] +=
			    (learningRate * change) +
			    (momentumFactor *
			     neuralNetwork->changeInput.elements[i]
				 .elements[j]);
			neuralNetwork->changeInput.elements[i].elements[j] =
			    change;
		}

	// Calculate error
	double error = 0;
	for (unsigned k = 0; k < neuralNetwork->numberOutput; k++)
		error +=
		    (1.0 / neuralNetwork->numberInput) * pow(targets.elements[k] -
				  neuralNetwork->activationOutput.elements[k],
			      2);

	return error;
}

void NeuralNetwork_train(struct NeuralNetwork *neuralNetwork,
			 unsignedArray2D input, unsignedArray2D output,
			 double threshold, double learningRate,
			 double momentumFactor)
{
	double error = 0;
	unsigned long step = 0;
	do {
		error = 0;

		for (unsigned j = 0; j < input.sizeX; j++) {
			NeuralNetwork_update(neuralNetwork, input.elements[j]);
			error += NeuralNetwork_backPropagate(
			    neuralNetwork, output.elements[j], learningRate,
			    momentumFactor);
		}

		if(!(++step % 100))
		{
			printf("Error ratio: %g\n", error);
			step = 0;
		}

	} while (fabs(error) > threshold);

	printf("Final error ratio: %g\n", error);
}

void NeuralNetwork_test(struct NeuralNetwork *neuralNetwork,
			unsignedArray2D input)
{
	for (unsigned i = 0; i < input.sizeX; i++) {
		print_unsignedArray(input.elements[i]);
		printf(" -> ");
		print_doubleArray(
		    NeuralNetwork_update(neuralNetwork, input.elements[i]));
		printf("\n");
	}
}

doubleArray NeuralNetwork_testDouble(struct NeuralNetwork *neuralNetwork,
				     unsignedArray input)
{
	return NeuralNetwork_update(neuralNetwork, input);
}

void NeuralNetwork_loadWeightInput(struct NeuralNetwork *neuralNetwork,
				   char *serialized)
{
	unsigned pos = 0;
	for (unsigned i = 0; i < neuralNetwork->numberInput; i++)
		for (unsigned j = 0; j < neuralNetwork->numberHidden; j++) {
			unsigned start = pos;
			while (serialized[pos] != ';')
				pos++;
			pos++;
			char *coef = malloc(10 * sizeof(char));
			strncpy(coef, &serialized[start], pos - start - 1);

			// We actually don't need to really check for anything
			// when adding the value to our weights, as we
			// assume the user is importing correctly serialized
			// weights.
			neuralNetwork->weightInput.elements[i].elements[j] =
			    atof(coef);
		}
}
void NeuralNetwork_loadWeightOutput(struct NeuralNetwork *neuralNetwork,
				    char *serialized)
{
	unsigned pos = 0;
	for (unsigned j = 0; j < neuralNetwork->numberHidden; j++)
		for (unsigned k = 0; k < neuralNetwork->numberOutput; k++) {
			unsigned start = pos;
			while (serialized[pos] != ';')
				pos++;
			pos++;
			char *coef = malloc(10 * sizeof(char));
			strncpy(coef, &serialized[start], pos - start - 1);

			// We actually don't need to really check for anything
			// when adding the value to our weights, as we
			// assume the user is importing correctly serialized
			// weights.
			neuralNetwork->weightOutput.elements[j].elements[k] =
			    atof(coef);
		}
}

char *NeuralNetwork_serializeWeightsInput(struct NeuralNetwork *neuralNetwork)
{
	unsigned stringLength = 0;
	char *serialized = malloc((stringLength + 1) * sizeof(char));

	for (unsigned i = 0; i < neuralNetwork->numberInput; i++)
		for (unsigned j = 0; j < neuralNetwork->numberHidden; j++) {
			char s[20];
			sprintf(
			    s, "%.6f",
			    neuralNetwork->weightInput.elements[i].elements[j]);
			stringLength += 20;
			serialized = realloc(serialized, (stringLength + 1) * sizeof(char));
			strcat(serialized, s);
			strcat(serialized, ";");
		}

	return serialized;
}
char *NeuralNetwork_serializeWeightsOutput(struct NeuralNetwork *neuralNetwork)
{
	unsigned stringLength = 0;
	char *serialized = malloc((stringLength + 1) * sizeof(char));

	for (unsigned j = 0; j < neuralNetwork->numberHidden; j++)
		for (unsigned k = 0; k < neuralNetwork->numberOutput; k++) {
			char s[20];
			sprintf(s, "%.6f",
				neuralNetwork->weightOutput.elements[j]
				    .elements[k]);
			stringLength += 20;
			serialized = realloc(serialized, (stringLength + 1) * sizeof(char));
			strcat(serialized, s);
			strcat(serialized, ";");
		}

	return serialized;
}
