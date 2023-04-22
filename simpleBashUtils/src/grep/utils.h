#include <ctype.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void toUpperCase(char* s);
int KMP(char* s, char* p);
int stringIncludesString(char* str, char* substr);
void executeFullRegex(regex_t pattern, char* string, char** matchedSubStrings,
                      int* matchedSubStringsNumber);