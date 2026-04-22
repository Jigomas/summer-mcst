#include <stdio.h>
#include <string.h>
#include <getopt.h>

static const char *elbrus_vals[] = {
    "1c+", "2c+", "2c3", "4c", "8c", "16c", NULL
};

int main(int argc, char *argv[])
{
    static struct option longopts[] = {
        {"elbrus", required_argument, NULL, 0},
        {NULL, 0, NULL, 0}
    };

    char  shorts[32];
    int   nshorts = 0;
    char *longs[64];
    int   nlongs  = 0;

    opterr = 0;

    int opt, lidx;
    while ((opt = getopt_long(argc, argv, "mcst", longopts, &lidx)) != -1) {
        switch (opt) {
        case 0: {
            int ok = 0;
            for (int i = 0; elbrus_vals[i]; i++)
                if (strcmp(optarg, elbrus_vals[i]) == 0) { ok = 1; break; }
            if (!ok) {
                printf("Incorrect option: 'elbrus=%s'\n", optarg);
                return 1;
            }
            longs[nlongs++] = optarg;
            break;
        }
        case 'm': case 'c': case 's': case 't':
            shorts[nshorts++] = (char)opt;
            break;
        default:
            if (optopt)
                printf("Incorrect option: '%c'\n", optopt);
            else
                printf("Incorrect option: '%s'\n", argv[optind - 1] + 2);
            return 1;
        }
    }

    printf("Short options:");
    for (int i = 0; i < nshorts; i++) printf(" '%c'", shorts[i]);
    putchar('\n');

    printf("Long options:");
    for (int i = 0; i < nlongs; i++) printf(" 'elbrus=%s'", longs[i]);
    putchar('\n');

    printf("Non options:");
    for (int i = optind; i < argc; i++) printf(" '%s'", argv[i]);
    putchar('\n');

    return 0;
}
