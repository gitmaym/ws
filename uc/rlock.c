#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/*
 * read lock test
 * return value:
 *   1   -   read lock
 *   0   -   can't read lock
 *  -1   -   system error
 */
int rtest(int fd, off_t start, off_t len)
{
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = start;
    lock.l_len = len;
    lock.l_pid = -1;

    if (fcntl(fd, F_GETLK, &lock) == -1)
        return -1;

    if (lock.l_type == F_UNLCK) 
        return 1;
    return 0;
}

/* read lock */
int rlock(int fd, off_t start, off_t len, int wait)
{
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = start;
    lock.l_len = len;
    lock.l_pid = -1;

    return fcntl(fd, wait ? F_SETLKW : F_SETLK, &lock);
}

/* read unlock */
int unlock(int fd, off_t start, off_t len)
{
    struct flock lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = start;
    lock.l_len = len;
    lock.l_pid = -1;

    return fcntl(fd, F_SETLK, &lock);
}

int main(int argc, char *argv[])
{
    int lock = 0;
    if (argc > 1) {
        if (!strcmp(argv[1], "-l"))
            lock = 1;
        else
            goto usage;
    }

    int fd = open("/tmp/lock.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    if (lock) {
        // base lock test not block mode
        int unlock = 0;
        do {
            if ((unlock = rtest(fd, 0, 0)) == -1) {
                perror("rtest");
                return -1;
            }

            if (!unlock) {
                printf("file locked, try again later on...\n");
                // ...
            }
        } while (!unlock);

        if (rlock(fd, 0, 0, 0) == -1) {
            perror("rlock");
            return -1;
        }

        // base lock failed not block mode
        while (rlock(fd, 0, 0, 0) == -1) {
            if (errno != EACCES && errno != EAGAIN) {
                perror("rlock");
                return -1;
            }
            printf("file locked, try again later on...\n");
            //...
        }

        // block mode
        if (rlock(fd, 0, 0, 0) ==  -1) {
            perror("rlock");
            return -1;
        }
    }

    /*
    char buf[1024];
    ssize_t readed;
    while (0 < (readed = read(fd, buf, sizeof(buf))))
        write(STDOUT_FILENO, buf, readed);
    */

    char buf[1];
    ssize_t readed;

    while (0 < (readed = read(fd, buf, sizeof(buf)))) {
        write(STDOUT_FILENO, buf, readed);
        sleep(1);
    }
    printf("\n");

    if (-1 == readed) {
        perror("read");
        return -1;
    }

    if (lock)
        if (unlock(fd, 0, 0) == -1) {
            perror("ulock");
            return -1;
        }

    close(fd);
    return 0;

usage:
    fprintf(stderr, "usage: %s [-l]\n", argv[0]);
    return -1;
}
