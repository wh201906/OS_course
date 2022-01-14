#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

#define PATHLEN 256
std::vector<std::string> allowList = {"find", "grep", "ls", "pwd"};

void rainbow(const char *str);
std::vector<std::string> split(std::string str, std::string splitter);

int main()
{
    std::string cmd, path;
    std::vector<std::string> args;
    bool flag = true;

    rainbow("A simple shell\n");
    path.resize(PATHLEN);
    while (flag)
    {
        getcwd(&path[0], PATHLEN);
        printf("\033[1;32m"); // green, bold
        printf("Fake $hell:");
        printf("\033[1;33m"); // yellow, bold
        printf("%s", &path[0]);
        printf("\033[0m"); // cancel
        printf(">");
        getline(std::cin, cmd);
        args.clear();
        args = split(cmd, " ");
        if (args[0] == "exit")
            flag = 0;
        else if (args[0] == "cd")
        {
            size_t pos;
            std::string tmp;
            tmp = cmd + ";pwd"; // keep arguments of cd
            FILE *handle = popen(tmp.c_str(), "r");
            tmp.resize(PATHLEN);
            fgets(&tmp[0], PATHLEN, handle);
            if ((pos = tmp.find('\n')) != std::string::npos)
                tmp[pos] = '\0';
            chdir(tmp.c_str());
            pclose(handle);
        }
        else if (args[0] == "about")
        {
            std::string tmp;
            tmp = "./1.out";
            system(tmp.c_str());
        }
        else if (args[0] == "test")
        {
            std::string tmp;
            tmp = "./2.out";
            system(tmp.c_str());
        }
        else if (args[0] == "mysleep")
        {
            std::string tmp;
            tmp = "./3.out " + args[1];
            system(tmp.c_str());
        }
        else
        {
            bool executed = false;
            for (auto it = allowList.cbegin(); it != allowList.cend(); it++)
            {
                if (args[0] == *it)
                {
                    executed = true;
                    system(cmd.c_str());
                    break;
                }
            }
            if (!executed)
            {
                printf("\033[1;31m"); // red, bold
                printf("Command not found\n");
            }
        }
    }
}

void rainbow(const char *str)
{
    char color[] = {'0', '1', '2', '3', '4', '5', '7'};
    int colorId = 0;
    int i = 0;
    while (str[i] != '\0')
    {
        printf("\033[22;3%cm", color[colorId]);
        putchar(str[i]);
        if (str[i] > ' ' && str[i] < '\x7F')
        {
            colorId++;
            colorId %= sizeof(color);
        }
        i++;
    }
}

std::vector<std::string> split(std::string str, std::string splitter)
{
    std::regex re(splitter);
    return std::vector<std::string>(std::sregex_token_iterator(str.begin(), str.end(), re, -1), std::sregex_token_iterator());
}