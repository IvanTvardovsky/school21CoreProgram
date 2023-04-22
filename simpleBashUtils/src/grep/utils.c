#include "utils.h"

void toUpperCase(char* s) {
  while (*s) {
    *s = toupper((unsigned char)*s);
    s++;
  }
}

int KMP(char* s, char* p) {
  int i, j, N, M;
  N = strlen(s);
  M = strlen(p);

  int* d = (int*)malloc(M * sizeof(int));

  // prefix
  d[0] = 0;
  for (i = 1, j = 0; i < M; ++i) {
    while (j > 0 && p[j] != p[i]) j = d[j - 1];
    if (p[j] == p[i]) ++j;
    d[i] = j;
  }

  // search
  for (i = 0, j = 0; i < N; ++i) {
    while (j > 0 && p[j] != s[i]) j = d[j - 1];
    if (p[j] == s[i]) ++j;
    if (j == M) {
      free(d);
      return 1;
    }
  }
  free(d);
  return 0;
}

int stringIncludesString(char* str, char* substr) {
  int i = 0;
  while ((str = strstr(str, substr)) != NULL) {
    ++i;
    str += strlen(substr);
  }
  return i;
}

void executeFullRegex(regex_t pattern, char* string, char** matchedSubStrings,
                      int* matchedSubStringsNumber) {
  size_t nmatch = 2;
  regmatch_t pmatch[2];
  int code = 1;
  while ((code = regexec(&pattern, string, nmatch, pmatch, 0)) != 1) {
    strncpy(matchedSubStrings[*matchedSubStringsNumber],
            &string[pmatch[0].rm_so], pmatch[0].rm_eo - pmatch[0].rm_so);
    string = string + pmatch[0].rm_eo;
    (*matchedSubStringsNumber)++;
  }
}
