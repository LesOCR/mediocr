#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "maths.h"

double maths_rand(double a, double b)
{
	return (b - a) * ((double)rand() / RAND_MAX) + a;
}

double maths_sigmoid(double x)
{
	return 1.0 / (1.0 + exp(-x));
}

double maths_dsigmoid(double y)
{
	return maths_sigmoid(y) * (1.0 - maths_sigmoid(y));
}

int maths_between(int in, Uint16 l, Uint16 r)
{
	if(in > r)
		in = r;
	if(in < l)
		in = l;

	return in;
}
