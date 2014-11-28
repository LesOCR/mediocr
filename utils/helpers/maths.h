#include <SDL/SDL.h>
#include "../types/arrays.h"

#ifndef UTILS_HELPERS_MATHS_
#define UTILS_HELPERS_MATHS_

double maths_rand(double a, double b);
double maths_sigmoid(double x);
double maths_dsigmoid(double y);

int maths_between(int in, Uint16 l, Uint16 r);

#endif
