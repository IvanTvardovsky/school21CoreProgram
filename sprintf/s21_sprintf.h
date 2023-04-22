#ifndef S21_SPRINTF_H
#define S21_SPRINTF_H

#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_string.h"

typedef struct FormatSpecificator {
  bool minus;
  bool plus;
  bool space;
  bool sharp;
  bool zero;
  s21_size_t width;
  int precision;
  bool isPrecisionSet;
  char length;
  char specificator;
} Specificator;

const char *GetFlags(Specificator *specificator, const char *format);
const char *GetWidth(Specificator *specificator, const char *format,
                     va_list vl);
const char *GetPrecision(Specificator *specificator, const char *format,
                         va_list vl);
const char *GetLength(Specificator *specificator, const char *format);
const char *GetSpecificator(Specificator *specificator, const char *format);
void ProcessSpecifiers(char *temp, Specificator specificator, va_list vl);
void FormatPrecision(char *temp, Specificator Specificator);
void FormatFlags(char *temp, Specificator specificator);

void ClearSpecificator(Specificator *specificator);
bool IsDigit(char symb);
int StrToInt(const char *str, int size);
void IntToStr(char *string, int64_t num);
int Order(int pow);
void ReverseString(char *string, int length);
void FormatChar(char *temp, Specificator specificator, char c);
void FormatWChar(char *temp, Specificator specificator, wchar_t wchar);
void DoubleToString(char *temp, Specificator specificator, long double value);
int64_t OrdersInNumber(int64_t num);
void eSignPow(char *temp, Specificator specificator, int64_t pow);
void FormatG(char *temp, int precision);
void RemoveZeroes(char *temp);
void OctToStr(char *temp, int64_t value);
int64_t OctToInt(int64_t value);
void FormatString(char *dest, Specificator specificator, char *string1,
                  wchar_t *string2);
void UIntToStr(char *string, uint64_t num);
void DecimalToHex(char *dest, uint64_t value);
void ToLowerCase(char *s);
void AddAdress(char *temp, Specificator specificator);

void ParseDI(char *temp, Specificator specificator, va_list vl);
void ParseC(char *temp, Specificator specificator, va_list vl);
void ParseE(char *temp, Specificator specificator, va_list vl);
void ParseF(char *temp, Specificator specificator, va_list vl);
void ParseG(char *temp, Specificator specificator, va_list vl);
void ParseO(char *temp, Specificator specificator, va_list vl);
void ParseS(char *temp, Specificator specificator, va_list vl);
void ParseU(char *temp, Specificator specificator, va_list vl);
void ParseX(char *temp, Specificator specificator, va_list vl);
void ParseP(char *temp, Specificator specificator, va_list vl);

#endif
