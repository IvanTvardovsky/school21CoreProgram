#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void SpacePrinter(int number);
void PrintUnprintable(char symbol);
void Printer(char symbol, int vFlag, int eFlag, int tFlag);
void FlagDeleter(char flag, char* flags);