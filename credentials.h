#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char username[100];
    char passkey[100];
} credentials;

credentials read_credentials(const char* filename);

#endif /* CREDENTIALS_H */
