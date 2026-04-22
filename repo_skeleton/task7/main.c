#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

static void show(const char *label, const char *path)
{
    printf("%s:\n", label);
    FILE *f = fopen(path, "r");
    if (!f) { perror(path); return; }
    int c;
    while ((c = fgetc(f)) != EOF)
        putchar(c);
    fclose(f);
}

static void copy(int src, int dst)
{
    char buf[512];
    ssize_t n;
    while ((n = read(src, buf, sizeof buf)) > 0)
        write(dst, buf, n);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: task7 <path>\n");
        return 1;
    }

    int src = open(argv[1], O_RDONLY);
    if (src < 0) { perror(argv[1]); return 1; }

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }

    if (pid == 0) {
        int dst = open("child_copy", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (dst < 0) { perror("child_copy"); _exit(1); }
        copy(src, dst);
        close(dst);
        close(src);
        _exit(0);
    }

    int dst = open("parent_copy", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst < 0) { perror("parent_copy"); return 1; }
    copy(src, dst);
    close(dst);

    wait(NULL);
    close(src);

    show("Parent", "parent_copy");
    show("Child",  "child_copy");
    return 0;
}
