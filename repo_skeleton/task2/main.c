#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>

static int wfd;

static void *reader(void *arg)
{
    int fd = *(int *)arg;
    char buf[512];
    ssize_t n;
    while ((n = read(fd, buf, sizeof buf)) > 0)
        write(STDOUT_FILENO, buf, n);
    return NULL;
}

static void *writer(void *arg)
{
    (void)arg;
    char buf[512];
    ssize_t n;
    while ((n = read(STDIN_FILENO, buf, sizeof buf)) > 0)
        if (write(wfd, buf, n) < 0)
            break;
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: task2 <fifo_path>\n");
        return 1;
    }

    signal(SIGPIPE, SIG_IGN);

    char back[strlen(argv[1]) + 6];
    snprintf(back, sizeof back, "%s.back", argv[1]);

    unlink(back);
    int is_a = (mkfifo(back, 0666) == 0);
    if (!is_a && errno != EEXIST) {
        perror("mkfifo");
        return 1;
    }

    int rfd;
    if (is_a) {
        wfd = open(argv[1], O_WRONLY);
        rfd = open(back,    O_RDONLY);
    } else {
        rfd = open(argv[1], O_RDONLY);
        wfd = open(back,    O_WRONLY);
    }
    if (rfd < 0 || wfd < 0) { perror("open"); return 1; }

    pthread_t tr, tw;
    pthread_create(&tr, NULL, reader, &rfd);
    pthread_create(&tw, NULL, writer, NULL);

    pthread_join(tw, NULL);
    pthread_cancel(tr);
    pthread_join(tr, NULL);

    close(rfd);
    close(wfd);
    return 0;
}
