#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *div, char *file)
{

    int fd;
    struct dirent de;
    struct stat st;
    char buf[512];
    if ((fd = open(div, 0)) < 0)
    {
        fprintf(2, "ls: cannot open %s\n", div);
        return;
    }
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "ls: cannot stat %s\n", div);
        close(fd);
        return;
    }

    if (st.type != T_DIR)
    {
        fprintf(2, "find: %s is not a directory\n", div);
        close(fd);
        return;
    }

    if (strlen(div) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        fprintf(2, "find: directory too long\n");
        close(fd);
        return;
    }

    strcpy(buf, div);
    char *p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0)
            continue;
        if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
            continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0)
        {
            fprintf(2, "ls: cannot stat %s\n", div);
            close(fd);
            return;
        }

        if (st.type == T_FILE && !strcmp(de.name, file))
        {
            printf("%s\n", buf);
        }

        if (st.type == T_DIR)
        {
            find(buf, file);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc == 3)
        find(argv[1], argv[2]);

    exit(0);
}