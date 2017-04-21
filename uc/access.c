#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return -1;
    }

    printf("file: %s => ", argv[1]);

    if (-1 == access(argv[1], F_OK)) {
        printf("not exist(%m) \n");
    } else {
        if (-1 == access(argv[1], R_OK)) {
            printf("not read(%m), ");        
        } else {
            printf("read, ");
        }

        if (-1 == access(argv[1], W_OK)) {
            printf("not write(%m), ");        
        } else {
            printf("write, ");
        }

        if (-1 == access(argv[1], X_OK)) {
            printf("not exec(%m).\n");        
        } else {
            printf("exec.\n");
        }
    }
    return 0;
}
