#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define R 0
#define W 1
void redirect(int fd, int p[])
{
    close(fd);
    dup(p[fd]);
    close(p[R]);
    close(p[W]);
}
void create()
{
    int pip[2], p;
    if (read(R, &p, sizeof(p)))
    {
        printf("prime %d\n", p);
        pipe(pip);       // new pipe
        if (fork() == 0) //child
        {
            redirect(R, pip); //set read from the new pipe
            create();
        }
        else
        {
            redirect(W, pip); //set write to the new pipe
            //keep read from old pipe and write to new pipe
            int n;
            while (read(R, &n, sizeof(n)))
            {
                if (n % p != 0)
                {
                    write(W, &n, sizeof(n));
                }
            }
            close(R);
            close(W);
            wait(0);
        }
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    int pip[2];

    pipe(pip);
    if (fork() == 0)
    {
        redirect(R, pip);
        create();
    }
    else
    {
        int p = 2;
        printf("prime %d\n", p);
        redirect(W, pip);
        for (int i = 3; i < 36; ++i)
        {
            if (i % p != 0)
            {
                write(W, &i, sizeof(i));
            }
        }
        close(W);
        close(R);
        wait(0);
    }
    exit(0);
}