#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int main(int argc, char* argv[]) {
  const char* short_options = "bevnstzx";

  char* flags;
  flags = (char*)calloc(128, sizeof(char));
  char* flagsToDelete;
  flagsToDelete = (char*)calloc(128, sizeof(char));

  int counter = 0;

  char filesArray[64][128] = {0};  // [kolvo strok][kolvo simvolov]
  int filesArrayCounter = 0;
  char* buffer;

  for (int i = 1; i < argc; ++i) {
    buffer = (char*)calloc(128, sizeof(char));
    strcpy(buffer, argv[i]);

    if (buffer[0] != '-') {
      strcpy(filesArray[filesArrayCounter], buffer);
      ++filesArrayCounter;
    } else if (filesArrayCounter > 0) {
      int len = strlen(flagsToDelete);
      flagsToDelete[strlen(flagsToDelete)] = buffer[1];
      flagsToDelete[len + 1] = '\0';
    }
    free(buffer);
  }

  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {"E", no_argument, NULL, 'z'},
      {"T", no_argument, NULL, 'x'},
      {NULL, 0, NULL, 0}};

  int rez;
  int option_index = 1;

  while ((rez = getopt_long(argc, argv, short_options, long_options,
                            &option_index)) != -1) {
    switch (rez) {
      case 'b': {
        if (strchr(flags, 'b') == NULL) {
          flags[counter] = 'b';
          ++counter;
        }
        break;
      };

      case 'e': {
        if (strchr(flags, 'e') == NULL) {
          flags[counter] = 'e';
          ++counter;
        }
        break;
      };

      case 'n': {
        if (strchr(flags, 'n') == NULL) {
          flags[counter] = 'n';
          ++counter;
        }
        break;
      };

      case 's': {
        if (strchr(flags, 's') == NULL) {
          flags[counter] = 's';
          ++counter;
        }
        break;
      };

      case 't': {
        if (strchr(flags, 't') == NULL) {
          flags[counter] = 't';
          ++counter;
        }
        break;
      };

      case 'v': {
        if (strchr(flags, 'v') == NULL) {
          flags[counter] = 'v';
          ++counter;
        }
        break;
      };

      case 'z': {
        if (strchr(flags, 'z') == NULL) {
          flags[counter] = 'z';
          ++counter;
        }
        break;
      };

      case 'x': {
        if (strchr(flags, 'x') == NULL) {
          flags[counter] = 'x';
          ++counter;
        }
        break;
      };
    };
  };

  for (int i = 0; i < (int)strlen(flagsToDelete); ++i) {
    FlagDeleter(flagsToDelete[i], flags);
  }

  int isVFlag = 0;
  int isEFlag = 0;
  int isTFlag = 0;

  if (strchr(flags, 'z') && strchr(flags, 'v')) {
    isEFlag = 1;
  }

  if (strchr(flags, 'x') && strchr(flags, 'v')) {
    isTFlag = 1;
  }

  if (strchr(flags, 'e')) {
    isEFlag = 1;
  }

  if (strchr(flags, 't')) {
    isTFlag = 1;
  }

  if ((strchr(flags, 'v')) || isEFlag || isTFlag) {
    isVFlag = 1;
  }

  for (int i = 0; i < filesArrayCounter; ++i) {
    FILE* file;
    if ((file = fopen(filesArray[i], "r")) == NULL) {
      fprintf(stderr, "%s", "Could not open the file\n");
      return 0;
    }

    char symbol;
    char prevSymbol = '\n';
    int spacesInRow = 1;
    counter = 1;

    while ((symbol = fgetc(file)) != EOF) {
      if (symbol == '\n') {
        ++spacesInRow;
      } else {
        spacesInRow = 0;
      }

      if (spacesInRow > 2) {
        if (strchr(flags, 's') == NULL) {
          if ((strchr(flags, 'n') != NULL) && (strchr(flags, 'b') == NULL)) {
            SpacePrinter(counter);
            printf("%d	", counter);
            ++counter;
          }
          Printer(symbol, isVFlag, isEFlag, isTFlag);
        }

      } else {
        if ((strchr(flags, 'b') != NULL) && (symbol != '\n') &&
            (prevSymbol == '\n')) {
          SpacePrinter(counter);
          printf("%d	", counter);
          ++counter;
        } else if ((strchr(flags, 'n') != NULL) && (prevSymbol == '\n') &&
                   (strchr(flags, 'b') == NULL)) {
          SpacePrinter(counter);
          printf("%d	", counter);
          ++counter;
        }
        Printer(symbol, isVFlag, isEFlag, isTFlag);
      }
      prevSymbol = symbol;
    }
  }
  free(flags);
  free(flagsToDelete);
  return 0;
}
