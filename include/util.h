#ifndef __UTIL_H_
#define __UTIL_H_

char* rtrim(char* str, char* seps);
char* ltrim(char* str, char* seps);
char* trim(char*  str, char* seps);

char** split_str(char* str, char* delim);

#endif
