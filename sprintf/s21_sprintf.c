#include "s21_sprintf.h"

#include <math.h>
void FormatFlags(char *temp, Specificator specificator) {
  char tmp[512] = {'\0'};
  bool sign = specificator.plus || specificator.space || (*temp == '-'),
       minus = 0;
  if (specificator.plus && specificator.specificator != 'u' &&
      specificator.width >= s21_strlen(temp) + sign) {
    sign = 0;
    tmp[0] = temp[0] == '-' ? temp[0] : '+';
    s21_strcpy(tmp + 1, temp[0] == '-' ? temp + 1 : temp);
    s21_strcpy(temp, tmp);
  } else if (specificator.space && temp[0] != '-' &&
             specificator.specificator != 'u' &&
             specificator.width >= s21_strlen(temp) + sign) {
    sign = 0;
    tmp[0] = ' ';
    s21_strcpy(tmp + 1, temp);
    s21_strcpy(temp, tmp);
  } else if (!specificator.zero && *temp == '-' &&
             specificator.specificator != 'u') {
    tmp[0] = '-';
    s21_strcpy(tmp + 1, temp);
    sign = 0;
  }
  char *tempBeginning = temp;
  if (*temp == '-' && specificator.zero) {
    ++temp;
    minus = 1;
  }
  if (specificator.width > s21_strlen(temp)) {
    int idx = specificator.width - s21_strlen(temp) - sign;
    if (!specificator.minus) {
      s21_memset(tmp, specificator.zero ? '0' : ' ', idx);
      s21_strcpy(tmp + idx, temp);
    } else {
      s21_strcpy(tmp, temp);
      s21_memset(tmp + s21_strlen(tmp), ' ', idx);
    }
  } else {
    s21_strcpy(tmp, temp);
  }
  temp = tempBeginning;
  char tmp2[512] = {'\0'};
  if (specificator.plus && specificator.specificator != 'u' && tmp[0] != '+' &&
      sign) {
    tmp2[0] = temp[0] == '-' ? temp[0] : '+';
    s21_strcpy(tmp2 + 1, temp[0] == '-' ? tmp + 1 : tmp);
  } else if (specificator.space && temp[0] != '-' &&
             specificator.specificator != 'u' && sign) {
    tmp2[0] = ' ';
    s21_strcpy(tmp2 + 1, tmp);
  } else if (minus && sign) {
    tmp2[0] = '-';
    s21_strcpy(tmp2 + 1, tmp);
  } else {
    s21_strcpy(tmp2, tmp);
  }
  s21_strcpy(temp, tmp2);
}

int s21_sprintf(char *str, const char *format, ...) {
  va_list vl;
  va_start(vl, format);
  char *strBeginning;
  strBeginning = str;
  Specificator specificator = {0};

  while (*format) {
    if (*format == '%') {
      ClearSpecificator(&specificator);
      ++format;
    } else {
      *str++ = *format++;
      continue;
    }

    format = GetFlags(&specificator, format);
    format = GetWidth(&specificator, format, vl);
    format = GetPrecision(&specificator, format, vl);
    format = GetLength(&specificator, format);
    format = GetSpecificator(&specificator, format);

    char temp[512] = {'\0'};
    ProcessSpecifiers(temp, specificator, vl);
    for (int i = 0; temp[i]; ++i, ++str) {
      *str = temp[i];
    }

    if (specificator.specificator == 'n') {
      int *givenArgument = va_arg(vl, int *);
      *givenArgument = str - strBeginning;
    }
  }
  *str = '\0';
  va_end(vl);
  return str - strBeginning;
}

void ProcessSpecifiers(char *temp, Specificator specificator, va_list vl) {
  char c = specificator.specificator;
  if (c == 'd' || c == 'i') {
    ParseDI(temp, specificator, vl);
  } else if (c == 'c') {
    ParseC(temp, specificator, vl);
  } else if (c == 'e' || c == 'E') {
    ParseE(temp, specificator, vl);
  } else if (c == 'f') {
    ParseF(temp, specificator, vl);
  } else if (c == 'o') {
    ParseO(temp, specificator, vl);
  } else if (c == 's') {
    ParseS(temp, specificator, vl);
  } else if (c == 'u') {
    ParseU(temp, specificator, vl);
  } else if (c == 'x' || c == 'X') {
    ParseX(temp, specificator, vl);
  } else if (c == 'p') {
    ParseP(temp, specificator, vl);
  } else if (c == '%') {
    *temp = '%';
  }
}

void ParseP(char *temp, Specificator specificator, va_list vl) {
  uint64_t pointer = va_arg(vl, uint64_t);
  DecimalToHex(temp, pointer);
  ToLowerCase(temp);
  FormatPrecision(temp, specificator);
  AddAdress(temp, specificator);
  FormatFlags(temp, specificator);
}

