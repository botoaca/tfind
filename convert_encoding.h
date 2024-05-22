#ifndef CONVERT_ENCODING_H
#define CONVERT_ENCODING_H

#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <iconv.h>
#endif

char* convert_windows1251_to_utf8(const char* windows1251_string);

#endif /* CONVERT_ENCODING_H */