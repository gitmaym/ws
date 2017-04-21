#include <stdio.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s <dir> \n", argv[0]);
        return -1;
    }

    DIR *dp = opendir(argv[1]);
    if (!dp) {
        perror("opendir");
        return -1;
    }

    errno = 0;
    struct dirent *de;

    for (de = readdir(dp); de; de = readdir(dp)) {
        switch (de->d_type) {
            case DT_DIR:
                printf("    directory:    ");
                break;
            case DT_REG:
                printf("    general file:   ");
                break;
            case DT_LNK:
                printf("    link file:  ");
                break;
            case DT_BLK:
                printf("    black file: ");
                break;
            case DT_CHR:
                printf("    char file:  ");
                break;
            case DT_SOCK:
                printf("    unix socket:    ");
                break;
            case DT_FIFO:
                printf("    name pipeline:  ");
                break;
            default:
                printf("    unknown:    ");
                break;
        }
        printf("%s (%ul) \n", de->d_name, de->d_ino);
    }

    if (errno) {
        perror("readdir");
        return -1;
    }

    closedir(dp);
    return 0;
}
