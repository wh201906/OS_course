#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define CMDLEN 100
#define PATHLEN 256
#define AVAILABLECMDLEN 10
char cmd[CMDLEN];
char head[CMDLEN];
char tmp[CMDLEN];
char path[PATHLEN];
int flag = 1;

char allowList[][AVAILABLECMDLEN] = {"find", "grep", "ls", "pwd"};

void addEnd(char *str)
{
    int i = 0;
    while (i < CMDLEN && str[i] != '\n' && str[i] != '\0')
        i++;
    if (i >= CMDLEN)
        str[CMDLEN - 1] = '\0';
    else if (str[i] == '\n')
        str[i] = '\0';
}

int trim()
{
    int i;
    int end;
    // handle leading space
    for (i = 0; cmd[i] != '\0'; i++)
    {
        if (cmd[i] != ' ')
            break;
    }
    if (i > 0 && cmd[i] != ' ' && cmd[i - 1] == ' ')
        memmove(cmd, cmd + i, CMDLEN - i);

    // handle trailling space
    i = 0;
    while (cmd[i] != '\0')
    {
        while (cmd[i] != '\0' && cmd[i] != ' ')
            i++;

        end = i;
        while (cmd[i] == ' ')
            i++;
    }
    cmd[end] = '\0';
    return end;
}

void getHead()
{
    int i = 0;
    while (cmd[i] != '\0' && cmd[i] != ' ')
        i++;
    memcpy(head, cmd, i);
    head[i] = '\0';
}

int getParam()
{
    int i = 0;
    while (cmd[i] != '\0' && cmd[i] != ' ')
        i++;
    while (cmd[i] == ' ')
        i++;
    return i;
}

void rainbow(char* str)
{
    char color[] = {'0','1','2','3','4','5','7'};
    int colorId = 0;
    int i = 0;
    while(str[i] != '\0')
    {
        printf("\033[22;3%cm", color[colorId]);
        putchar(str[i]);
        if(str[i] > ' ' && str[i] < '\x7F')
        {
            colorId++;
            colorId %= sizeof(color);
        }
        i++;
    }
}

int main()
{
    int i;
    rainbow("A simple shell\n");
    while (flag)
    {
        getcwd(path, PATHLEN);
        printf("\033[1;32m"); // green, bold
        printf("Fake $hell:");
        printf("\033[1;33m"); // yellow, bold
        printf("%s", path);
        printf("\033[0m"); // cancel
        printf(">");
        fgets(cmd, CMDLEN, stdin);
        addEnd(cmd);
        trim();
        getHead();
        if (strcmp(head, "exit") == 0)
            flag = 0;
        else if (strcmp(head, "cd") == 0)
        {
            strcpy(tmp, cmd);
            strcat(tmp, ";pwd");
            FILE *handle = popen(tmp, "r");
            fgets(tmp, CMDLEN, handle);
            addEnd(tmp);
            chdir(tmp);
        }
        else if (strcmp(head, "about") == 0)
        {
            strcpy(tmp, "./1.out");
            system(tmp);
        }
        else if (strcmp(head, "test") == 0)
        {
            strcpy(tmp, "./2.out");
            system(tmp);
        }
        else if (strcmp(head, "mysleep") == 0)
        {
            strcpy(tmp, "./3.out ");
            strcat(tmp, cmd + getParam());
            system(tmp);
        }
        else
        {
            for (i = 0; i < sizeof(allowList) / AVAILABLECMDLEN; i++)
            {
                if (strcmp(head, allowList[i]) == 0)
                {
                    system(cmd);
                    break;
                }
            }
            if (i >= sizeof(allowList) / AVAILABLECMDLEN)
            {
                printf("\033[1;31m"); // red, bold
                printf("Command not found\n");
            }
        }
    }
    return 0;
}