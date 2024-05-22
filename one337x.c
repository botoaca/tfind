#include <stdio.h>
#include <curl/curl.h>

void _ovrd_one337x(char* query) {
    printf("%s", query);
}
void (*one337x)(char*) = _ovrd_one337x;