void DecimalToHex(char *dest, uint64_t value) {
  uint64_t remainder;
  char *destBeginning = dest;
  while (value != 0) {
    remainder = value % 16;
    if (remainder < 10) {
      *dest = 48 + remainder;
    } else {
      *dest = 55 + remainder;
    }
    ++dest;
    value /= 16;
  }
  *dest = '\0';
  ReverseString(destBeginning, s21_strlen(destBeginning));
}

void ToLowerCase(char *s) {
  while (*s) {
    *s = tolower((unsigned char)*s);
    s++;
  }
}

void AddAdress(char *temp, Specificator specificator) {
  s21_memmove(temp + 2, temp, s21_strlen(temp));
  *temp = '0';
  ++temp;
  if (specificator.specificator == 'p') {
    *temp = 'x';
  } else {
    *temp = 'X';
  }
  ++temp;
}

void ParseX(char *temp, Specificator specificator, va_list vl) {
  uint64_t value = va_arg(vl, uint64_t);
  if (specificator.length == 'h') {
    value = (uint16_t)value;
  } else if (specificator.length == 'l') {
    value = (uint64_t)value;
  } else if (specificator.length == '0') {
    value = (uint32_t)value;
  }

  char *tempBeginning = temp;

  DecimalToHex(tempBeginning, value);
  FormatPrecision(tempBeginning, specificator);

  if (specificator.sharp == 1) {
    AddAdress(temp, specificator);
  }

  FormatFlags(tempBeginning, specificator);

  if (specificator.specificator == 'x') {
    ToLowerCase(temp);
  }
}

void UIntToStr(char *string, uint64_t num) {
  char *iterator = string;
  int counter = 0;
  while (num > 9) {
    *iterator = (num % 10) + '0';
    ++iterator;
    ++counter;
    num /= 10;
  }
  *iterator = num + '0';
  ++iterator;
  ++counter;

  iterator = string;
  ReverseString(iterator, counter);
}

void ParseU(char *temp, Specificator specificator, va_list vl) {
  uint64_t value = va_arg(vl, uint64_t);
  if (specificator.length == 'h') {
    value = (uint16_t)value;
  } else if (specificator.length == '0') {
    value = (uint32_t)value;
  }

  UIntToStr(temp, value);
  FormatPrecision(temp, specificator);
  FormatFlags(temp, specificator);
}

void ParseS(char *temp, Specificator specificator, va_list vl) {
  char *str = va_arg(vl, char *);
  FormatString(temp, specificator, str, NULL);
}

void FormatString(char *dest, Specificator specificator, char *string1,
                  wchar_t *string2) {
  char temp[512];
  if (string2 != NULL) {
    wcstombs(temp, string2, 512);
  } else {
    s21_strcpy(temp, string1);
  }

  if (specificator.isPrecisionSet == 1) {
    temp[specificator.precision] = '\0';
  }

  int spaceCounter = specificator.width - s21_strlen(temp);

  if (spaceCounter > 0) {
    if (specificator.minus == 1) {
      s21_strcpy(dest, temp);
      s21_memset(dest + s21_strlen(temp), ' ', spaceCounter);
    } else {
      s21_memset(dest, ' ', spaceCounter);
      s21_strcpy(dest + spaceCounter, temp);
    }
  } else {
    s21_strcpy(dest, temp);
  }
}

void ParseO(char *temp, Specificator specificator, va_list vl) {
  int64_t value = va_arg(vl, int64_t);
  char *tempIterator = temp;
  if (specificator.sharp == 1) {
    *tempIterator = '0';
    ++tempIterator;
    OctToStr(tempIterator, value);
  } else {
    OctToStr(temp, value);
  }
  FormatPrecision(temp, specificator);
  FormatFlags(temp, specificator);
}

void OctToStr(char *temp, int64_t value) { IntToStr(temp, OctToInt(value)); }

int64_t OctToInt(int64_t value) {
  int64_t octValue = 0;
  int64_t counter = 1;
  while (value != 0) {
    octValue += (value % 8) * counter;
    value /= 8;
    counter *= 10;
  }
  return octValue;
}

void ParseF(char *temp, Specificator specificator, va_list vl) {
  long double value = 0;
  if (specificator.length == 'L') {
    value = va_arg(vl, long double);
  } else {
    value = va_arg(vl, double);
  }

  if (specificator.isPrecisionSet == 0) {
    specificator.precision = 6;
  }

  DoubleToString(temp, specificator, value);
  FormatFlags(temp, specificator);
}

