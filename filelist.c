#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "parson.h"
#include "credentials.h"

/*
https://filelist.io/api.php?username=x&passkey=x&action=search-torrents&type=imdb&query=tt0103969
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

void _ovrd_filelist(char* query) {
    credentials creds = read_credentials("creds_filelist.txt");
    
    CURL *curl_handle;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();

    char* json_buf = NULL;
    if (curl_handle) {
        char url[512];
        sprintf(url, "https://filelist.io/api.php?username=%s&passkey=%s&action=search-torrents&type=imdb&query=%s", creds.username, creds.passkey, query);
        curl_easy_setopt(curl_handle, CURLOPT_URL, url);

        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_mem_cb);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &json_buf);

        res = curl_easy_perform(curl_handle);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl_handle);
    }

    JSON_Value *root_value;
    JSON_Array *torrents;
    JSON_Object *torrent;

    root_value = json_parse_string(json_buf);
    if (json_value_get_type(root_value) != JSONArray) {
        fprintf(stderr, "invalid json\n");
        exit(1);
    }

    torrents = json_value_get_array(root_value);
    printf("%-50s %s\n", "Name", "Download Link");
    for (size_t i = 0; i < json_array_get_count(torrents); i++) {
        torrent = json_array_get_object(torrents, i);
        printf("%-50s %s\n",
            json_object_dotget_string(torrent, "name"),
            json_object_get_string(torrent, "download_link")
        );
    }

    curl_global_cleanup();
    free(json_buf);
    exit(0);
    
}
void (*filelist)(char*) = _ovrd_filelist;