#include "credentials.h"

credentials read_credentials(const char* filename) {
    credentials creds;
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "unable to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    if (fscanf(file, "%s %s", creds.username, creds.passkey) != 2) {
        fprintf(stderr, "invalid credentials file format\n");
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return creds;
}