void ParseE(char *temp, Specificator specificator, va_list vl) {
  long double value = 0;
  if (specificator.length == 'L') {
    value = va_arg(vl, long double);
  } else {
    value = va_arg(vl, double);
  }
  int negative = 1;
  if (value < 0.0) {
    negative = -1;
    value *= -1.0;
  }
  int pow = 0;
  if (value >= 10.0) {
    while (value >= 10) {
      value /= 10.0;
      ++pow;
    }
  } else if (value < 1.0 && value > 0.0) {
    while (value < 1.0 && value > 0.0) {
      value *= 10;
      --pow;
    }
  }
  if (specificator.isPrecisionSet == 0) {
    specificator.precision = 6;
  }
  value *= negative;
  DoubleToString(temp, specificator, value);
  eSignPow(temp + s21_strlen(temp), specificator, pow);
  FormatFlags(temp, specificator);
}

void eSignPow(char *temp, Specificator specificator, int64_t pow) {
  if (specificator.specificator == 'e') {
    *temp = 'e';
  } else {
    *temp = 'E';
  }
  ++temp;

  if (pow >= 0) {
    *temp = '+';
  } else {
    *temp = '-';
    pow *= -1;
  }
  ++temp;

  if (pow < 10) {
    *temp = '0';
    ++temp;
  }

  IntToStr(temp, pow);
}

void DoubleToString(char *temp, Specificator specificator, long double value) {
  bool negative = 0;
  if (value < 0.0) {
    value *= -1.0;
    negative = 1;
  }
  long double fL = 0;
  long double fR = modfl(value, &fL);
  if (specificator.precision == 0) {
    fL = roundl(value);
    fR = 0;
  }
  int nullsBeforeNum = 0;
  long double tempFR = fR;
  if (fR != 0) {
    while (tempFR < 1.0) {
      tempFR *= 10.0;
      ++nullsBeforeNum;
    }
    --nullsBeforeNum;
  }
  for (int i = 0; i < specificator.precision; ++i) {
    fR *= 10;
  }
  int64_t right = roundl(fR), left = fL;
  if (negative == 1) {
    *temp = '-';
    ++temp;
  }
  IntToStr(temp, left);
  int64_t leftSize = s21_strlen(temp) + 1;
  temp += s21_strlen(temp);
  if (specificator.precision != 0 || specificator.sharp == 1) {
    *temp = '.';
    ++temp;
  }
  if (right == 0) {
    for (int64_t i = 0; i < specificator.precision; ++i) {
      *temp = '0';
      ++temp;
    }
  } else {
    for (int i = 0; i < nullsBeforeNum; ++i) {
      *temp = '0';
      ++temp;
    }
    IntToStr(temp, right);
    temp += s21_strlen(temp) - leftSize;
  }
}

void ParseDI(char *temp, Specificator specificator, va_list vl) {
  if (specificator.length == '0') {
    int value = va_arg(vl, int);
    IntToStr(temp, value);
  } else if (specificator.length == 'h') {
    int16_t value = va_arg(vl, int);
    IntToStr(temp, value);
  } else {
    int64_t value = va_arg(vl, int64_t);  // если 'l', то оно и остается int64_t
    IntToStr(temp, value);
  }
  FormatPrecision(temp, specificator);
  FormatFlags(temp, specificator);
}

void ParseC(char *temp, Specificator specificator, va_list vl) {
  char c;
  c = va_arg(vl, int);
  FormatChar(temp, specificator, c);
}

void FormatChar(char *temp, Specificator specificator, char c) {
  if (specificator.width && specificator.minus == 0) {
    for (size_t i = 0; i < specificator.width; ++i) {
      if (i != specificator.width - 1) {
        *temp = ' ';
      } else {
        *temp = c;
      }
      ++temp;
    }
  } else if (specificator.width) {
    *temp = c;
    ++temp;
    for (size_t i = 1; i < specificator.width; ++i, ++temp) {
      *temp = ' ';
    }
  } else {
    *temp = c;
    ++temp;
  }
}

void FormatPrecision(char *temp, Specificator specificator) {
  int counter = 0;
  int len = s21_strlen(temp);
  char *tempBeginning = temp;
  char *buffer;
  buffer = (char *)malloc(512 * sizeof(char));
  char *bufferBeginning = buffer;
  if (len - (*tempBeginning == '-') < specificator.precision) {
    if (*temp == '-') {
      *buffer = '-';
      ++buffer;
      ++temp;
      ++counter;
    }
    for (int i = 0; i < specificator.precision - len + counter; ++i) {
      *buffer = '0';
      ++buffer;
    }

    for (int i = 0; i < len; ++i) {
      *buffer = *temp;
      ++buffer;
      ++temp;
    }
    *buffer = '\0';
    s21_strcpy(tempBeginning, bufferBeginning);
  }
  free(bufferBeginning);
}

bool IsDigit(char symb) {
  if (symb >= '0' && symb <= '9')
    return 1;
  else
    return 0;
}

