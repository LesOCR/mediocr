#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "maths.h"

double maths_rand(double a, double b)
{
	return (b - a)*((double)rand()/RAND_MAX) + a;
}

double maths_sigmoid(double x)
{
	return tanh(x);
	// return 1/(1 + exp(-x));
}

double maths_dsigmoid(double y)
{
	return 1 - (y*y);
}
