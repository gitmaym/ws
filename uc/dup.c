#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(void)
{
    int fd1 = open("dup1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (-1 == fd1) {
        perror("open");
        return -1;
    }
    printf("fd1 = %d\n", fd1);

    int fd2 = open("dup2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (-1 == fd2) {
        perror("open");
        return -1;
    }
    printf("fd2 = %d\n", fd2);

    int fd3 = dup2(fd1, fd2);
    if (-1 == fd3) {
        perror("dup2");
        return -1;
    }

    /*    
    int fd3 = fcntl(fd1, F_DUPFD, fd2);
    if (-1 == fd3) {
        perror("fcntl");
        return -1;
    }
    */
    printf("fd3 = %d\n", fd3);

    const char *text = "123";
    if (-1 == write(fd1, text, strlen(text) * sizeof(text[0]))) {
        perror("write");
        return -1;
    }

    text = "456";
    if (-1 == write(fd2, text, strlen(text) * sizeof(text[0]))) {
        perror("write");
        return -1;
    }

    text = "789";
    if (-1 == write(fd3, text, strlen(text) * sizeof(text[0]))) {
        perror("write");
        return -1;
    }

    close(fd3);
    close(fd2);
    close(fd1);

    return 0;
}
