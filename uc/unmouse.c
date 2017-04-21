#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

void mysignal(int signum)
{
    printf("user enter [Ctrl + C] Quit!\n");
    exit(0);
}

int main(void)
{
    if (SIG_ERR == signal(SIGINT, mysignal)) {
        perror("signal");
        return -1;
    }

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = -1;

    int fd = open("/dev/input/mouse0", O_RDONLY);
    if (-1 == fd) {
        perror("opne");
        return -1;
    }

    if (-1 == fcntl(fd, F_SETLK, &lock)) {
        perror("fcntl");
        return -1;
    }

    for (;;);
    close(fd);
    return 0;
}
