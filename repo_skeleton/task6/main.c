#define _GNU_SOURCE
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>

static void perms_str(mode_t m, char out[11]) {
    out[0]  = S_ISDIR(m)    ? 'd'
              : S_ISLNK(m)  ? 'l'
              : S_ISFIFO(m) ? 'p'
              : S_ISSOCK(m) ? 's'
              : S_ISBLK(m)  ? 'b'
              : S_ISCHR(m)  ? 'c'
                            : '-';
    out[1]  = (m & S_IRUSR) ? 'r' : '-';
    out[2]  = (m & S_IWUSR) ? 'w' : '-';
    out[3]  = (m & S_ISUID) ? 's' : (m & S_IXUSR) ? 'x' : '-';
    out[4]  = (m & S_IRGRP) ? 'r' : '-';
    out[5]  = (m & S_IWGRP) ? 'w' : '-';
    out[6]  = (m & S_ISGID) ? 's' : (m & S_IXGRP) ? 'x' : '-';
    out[7]  = (m & S_IROTH) ? 'r' : '-';
    out[8]  = (m & S_IWOTH) ? 'w' : '-';
    out[9]  = (m & S_ISVTX) ? 't' : (m & S_IXOTH) ? 'x' : '-';
    out[10] = '\0';
}

static const char* ftype(mode_t m) {
    if (S_ISREG(m))
        return "regular file";
    if (S_ISDIR(m))
        return "directory";
    if (S_ISLNK(m))
        return "symbolic link";
    if (S_ISFIFO(m))
        return "fifo";
    if (S_ISSOCK(m))
        return "socket";
    if (S_ISBLK(m))
        return "block device";
    if (S_ISCHR(m))
        return "character device";
    return "unknown";
}

static void print_ts(const char* label, const struct timespec* ts) {
    char buf[32];
    strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", localtime(&ts->tv_sec));
    printf("%-8s: %s.%09ld\n", label, buf, ts->tv_nsec);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: task6 <path>\n");
        return 1;
    }

    struct stat st;
    if (lstat(argv[1], &st) != 0) {
        perror(argv[1]);
        return 1;
    }

    char p[11];
    perms_str(st.st_mode, p);

    struct passwd* pw = getpwuid(st.st_uid);
    struct group*  gr = getgrgid(st.st_gid);

    printf("  File: %s\n", argv[1]);
    printf("  Size: %-10lld Blocks: %-8lld IO Block: %ld\n",
           (long long) st.st_size,
           (long long) st.st_blocks,
           (long) st.st_blksize);
    printf("  Type: %s\n", ftype(st.st_mode));
    printf("Device: %d,%d  Inode: %llu  Links: %lu\n",
           (int) major(st.st_dev),
           (int) minor(st.st_dev),
           (unsigned long long) st.st_ino,
           (unsigned long) st.st_nlink);
    printf("Access: (%04o/%s)  Uid: (%d/%s)  Gid: (%d/%s)\n",
           (unsigned) (st.st_mode & 07777),
           p,
           (int) st.st_uid,
           pw ? pw->pw_name : "?",
           (int) st.st_gid,
           gr ? gr->gr_name : "?");
    print_ts("Access", &st.st_atim);
    print_ts("Modify", &st.st_mtim);
    print_ts("Change", &st.st_ctim);

    return 0;
}
