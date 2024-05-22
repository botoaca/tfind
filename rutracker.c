#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "credentials.h"
#include "curl_callbacks.h"
#include "convert_encoding.h"

/*
https://rutracker.org/forum/tracker.php?nm=balanta%201992
*/

void _ovrd_rutracker(char* query) {
    credentials creds = read_credentials("creds_rutracker.txt");
    const char* cookies = "cookies_rutracker.txt";
    char postfields[512];
    curl_global_init(CURL_GLOBAL_ALL);
    
    CURL *curl_handle;
    CURLcode res;
    
    curl_handle = curl_easy_init();
    if (!curl_handle) {
        fprintf(stderr, "curl_easy_init() failed\n");
        exit(1);
    }

    sprintf(postfields, "login_username=%s&login_password=%s&login=%s", creds.username, creds.passkey, "Вход");
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, postfields);
    curl_easy_setopt(curl_handle, CURLOPT_URL, "https://rutracker.org/forum/login.php");
    curl_easy_setopt(curl_handle, CURLOPT_COOKIEJAR, cookies);
    res = curl_easy_perform(curl_handle);
    if (res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

    memory chunk = {0};
    memset(postfields, 0, sizeof(postfields));
    sprintf(postfields, "nm=%s", query);

    curl_easy_setopt(curl_handle, CURLOPT_URL, "https://rutracker.org/forum/tracker.php");
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, postfields);
    curl_easy_setopt(curl_handle, CURLOPT_COOKIEFILE, cookies);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_mem_cb);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&chunk);
    res = curl_easy_perform(curl_handle);
    if (res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

    char* ptr = chunk.response;
    printf("%-50s %s\n", "Name", "Download Link");
    while ((ptr = strstr(ptr, "<a data-topic_id=")) != NULL) {
        char* href_ptr = strstr(ptr, "href=\"");
        if (href_ptr == NULL) break;
        href_ptr += strlen("href=\"");
        char* href_end = strstr(href_ptr, "\"");
        if (href_end == NULL) break;
        int href_len = href_end - href_ptr;
        char* href = (char*)malloc(href_len + 1);
        strncpy(href, href_ptr, href_len);
        href[href_len] = '\0';
        char url[128];
        sprintf(url, "https://rutracker.org/forum/%s", href);

        char* text_start = strstr(ptr, ">");
        if (text_start == NULL) break;
        text_start += 1;
        char* text_end = strstr(text_start, "</a>");
        if (text_end == NULL) break;
        int text_len = text_end - text_start;
        char* text = (char*)malloc(text_len + 1);
        strncpy(text, text_start, text_len);
        text[text_len] = '\0';

        printf("%-50.250s %s\n",
            convert_windows1251_to_utf8(text),
            url
        );

        ptr = text_end + strlen("</a>");
        free(href);
        free(text);
    }

    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
    exit(0);
}

void (*rutracker)(char*) = _ovrd_rutracker;