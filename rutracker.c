#include <stdio.h>
#include <curl/curl.h>

void _ovrd_rutracker(char* query) {
    printf("%s", query);
}
void (*rutracker)(char*) = _ovrd_rutracker;