int StrToInt(const char *str, int size) {
  int res = 0;
  int pow = 0;
  for (int i = size - 1; i >= 0; i--, pow++) {
    res += (str[i] - '0') * Order(pow);
  }
  return res;
}

int Order(int pow) {
  int res = 1;
  for (int i = 0; i < pow; i++) {
    res *= 10;
  }
  return res;
}

void IntToStr(char *string, int64_t num) {
  bool negative = 0;
  if (num < 0) {
    num *= -1;
    negative = 1;
  }
  char *iterator = string;
  int counter = 0;
  while (num > 9) {
    *iterator = (num % 10) + '0';
    ++iterator;
    ++counter;
    num /= 10;
  }
  *iterator = num + '0';
  ++iterator;
  ++counter;

  if (negative) {
    *iterator = '-';
    ++iterator;
    ++counter;
  }

  iterator = string;

  ReverseString(iterator, counter);
}

void ReverseString(char *string, int length) {
  for (int i = 0; i < length / 2; ++i) {
    char left = *(string + i);
    char right = *(string + length - i - 1);
    *(string + i) = right;
    *(string + length - i - 1) = left;
  }
}

void ClearSpecificator(Specificator *specificator) {
  specificator->minus = 0;
  specificator->plus = 0;
  specificator->space = 0;
  specificator->sharp = 0;
  specificator->zero = 0;
  specificator->width = 0;
  specificator->precision = 0;
  specificator->length = '0';
  specificator->specificator = '0';
  specificator->isPrecisionSet = 0;
}

const char *GetFlags(Specificator *specificator, const char *format) {
  while (*format == '-' || *format == '+' || *format == ' ' || *format == '#' ||
         *format == '0') {
    if (*format == '-') {
      specificator->minus = 1;
    } else if (*format == '+') {
      specificator->plus = 1;
    } else if (*format == ' ') {
      specificator->space = 1;
    } else if (*format == '#') {
      specificator->sharp = 1;
    } else if (*format == '0') {
      specificator->zero = 1;
    }
    ++format;
  }
  return format;
}

const char *GetWidth(Specificator *specificator, const char *format,
                     va_list vl) {
  if (*format == '*') {
    specificator->width = va_arg(vl, int);
    ++format;
  } else {
    char buffer[256];
    int counter = 0;
    while (IsDigit(*format)) {
      buffer[counter] = *format;
      ++counter;
      ++format;
    }
    buffer[counter] = '\0';
    specificator->width = StrToInt(buffer, counter);
  }
  return format;
}

const char *GetPrecision(Specificator *specificator, const char *format,
                         va_list vl) {
  if (*format == '.') {
    ++format;
    if (*format == '*') {
      specificator->precision = va_arg(vl, int);
      specificator->isPrecisionSet = 1;
      ++format;
    } else {
      char buffer[256];
      int counter = 0;
      while (IsDigit(*format)) {
        buffer[counter] = *format;
        ++counter;
        ++format;
      }
      buffer[counter] = '\0';
      specificator->precision = StrToInt(buffer, counter);
      specificator->isPrecisionSet = 1;
    }
  }
  return format;
}

const char *GetLength(Specificator *specificator, const char *format) {
  if (*format == 'h') {
    specificator->length = 'h';
    ++format;
  } else if (*format == 'l') {
    specificator->length = 'l';
    ++format;
  } else if (*format == 'L') {
    specificator->length = 'L';
    ++format;
  }
  return format;
}

const char *GetSpecificator(Specificator *specificator, const char *format) {
  if (*format == 'c') {
    specificator->specificator = 'c';
    ++format;
  } else if (*format == 'd') {
    specificator->specificator = 'd';
    ++format;
  } else if (*format == 'i') {
    specificator->specificator = 'i';
    ++format;
  } else if (*format == 'e') {
    specificator->specificator = 'e';
    ++format;
  } else if (*format == 'E') {
    specificator->specificator = 'E';
    ++format;
  } else if (*format == 'f') {
    specificator->specificator = 'f';
    ++format;
  } else if (*format == 'o') {
    specificator->specificator = 'o';
    ++format;
  } else if (*format == 's') {
    specificator->specificator = 's';
    ++format;
  } else if (*format == 'u') {
    specificator->specificator = 'u';
    ++format;
  } else if (*format == 'x') {
    specificator->specificator = 'x';
    ++format;
  } else if (*format == 'X') {
    specificator->specificator = 'X';
    ++format;
  } else if (*format == 'p') {
    specificator->specificator = 'p';
    ++format;
  } else if (*format == 'n') {
    specificator->specificator = 'n';
    ++format;
  } else if (*format == '%') {
    specificator->specificator = '%';
    ++format;
  }
  return format;
}
