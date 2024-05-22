#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "curl_callbacks.h"

/*
https://1337x.to/search/the+oak+1992/1/
*/

static char* str_replace(const char *str, char find, char replace) {
    char *result = strdup(str);
    for (int i = 0; result[i] != '\0'; i++) {
        if (result[i] == find) result[i] = replace;
    }
    return result;
}

void _ovrd_one337x(char* query) {
    CURL *curl_handle;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();

    memory chunk = {0};
    if (!curl_handle) {
        fprintf(stderr, "curl_easy_init() failed\n");
        exit(1);
    }

    char url[512];
    sprintf(url, "https://1337x.to/search/%s/1/", str_replace(query, ' ', '+'));
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_mem_cb);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&chunk);

    res = curl_easy_perform(curl_handle);

    if (res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    curl_easy_cleanup(curl_handle);

    printf("%-50s %s\n", "Name", "Download Link");
    char t_url[256], text[256];
    while ((chunk.response = strstr(chunk.response, "<td class=\"coll-1 name\"")) != NULL) {
        chunk.response = strstr(chunk.response, "<a href=\"/torrent/");
        if (chunk.response == NULL) break;
        chunk.response += 9;

        char* url_end = strstr(chunk.response, "\">");
        if (url_end == NULL) break;
        *url_end = '\0';
        sprintf(t_url, "https://1337x.to%s", chunk.response);

        chunk.response = url_end + 2;
        char* text_end = strstr(chunk.response, "</a>");
        if (text_end == NULL) break;
        *text_end = '\0';
        strcpy(text, chunk.response);

        printf("%-50.50s %s\n",
            text,
            t_url
        );

        chunk.response = text_end + 4;
    }

    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
    exit(0);
}
void (*one337x)(char*) = _ovrd_one337x;