#include <unistd.h>

unsigned file_exists(char *path)
{
	return access(path, F_OK) != -1;
}
