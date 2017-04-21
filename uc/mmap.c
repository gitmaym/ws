#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(void)
{
    int fd = open("/tmp/map.txt", O_RDONLY);
    if (-1 == fd) {
        perror("open");
        return -1;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        return -1;
    }

    void *map = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    printf("%s\n", (char *)map);
    munmap(map, st.st_size);
    close(fd);
    return 0;
}
