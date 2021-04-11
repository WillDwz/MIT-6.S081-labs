#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "usage: xargs command\n");
        exit(1);
    }

    char *_argv[MAXARG];
    for (int i = 1; i < argc; i++) // delete xargs
        _argv[i - 1] = argv[i];

    //read
    // a char: add to a word
    // a word: add to argv
    // a line: fork exec
    char buf[MAXARG];
    char c = 0;
    int stat = 1;
    while (stat)
    {
        int buf_cur = 0;
        int buf_begin = 0;
        int arg_cur = argc - 1;
        while (1)
        {
            stat = read(0, &c, 1);
            if (stat == 0)
                exit(0);
            if (c == ' ' || c == '\n')
            {
                buf[buf_cur++] = 0;
                _argv[arg_cur++] = &buf[buf_begin];
                buf_begin = buf_cur;
                if (c == '\n')
                    break;
            }
            else
            {
                buf[buf_cur++] = c;
            }
        }
        _argv[arg_cur] = 0;
        if (fork() == 0)
        {
            exec(_argv[0], _argv);
        }
        else
        {
            wait(0);
        }
    }
    exit(0);
}
