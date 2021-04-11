#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p[2];
    char byte1 = 'o';

    pipe(p);

    if (fork() == 0) //child process
    {
        char buf[1];
        int id = getpid();
        read(p[0], buf, 1);
        close(p[0]);

        write(p[1], buf, 1);
        close(p[1]);
        printf("%c", buf[0]);
        printf("<%d>:received ping\n", id);
        exit(0);
    }
    else
    {
        char buf[1];
        int status;
        int id = getpid();
        write(p[1], &byte1, 1);
        close(p[1]);
        wait(&status);
        read(p[0], buf, 1);
        close(p[0]);

        printf("%c", buf[0]);
        printf("<%d>:received pong\n", id);
    }
    exit(0);
}