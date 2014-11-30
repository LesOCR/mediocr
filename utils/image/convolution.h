
#include <SDL/SDL.h>

#ifndef UTILS_IMAGE_CONVOLUTION_
#define UTILS_IMAGE_CONVOLUTION_

SDL_Surface *convolution_apply(SDL_Surface *s, int *matrix, unsigned matrixSize,
	unsigned divider);
SDL_Color convolution_dowork(SDL_Surface *s, int *matrix, unsigned matrixSize,
	unsigned divider, unsigned x, unsigned y);

// Constants (convolution matrices)
// const int convolution_blur[9] = {
// 	1, 1, 1,
// 	1, 1, 1,
// 	1, 1, 1
// };
// const int convolution_blur_size = 9;
// const int convolution_meanRemoval[9] = {
// 	-1, -1, -1,
// 	-1, 9, -1,
// 	-1, -1, -1
// };
// const int convolution_meanRemoval_size = 9;
// const int convolution_sharpen[9] = {
// 	0, -2, 0,
// 	-2, 11, -2,
// 	0, -2, 0
// };
// const int convolution_sharpen_size = 9;
// const int convolution_emboss[9] = {
// 	2, 0, 0,
// 	0, -1, 0,
// 	0, 0, -1
// };
// const int convolution_emboss_size = 9;
// const int convolution_embossSubtle[9] = {
// 	1, 1, -1,
// 	1, 3, -1,
// 	1, -1, -1
// };
// const int convolution_embossSubtle_size = 9;
// const int convolution_edgeDetect[9] = {
// 	1, 1, 1,
// 	1, -7, 1,
// 	1, 1, 1
// };
// const int convolution_edgeDetect_size = 9;
// const int convolution_edgeDetect2[9] = {
// 	-5, 0, 0,
// 	0, 0, 0,
// 	0, 0, 5
// };
// const int convolution_edgeDetect2_size = 9;

#endif
