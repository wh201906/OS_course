#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <map>

#define PATHLEN 256
std::vector<std::string> allowList = {"find", "grep", "ls", "pwd", "sleep"};
std::map<std::string, std::string> cmdMap{{"about", "./1.out"}, {"test", "./2.out"}, {"mysleep", "./3.out"}};
std::string cmd, path;
std::vector<std::string> args;

void rainbow(const char *str);                                         // show something with different color
std::vector<std::string> split(std::string str, std::string splitter); // split a string
void runCMD();                                                         // fork a new process then run the command with arguments(based on args[])

int main()
{
    bool flag = true;
    auto mapped = cmdMap.cbegin();

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
        mapped = cmdMap.find(args[0]);
        if (args[0] == "exit" || args[0] == "quit") // quit
            flag = 0;
        else if (args[0] == "cd") // cd, use external cd and pwd command as helpers
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
        else if (mapped != cmdMap.cend()) // mapped commands, "mapped" is updated when the shell reads a line
        {
            args[0] = mapped->second;
            runCMD();
        }
        else // other commands
        {
            bool executed = false;
            for (auto it = allowList.cbegin(); it != allowList.cend(); it++)
            {
                if (args[0] == *it)
                {
                    executed = true;
                    runCMD();
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

void runCMD()
{
    pid_t childPid;
    if ((childPid = fork()) == 0)
    {
        const char **argList = new const char *[args.size() + 1];
        argList[args.size()] = nullptr;
        for (size_t i = 0; i < args.size(); i++)
        {
            argList[i] = args[i].c_str();
        }
        execvp(argList[0], (char *const *)argList);
        delete[] argList;
        exit(0);
    }
    else
    {
        waitpid(childPid, nullptr, 0);
    }
}