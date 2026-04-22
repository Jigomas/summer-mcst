#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static const char* errname(int e) {
    if (e == EPERM)
        return "EPERM";
    if (e == ENOENT)
        return "ENOENT";
    if (e == EINTR)
        return "EINTR";
    if (e == EIO)
        return "EIO";
    if (e == EBADF)
        return "EBADF";
    if (e == EAGAIN)
        return "EAGAIN";
    if (e == ENOMEM)
        return "ENOMEM";
    if (e == EACCES)
        return "EACCES";
    if (e == EBUSY)
        return "EBUSY";
    if (e == EEXIST)
        return "EEXIST";
    if (e == EINVAL)
        return "EINVAL";
    if (e == EISDIR)
        return "EISDIR";
    if (e == ENODEV)
        return "ENODEV";
    if (e == ENOTDIR)
        return "ENOTDIR";
    if (e == ENOSPC)
        return "ENOSPC";
    if (e == EPIPE)
        return "EPIPE";
    if (e == ERANGE)
        return "ERANGE";
    if (e == ENOSYS)
        return "ENOSYS";
    if (e == ELOOP)
        return "ELOOP";
    return "EUNKNOWN";
}

int main(void) {
    int fd = open("/this/file/does/not/exist", O_RDONLY);
    if (fd >= 0) {
        close(fd);
        return 0;
    }

    int err = errno;

    printf("errno value : %d\n", err);
    printf("strerror    : %s\n", strerror(err));
    perror("Can't open file");
    printf("Errno       = %s\n", errname(err));

    return err;
}
