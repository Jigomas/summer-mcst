#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int mode;

static int visible(const char *s)
{
    for (; *s; s++)
        if (isgraph((unsigned char)*s))
            return 1;
    return 0;
}

static int cmp(const void *a, const void *b)
{
    const char *s1 = *(const char **)a;
    const char *s2 = *(const char **)b;
    switch (mode) {
        case 0: return strcmp(s1, s2);
        case 1: return strcasecmp(s1, s2);
        case 2: return strcmp(s2, s1);
        case 3: return strcasecmp(s2, s1);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "Usage: task3 <input> <output> <plain|lex|rplain|rlex>\n");
        return 1;
    }

    if      (strcmp(argv[3], "plain")  == 0) mode = 0;
    else if (strcmp(argv[3], "lex")    == 0) mode = 1;
    else if (strcmp(argv[3], "rplain") == 0) mode = 2;
    else if (strcmp(argv[3], "rlex")   == 0) mode = 3;
    else { fprintf(stderr, "Unknown mode: %s\n", argv[3]); return 1; }

    FILE *fin = fopen(argv[1], "r");
    if (!fin) { perror(argv[1]); return 1; }

    char **lines = NULL;
    int n = 0, cap = 0;
    char *line = NULL;
    size_t llen = 0;
    ssize_t nr;

    while ((nr = getline(&line, &llen, fin)) > 0) {
        if (line[nr - 1] == '\n') line[nr - 1] = '\0';
        if (!visible(line)) continue;
        if (n == cap)
            lines = realloc(lines, (cap = cap ? cap * 2 : 16) * sizeof(char *));
        lines[n++] = strdup(line);
    }
    free(line);
    fclose(fin);

    qsort(lines, n, sizeof(char *), cmp);

    FILE *fout = fopen(argv[2], "w");
    if (!fout) { perror(argv[2]); return 1; }
    for (int i = 0; i < n; i++) {
        fputs(lines[i], fout);
        fputc('\n', fout);
        free(lines[i]);
    }
    fclose(fout);
    free(lines);
    return 0;
}
