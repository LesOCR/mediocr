# Simple Makefile
# Practical session 01

# Global Compilation Variables

# The compiler
CC=@gcc

# Pre-processor flags
CPPFLAGS=

# C compiler flags
CFLAGS= -Wall -Werror -std=c99 -O3

# Linker flags
LDFLAGS=

# Linker libs
LDLIBS= -lm `sdl-config --cflags --libs`

# all target just call the production of main
all: main clean

# main target using implicit rules, just express dependencies
main: main.o utils/neuralNetwork/neuralNetwork.o utils/helpers/maths.o utils/helpers/file.o utils/types/arrays.o utils/types/structArrays.o  utils/image/image.o utils/image/charDetection.o

.PHONY: clean

# clean compilation products
clean:
	@echo "Cleaning up .o files"
	@find . -name "*.o" -type f -delete

# END of File
