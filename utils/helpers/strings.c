
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "strings.h"

char *string_concat(char *s1, char *s2)
{
	char *result = malloc(strlen(s1) + strlen(s2) + 1); //+1 for the zero-terminator
	//in real code you would check for errors in malloc here
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}

char *string_concatChar(char *s1, char s2)
{
	char *result = malloc(strlen(s1)+2);//+1 for the zero-terminator
	//in real code you would check for errors in malloc here
	strcpy(result, s1);
	char *s22 = (char[2]) { (char) s2, '\0' };
	strcat(result, s22);
	return result;
}
