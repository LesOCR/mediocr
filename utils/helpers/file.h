#ifndef UTILS_HELPERS_FILE_
#define UTILS_HELPERS_FILE_

unsigned file_exists(char *path);
char *file_get_content(char *path);
void file_put_content(char *path, char *content);

#endif
