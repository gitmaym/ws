#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage: %s <src> <dst>\n", argv[0]);
        return 0;
    }

    struct stat st;
    if (-1 == stat(argv[1], &st)) {
        perror("stat");
        return -1;
    }

    int src = open(argv[1], O_RDONLY, st.st_mode);
    if (-1 == src) {
        perror("open");
        return -1;
    }

    int dst = open(argv[2], O_RDWR | O_CREAT | O_EXCL, st.st_mode);
    if (-1 == dst) {
        if (EEXIST != errno) {
            perror("open");
            return -1;
        }

        printf("file: %s exist, if trunc ? (Y/n): ", argv[2]);
        int c = getchar();
        if (c != 'Y' || c != 'y') return 0;

        if (-1 == (dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, st.st_mode))) {
            perror("open");
            return -1;
        }
    }

    unsigned char buf[1024] = {};
    ssize_t bytes;
    while (0 < (bytes = read(src, buf, sizeof(buf))))
        if (-1 == write(dst, buf, bytes)) {
            perror("write");
            return -1;
        }

    if (-1 == bytes) {
        perror("read");
        return -1;
    }

    close(dst);
    close(src);
    return 0;
}
