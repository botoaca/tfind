#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int __ERROR__(char* msg, int code) { fprintf(stderr, "%s", msg); exit(code); }
#define __ER_ND __ERROR__("not defined", 1)

void _filelist() { __ER_ND; }
void (*filelist)(char*);

void _one337x() { __ER_ND; }
void (*one337x)(char*);

void _rutracker() { __ER_ND; }
void (*rutracker)(char*);

int main(int argc, char *argv[]) {
    if (argc < 3)
        return __ERROR__("Usage: tfind tracker torrent_title\n", -1);

    char title[256] = "";
    for (int i = 2; i < argc; i++) {
        strcat(title, argv[i]);
        if (i < argc - 1) strcat(title, " ");
    }       

    if (!filelist) filelist = _filelist;
    if (!one337x) one337x = _one337x;
    if (!rutracker) rutracker = _rutracker;

    if (strcmp(argv[1], "filelist") == 0) filelist(title);
    else if (strcmp(argv[1], "1337x") == 0) one337x(title);
    else if (strcmp(argv[1], "rutracker") == 0) rutracker(title);
    else return __ERROR__("invalid tracker\n", 1);

    return 0;
}