#ifndef CREDENTIALS_H
#define CREDENTIALS_H

typedef struct {
    char username[100];
    char passkey[100];
} credentials;

credentials read_credentials(const char* filename);

#endif /* CREDENTIALS_H */
