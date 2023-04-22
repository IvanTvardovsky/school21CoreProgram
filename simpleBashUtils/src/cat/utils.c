#include "utils.h"

void SpacePrinter(int number) {
  int spaces;
  if (number < 0) {
    fprintf(stderr, "%s", "Number is below zero\n");
  } else if (number < 10) {
    spaces = 5;
  } else if (number < 100) {
    spaces = 4;
  } else if (number < 1000) {
    spaces = 3;
  } else if (number < 10000) {
    spaces = 2;
  } else if (number < 100000) {
    spaces = 1;
  } else {
    spaces = 0;
  }

  for (int i = 0; i < spaces; ++i) {
    printf(" ");
  }
}

void PrintUnprintable(char symbol) {
  if ((int)symbol == 127) {
    printf("^?");
  } else {
    char ch = symbol + 64;
    printf("^%c", ch);
  }
}

void Printer(char symbol, int vFlag, int eFlag, int tFlag) {
  if (vFlag == 1) {
    if (symbol == '\n') {
      if (eFlag == 1) {
        printf("$%c", symbol);
      } else {
        printf("%c", symbol);
      }
    } else if (symbol == '\t') {
      if (tFlag == 1) {
        printf("^I");
      } else {
        printf("%c", symbol);
      }
    } else {
      if (isprint(symbol)) {
        printf("%c", symbol);
      } else {
        PrintUnprintable(symbol);
      }
    }
  } else {
    printf("%c", symbol);
  }
}

void FlagDeleter(char flag, char* flags) {
  for (int i = 0; i < (int)strlen(flags); ++i) {
    if (flags[i] == flag) {
      flags[i] = '!';
    }
  }
}
