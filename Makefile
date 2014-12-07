# Simple Makefile
# Practical session 01

# Global Compilation Variables

# The compiler
CC=gcc

# Pre-processor flags
CPPFLAGS=

# C compiler flags
CFLAGS= -Wall -Werror -std=c99 -O3

# Linker flags
LDFLAGS=

# Linker libs
LDLIBS= -lm `sdl-config --cflags --libs`

SRC= main.c utils/neuralNetwork/neuralNetwork.c utils/neuralNetwork/charRecognition.c utils/neuralNetwork/wordCorrector.c utils/helpers/maths.c utils/helpers/file.c utils/helpers/strings.c utils/types/arrays.c utils/types/structArrays.c  utils/image/image.c utils/image/charDetection.c utils/image/convolution.c utils/image/filters.c
OBJ= ${SRC:.c=.o}

# all target just call the production of main
all: main

# main target using implicit rules, just express dependencies
main: ${OBJ}

.PHONY: clean

# clean compilation products
clean:
	find . -name "*.o" -type f -delete
	find . -name "*~" -type f -delete
	rm -f main

# END of File
