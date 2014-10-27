#include "../types/arrays.h"

#ifndef UTILS_NEURALNETWORK_NEURALNETWORK_
#define UTILS_NEURALNETWORK_NEURALNETWORK_

struct NeuralNetwork *neuralNetwork_main(unsigned ni, unsigned nh, unsigned no);
doubleArray NeuralNetwork_update(struct NeuralNetwork *neuralNetwork,
				 unsignedArray input);
double NeuralNetwork_backPropagate(struct NeuralNetwork *neuralNetwork,
				   unsignedArray output, double learningRate,
				   double momentumFactor);
void NeuralNetwork_train(struct NeuralNetwork *neuralNetwork,
			 unsignedArray2D input, unsignedArray2D output,
			 double threshold, double learningRate,
			 double momentumFactor);
void NeuralNetwork_test(struct NeuralNetwork *neuralNetwork,
			unsignedArray2D input);
doubleArray NeuralNetwork_testDouble(struct NeuralNetwork *neuralNetwork,
				     unsignedArray input);

void NeuralNetwork_loadWeightInput(struct NeuralNetwork *neuralNetwork,
				   char *serialized);
void NeuralNetwork_loadWeightOutput(struct NeuralNetwork *neuralNetwork,
				    char *serialized);
char *NeuralNetwork_serializeWeightsInput(struct NeuralNetwork *neuralNetwork);
char *NeuralNetwork_serializeWeightsOutput(struct NeuralNetwork *neuralNetwork);

struct NeuralNetwork {
	unsigned numberInput;
	unsigned numberHidden;
	unsigned numberOutput;

	doubleArray activationInput;
	doubleArray activationHidden;
	doubleArray activationOutput;

	doubleArray2D weightInput;
	doubleArray2D weightOutput;

	doubleArray2D changeInput;
	doubleArray2D changeOutput;
};

#endif
