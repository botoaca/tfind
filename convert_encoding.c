#include "convert_encoding.h"

char* convert_windows1251_to_utf8(const char *windows1251_string) {
    char* utf8_string = NULL;

#ifdef _WIN32
    int utf8_length = MultiByteToWideChar(1251, 0, windows1251_string, -1, NULL, 0);
    if (utf8_length == 0) return NULL;
    wchar_t *utf16_string = (wchar_t *)malloc(utf8_length * sizeof(wchar_t));
    if (utf16_string == NULL) return NULL;
    MultiByteToWideChar(1251, 0, windows1251_string, -1, utf16_string, utf8_length);
    utf8_length = WideCharToMultiByte(CP_UTF8, 0, utf16_string, -1, NULL, 0, NULL, NULL);
    utf8_string = (char *)malloc(utf8_length);
    if (utf8_string == NULL) {
        free(utf16_string);
        return NULL;
    }
    WideCharToMultiByte(CP_UTF8, 0, utf16_string, -1, utf8_string, utf8_length, NULL, NULL);
    free(utf16_string);
#else
    iconv_t converter = iconv_open("UTF-8", "WINDOWS-1251");
    if (converter == (iconv_t)-1) return NULL;
    size_t windows1251_length = strlen(windows1251_string);
    size_t utf8_length = windows1251_length * 4; // assuming worst-case expansion
    utf8_string = (char*)malloc(utf8_length);
    if (utf8_string == NULL) {
        iconv_close(converter);
        return NULL;
    }
    char* inbuf = (char*)windows1251_string;
    char* outbuf = utf8_string;
    size_t result = iconv(converter, &inbuf, &windows1251_length, &outbuf, &utf8_length);
    if (result == (size_t)-1) {
        free(utf8_string);
        utf8_string = NULL;
    }
    iconv_close(converter);
#endif

    return utf8_string;
}