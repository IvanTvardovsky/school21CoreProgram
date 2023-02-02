#include <ctype.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr, "grep: Not enough arguments\n");
    return 0;
  }

  const char* short_options = "e:f:ivclnhso";

  const struct option long_options[] = {{NULL, 0, NULL, 0}};

  int realFlagCounter = 0;

  for (int i = 0; i < argc; ++i) {
    char* temporary;
    temporary = (char*)malloc(256 * sizeof(char));
    strcpy(temporary, argv[i]);
    if (temporary[0] == '-') {
      ++realFlagCounter;
    }
    free(temporary);
  }

  int rez;
  int option_index = 1;
  int counter = 0;
  int fCounter = 0;
  int eCounter = 0;
  int maxIndex = 1;

  char* flags;
  flags = (char*)calloc(256, sizeof(char));
  char eFlagArguments[256][256] = {0};  // [kolvo strok][kolvo simvolov]
  char fFlagArguments[256][256] = {0};  // [kolvo strok][kolvo simvolov]

  while ((rez = getopt_long(argc, argv, short_options, long_options,
                            &option_index)) != -1) {
    switch (rez) {
      case 'e': {
        flags[counter] = 'e';
        strcpy(eFlagArguments[eCounter], optarg);
        ++counter;
        ++eCounter;
        ++maxIndex;
        break;
      };

      case 'f': {
        flags[counter] = 'f';
        strcpy(fFlagArguments[fCounter], optarg);
        ++counter;
        ++fCounter;
        ++maxIndex;
        break;
      };

      case 'i': {
        if (strchr(flags, 'i') == NULL) {
          flags[counter] = 'i';
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

      case 'c': {
        if (strchr(flags, 'c') == NULL) {
          flags[counter] = 'c';
          ++counter;
        }
        break;
      };

      case 'l': {
        if (strchr(flags, 'l') == NULL) {
          flags[counter] = 'l';
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

      case 'h': {
        if (strchr(flags, 'h') == NULL) {
          flags[counter] = 'h';
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

      case 'o': {
        if (strchr(flags, 'o') == NULL) {
          flags[counter] = 'o';
          ++counter;
        }
        break;
      };
    };
  };

  maxIndex += realFlagCounter;

  for (int i = 1; i < argc; ++i) {
    char* temp;
    temp = (char*)malloc(256 * sizeof(char));
    strcpy(temp, argv[i]);
    if (temp[0] == '-') {
      int flag = -1;
      for (long unsigned int j = 1; j < strlen(temp); ++j) {
        if (temp[j] == 'e' || temp[j] == 'f') {
          flag = j;
        }
      }
      if (flag != -1) {
        if (temp[flag + 1]) {
          maxIndex -= 1;
        }
      }
      for (long unsigned int j = 2; j < strlen(temp); ++j) {
        if (temp[j - 1] != 'e' && temp[j - 1] != 'f' && temp[j]) {
          counter -= 1;
        } else {
          break;
        }
      }
    }
    free(temp);
  }

  if (eCounter == 0 && fCounter == 0) {
    strcpy(eFlagArguments[0], argv[counter + 1]);
    ++eCounter;
    ++maxIndex;
  }

  char filesArray[256][256] = {0};  // [kolvo strok][kolvo simvolov]
  int filesArrayCounter = 0;
  char* buffer;

  for (int i = maxIndex; i < argc; ++i) {
    buffer = (char*)calloc(256, sizeof(char));
    strcpy(buffer, argv[i]);
    strcpy(filesArray[filesArrayCounter], buffer);
    ++filesArrayCounter;
    free(buffer);
  }

  char* temporary;
  temporary = (char*)malloc(512 * sizeof(char));
  strcpy(temporary, eFlagArguments[0]);
  if (temporary[0] == '/' && temporary[1] == ' ') {
    fprintf(stderr, "grep: trailing backslash (\\)\n");
    return 0;
  }
  free(temporary);

  regex_t reegex;

  // for flags
  int cFlagArray[4096] = {0};

  for (int i = 0; i < filesArrayCounter; ++i) {
    FILE* file;
    int flag = 1;
    int stringCounter = 1;

    if ((file = fopen(filesArray[i], "r")) == NULL) {
      if (strchr(flags, 's') == NULL)
        fprintf(stderr, "grep: %s: No such file or directory\n", filesArray[i]);
      flag = 0;
    }

    if (flag) {
      char* string;
      char* buffer1;
      string = (char*)malloc(4096 * sizeof(char));

      while (fgets(string, 4096 * sizeof(char), file) != NULL) {
        // for -i
        char* originalString;
        originalString = (char*)malloc(4096 * sizeof(char));
        strcpy(originalString, string);
        if (strchr(flags, 'i')) {
          toUpperCase(string);
        }

        // for -v and in general
        int wasWritten = 0;
        int marked = 0;

        // for -e
        for (int iteratorE = 0; iteratorE < eCounter; ++iteratorE) {
          buffer1 = (char*)malloc(4096 * sizeof(char));
          strcpy(buffer1, eFlagArguments[iteratorE]);

          if (strchr(flags, 'i')) {
            toUpperCase(buffer1);
          }

          // обычная строка без регулярок
          if (KMP(string, buffer1)) {
            if (!wasWritten) {
              // случай без флагов
              if (!strchr(flags, 'v') && !strchr(flags, 'c') &&
                  !strchr(flags, 'l') && !strchr(flags, 'o')) {
                if (marked == 0) {
                  if (!strchr(flags, 'h') && filesArrayCounter > 1) {
                    printf("%s:", filesArray[i]);
                  }
                  if (strchr(flags, 'n')) {
                    printf("%d:", stringCounter);
                  }
                }
                marked = 1;
                printf("%s", originalString);
                if (strchr(originalString, '\n') == NULL) printf("\n");
              }
              wasWritten = 1;
              if (!strchr(flags, 'v')) cFlagArray[i] += 1;
            }

            // только для флага -о
            if (!strchr(flags, 'v') && !strchr(flags, 'c') &&
                !strchr(flags, 'l') && strchr(flags, 'o')) {
              if (marked == 0) {
                if (!strchr(flags, 'h') && filesArrayCounter > 1) {
                  printf("%s:", filesArray[i]);
                }
                if (strchr(flags, 'n')) {
                  printf("%d:", stringCounter);
                }
              }
              marked = 1;
              printf("%s\n", buffer1);
              for (int z = 1; z < stringIncludesString(string, buffer1); ++z) {
                printf("%s\n", buffer1);
              }
            }
          }

          int value;
          value = regcomp(&reegex, buffer1, REG_NEWLINE);
          value = regexec(&reegex, string, 0, NULL, 0);

          // регулярки в -е
          if (value == 0) {
            if (!wasWritten) {
              if (!strchr(flags, 'v') && !strchr(flags, 'c') &&
                  !strchr(flags, 'l') && !strchr(flags, 'o')) {
                if (marked == 0) {
                  if (!strchr(flags, 'h') && filesArrayCounter > 1) {
                    printf("%s:", filesArray[i]);
                  }
                  if (strchr(flags, 'n')) {
                    printf("%d:", stringCounter);
                  }
                }
                marked = 1;
                printf("%s", originalString);
                if (strchr(originalString, '\n') == NULL) printf("\n");

                if (!strchr(flags, 'v') && !strchr(flags, 'c') &&
                    !strchr(flags, 'l') && strchr(flags, 'o')) {
                  printf("%s\n", buffer1);
                }
              }
              wasWritten = 1;
              if (!strchr(flags, 'v')) cFlagArray[i] += 1;

              // только для флага -о
              if (!strchr(flags, 'v') && !strchr(flags, 'c') &&
                  !strchr(flags, 'l') && strchr(flags, 'o')) {
                if (marked == 0) {
                  if (!strchr(flags, 'h') && filesArrayCounter > 1) {
                    printf("%s:", filesArray[i]);
                  }
                  if (strchr(flags, 'n')) {
                    printf("%d:", stringCounter);
                  }
                }
                marked = 1;

                char** regularExpressions1 =
                    (char**)malloc(256 * sizeof(char*));
                for (int l = 0; l < 256; ++l) {
                  regularExpressions1[l] = (char*)malloc(512 * sizeof(int));
                }

                int regCounter1 = 1;

                executeFullRegex(reegex, string, regularExpressions1,
                                 &regCounter1);

                for (int l = 1; l < regCounter1; ++l) {  // цикл по строкам
                  printf("%s", regularExpressions1[l]);
                  if (strchr(regularExpressions1[l], '\n') == NULL)
                    printf("\n");
                }

                for (int l = 0; l < 256; ++l) {
                  free(regularExpressions1[l]);
                }
                free(regularExpressions1);
              }
            }
          }

          regfree(&reegex);
          free(buffer1);
        }

        // for -f
        for (int iteratorF = 0; iteratorF < fCounter; ++iteratorF) {
          FILE* fFile;
          int flagInF = 1;

          if ((fFile = fopen(fFlagArguments[iteratorF], "r")) == NULL) {
            if (strchr(flags, 's') == NULL)
              fprintf(stderr, "%s", "Could not open the file for -f\n");
            flagInF = 0;
          }

          if (flagInF) {
            char* fString;
            fString = (char*)malloc(4096 * sizeof(char));

            while (fgets(fString, 4096, fFile) != NULL) {
              if (strchr(flags, 'i')) {
                toUpperCase(fString);
              }

              char* temp;
              temp = (char*)malloc(4096 * sizeof(char));
              strcpy(temp, fString);

              int value;
              value = regcomp(&reegex, temp, REG_NEWLINE);
              value = regexec(&reegex, string, 0, NULL, 0);

              if (value == 0) {
                if (!wasWritten) {
                  if (!strchr(flags, 'v') && !strchr(flags, 'c') &&
                      !strchr(flags, 'l')) {
                    if (marked == 0) {
                      if (!strchr(flags, 'h') && filesArrayCounter > 1) {
                        printf("%s:", filesArray[i]);
                      }
                      if (strchr(flags, 'n')) {
                        printf("%d:", stringCounter);
                      }
                    }
                    marked = 1;
                    printf("%s", originalString);
                    if (strchr(originalString, '\n') == NULL) printf("\n");
                  }

                  wasWritten = 1;
                  if (!strchr(flags, 'v')) cFlagArray[i] += 1;

                  if (!strchr(flags, 'v') && !strchr(flags, 'c') &&
                      !strchr(flags, 'l') && strchr(flags, 'o')) {
                    char** regularExpressions2 =
                        (char**)malloc(256 * sizeof(char*));
                    for (int l = 0; l < 256; ++l) {
                      regularExpressions2[l] = (char*)malloc(512 * sizeof(int));
                    }

                    int regCounter2 = 1;

                    executeFullRegex(reegex, string, regularExpressions2,
                                     &regCounter2);

                    for (int l = 1; l < regCounter2; ++l) {  // цикл по строкам
                      printf("%s", regularExpressions2[l]);
                      if (strchr(regularExpressions2[l], '\n') == NULL)
                        printf("\n");
                    }

                    for (int l = 0; l < 256; ++l) {
                      free(regularExpressions2[l]);
                    }
                    free(regularExpressions2);
                  }
                }
              }
              free(fString);
              free(temp);
              fString = (char*)malloc(1024 * sizeof(char));
              regfree(&reegex);
            }

            free(fString);
            fclose(fFile);
          }
        }

        if (strchr(flags, 'v') && !wasWritten) {
          cFlagArray[i] += 1;
          if (!strchr(flags, 'c') && !strchr(flags, 'l') &&
              !strchr(flags, 'o')) {
            if (!strchr(flags, 'h') && filesArrayCounter > 1) {
              printf("%s:", filesArray[i]);
            }
            if (strchr(flags, 'n')) {
              printf("%d:", stringCounter);
            }
            printf("%s", originalString);
            if (strchr(originalString, '\n') == NULL) printf("\n");
          } else if (strchr(flags, 'o')) {
            if (!strchr(flags, 'h') && filesArrayCounter > 1) {
              printf("%s:", filesArray[i]);
            }
            if (strchr(flags, 'n')) {
              printf("%d:", stringCounter);
            }
            printf("%s", originalString);
            if (strchr(originalString, '\n') == NULL) printf("\n");
          }
        }
        ++stringCounter;
        free(originalString);
      }

      if (strchr(flags, 'c')) {
        if (strchr(flags, 'l')) {
          if (!strchr(flags, 'h') && filesArrayCounter > 1) {
            printf("%s:", filesArray[i]);
          }
          if (cFlagArray[i] > 0) {
            printf("1\n%s\n", filesArray[i]);
          } else {
            printf("0\n");
          }
        } else {
          if (!strchr(flags, 'h') && filesArrayCounter > 1) {
            printf("%s:", filesArray[i]);
          }
          printf("%d\n", cFlagArray[i]);
        }

      } else if (strchr(flags, 'l')) {
        if (cFlagArray[i] > 0) printf("%s\n", filesArray[i]);
      }

      free(string);
      fclose(file);
    }
  }
  free(flags);
  return 0;
}
