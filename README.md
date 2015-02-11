MediOCR
=======

MediOCR is a project that had to be done during the third semester at EPITA. Its main purpose is to be able to recognize text inside an image, no matter how much noise is present, or what font family/size/color has been used.

To do so, a neural network had to be used. If needed, the neural network can be used in other projects, as it is very flexible.

Installation
------------

### Git

``` sh
git clone https://github.com/mandor53/mediocr.git
make
```

----------------------

Usage
-----

### Overview

As of now, MediOCR is able to recognize a text in an image which has some noise in it. It is also able to correct words using a dictionary. MediOCR is also able to learn multiple fonts and recognize them, as well as some special characters if they've been put into the learning set.

### Help

``` sh
./main -h
```

### Execution

Making MediOCR output the text it recognized is one of the easiest things, all you need to do is specify the file that you want to process.

``` sh
./main -f "path/to/file.bmp"
```

### Use a specific dictionary

Dictionaries are used to automatically correct any mistake made by the neural network when trying to recognize characters. However, the words may vary depending on the language being used, which is why you might want to specify your own dictionary.

``` sh
./main -f "path/to/file.bmp" -d "path/to/dictionary.txt"
```

### Make MedioCR learn (populate the learning set and store weights)

The alphabet can be anything, and contain any character.
The path/to/learning/set directory must be formated the same way as in data/letters (which is the default learning set directory). The name of the directories inside is the ascii value of the characters. The images should be squares.

``` sh
./main -m "learn" -c "path/to/learning/set" -s "abcdefghijklmnopqrstuvwxyz"
```

----------------------

Remarks
-------

### Code

Please keep in mind that the code might be messy. This project was an attempt at learning the C programming language, and therefore a lot of coding aspects were still unknown or blurry.

### Optimization

The code is also NOT optimized. The memory isn't freed when it should, and some algorithms used in the project (such as the backpropagating neural network) should be cleaned up.
