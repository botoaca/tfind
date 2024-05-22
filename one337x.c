#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

/*
https://1337x.to/search/the+oak+1992/1/
*/

static size_t write_mem_cb(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char **mem = (char**)userp;
    *mem = realloc(*mem, realsize + 1);
    if (*mem == NULL) {
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
    memcpy(*mem, contents, realsize);
    (*mem)[realsize] = 0;
    return realsize;
}

char* str_replace(const char *str, char find, char replace) {
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

    char* buf = NULL;
    if (curl_handle) {
        char url[512];
        sprintf(url, "https://1337x.to/search/%s/1/", str_replace(query, ' ', '+'));
        curl_easy_setopt(curl_handle, CURLOPT_URL, url);

        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_mem_cb);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &buf);

        res = curl_easy_perform(curl_handle);

        if (res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl_handle);
    }

    printf("%-50s %s\n", "Name", "Download Link");
    char url[256], text[256];
    while ((buf = strstr(buf, "<td class=\"coll-1 name\"")) != NULL) {
        buf = strstr(buf, "<a href=\"/torrent/");
        if (buf == NULL) break;
        buf += 9;

        char *url_end = strstr(buf, "\">");
        if (url_end == NULL) break;
        *url_end = '\0';
        sprintf(url, "https://1337x.to%s", buf);

        buf = url_end + 2;
        char *text_end = strstr(buf, "</a>");
        if (text_end == NULL) {
            printf("No results found\n");
            break;
        }
        *text_end = '\0';
        strcpy(text, buf);

        printf("%-50s %s\n",
            text,
            url
        );

        buf = text_end + 4;
    }

    curl_global_cleanup();
    exit(0);
}
void (*one337x)(char*) = _ovrd_one337x;