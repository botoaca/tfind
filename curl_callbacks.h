#ifndef CURL_CALLBACKS_H
#define CURL_CALLBACKS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* response;
    size_t size;
} memory;

size_t write_mem_cb(void *data, size_t size, size_t nmemb, void *clientp);

#endif /* CURL_CALLBACKS_H */