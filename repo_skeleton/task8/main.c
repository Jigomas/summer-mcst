#define _POSIX_C_SOURCE 199309L
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define N 100

static int          A[N][N], B[N][N], C[N][N];
static volatile int gi, gj, gk;

static void handler(int sig) {
    (void) sig;
    char buf[64];
    int  n = snprintf(buf, sizeof buf, "%d %d\n%d %d\n%d %d\n", gi, gk, gk, gj, gi, gj);
    write(STDOUT_FILENO, buf, n);
    signal(SIGINT, SIG_DFL);
}

static void handler_sa(int sig) {
    (void) sig;
    char buf[64];
    int  n = snprintf(buf, sizeof buf, "%d %d\n%d %d\n%d %d\n", gi, gk, gk, gj, gi, gj);
    write(STDOUT_FILENO, buf, n);
    struct sigaction def = {.sa_handler = SIG_DFL};
    sigaction(SIGINT, &def, NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 2 || (strcmp(argv[1], "--signal") != 0 && strcmp(argv[1], "--sigaction") != 0)) {
        fprintf(stderr, "Usage: task8 --signal | --sigaction\n");
        return 1;
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            A[i][j] = i + 1;
            B[i][j] = j + 1;
        }

    if (strcmp(argv[1], "--signal") == 0) {
        signal(SIGINT, handler);
    } else {
        struct sigaction sa = {.sa_handler = handler_sa};
        sigemptyset(&sa.sa_mask);
        sigaction(SIGINT, &sa, NULL);
    }

    struct timespec half = {.tv_nsec = 500000000L};
    for (gi = 0; gi < N; gi++)
        for (gj = 0; gj < N; gj++)
            for (gk = 0; gk < N; gk++) {
                C[gi][gj] += A[gi][gk] * B[gk][gj];
                nanosleep(&half, NULL);
            }

    printf("Done.\n");
    return 0;
}
