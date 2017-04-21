#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#define USER_FILE "user.dat"

struct tag_user {
    char name[256];
    char passwd[256];
};

typedef int (*MENU)(void);
typedef int (*ONMENU)(void);

void menuLoop(MENU menu, ONMENU onMenu[], size_t menus)
{
    for (;;) {
        int idMenu = menu();
        if (idMenu < 0 || idMenu >= menus) printf("select invalid!\n");
        else
            if (onMenu[idMenu]() < 0) break;
    }
}

int menuLogin(void)
{
    printf("-------------------\n");
    printf("User Manager System\n");
    printf("-------------------\n");
    printf("[1] Register\n");
    printf("[2] Login\n");
    printf("[0] Quit\n");
    printf("-------------------\n");
    printf("Please select: ");
    int idMenu = -1;
    if (1 != scanf("%d", &idMenu)) scanf("%*[^\n]\n");
    return idMenu;
}

int menuUser(void)
{   
    printf("-------------------\n");
    printf("User Manager System\n");
    printf("-------------------\n");
    printf("[1] Add\n");
    printf("[2] Delete\n");
    printf("[3] Browse\n");
    printf("[0] Quit\n");
    printf("-------------------\n");
    printf("Please select: ");
    int idMenu = -1;
    if (1 != scanf("%d", &idMenu)) scanf("%*[^\n]\n");
    return idMenu;
}

int onRegister(void)
{
    struct tag_user userNew;
    printf("user name: ");
    scanf("%s", userNew.name);
    printf("user passwd: ");
    scanf("%s", userNew.passwd);

    int fd = open(USER_FILE, O_RDWR | O_CREAT, 0644);
    if (-1 == fd) {
        printf("open");
        return -1;
    }

    struct tag_user userOld;
    ssize_t bytes;
    while (0 < (bytes = read(fd, &userOld, sizeof(struct tag_user))))
        if (!strcmp(userNew.name, userOld.name)) {
            printf("User: %s exist, register failed!\n", userNew.name);
            close(fd);
            return 0;
        }

    if (-1 == bytes) {
        perror("read");
        close(fd);
        return -1;
    }

    if (-1 == write(fd, &userNew, sizeof(struct tag_user))) {
        perror("write");
        close(fd);
        return -1;
    }

    printf("Register success!\n");
    return 0;
}

int onQuit(void)
{
    printf("normal quit\n");
    return -1;
}



int onAdd(void)
{
    printf("add user info...\n");
    return 0;
}

int onDelete(void)
{
    printf("delete user info...\n");
    return 0;
}

int onBrowse(void)
{
    printf("browse user info...\n");
    return 0;
}

int onLogin(void)
{
    struct tag_user userNew;
    printf("user name: ");
    scanf("%s", userNew.name);
    printf("user passwd: ");
    scanf("%s", userNew.passwd);

    int fd = open(USER_FILE, O_RDONLY, 0644);
    if (-1 == fd) {
        perror("open");
        return -1;
    }

    struct tag_user userOld;
    ssize_t bytes;
    while (0 < (bytes = read(fd, &userOld, sizeof(struct tag_user))))
        if (!strcmp(userNew.name, userOld.name)) {
            if (strcmp(userNew.passwd, userOld.passwd)) {
                printf("passwd error!\n");
                close(fd);
                return 0;
            } else
                break;
        }

    if (-1 == bytes) {
        perror("read");
        close(fd);
        return -1;
    }

    if (0 == bytes) {
        printf("user name error, login failed!\n");
        close(fd);
        return 0;
    }

    close(fd);

    ONMENU onMenu[] = {onQuit, onAdd, onDelete, onBrowse};
    menuLoop(menuUser, onMenu, sizeof(onMenu) / sizeof(onMenu[0]));
    return 0;
}

int main(void)
{
    ONMENU onMenu[] = {onQuit, onRegister, onLogin};
    menuLoop(menuLogin, onMenu, sizeof(onMenu) / sizeof(onMenu[0]));
    return 0;
}
