#include "mainwindow.h"
#include <unistd.h>
#include <vector>

#include <QApplication>

#define CHILD_NUM 3 // default num
int main(int argc, char *argv[])
{
    int childNum = CHILD_NUM;
    int pipeHandle[2];
    int i;
    std::vector<int> fpid(childNum);

    // handle arguments
    if(argc > 1)
    {
        sscanf(argv[1], "%d", &childNum);
        if(childNum < 1 || childNum > 64)
            childNum = CHILD_NUM;
    }

    // some operations before fork
    for(i = 0; i < childNum; i++)
    {
        sem_t s;
        fpid[i] = -1;
    }
    pipe(pipeHandle);
    system("rm /dev/shm/*UnnamedPipeTest_*");

    // fork
    fpid[0] = fork();
    for(i = 1; i < childNum; i++)
    {
        if(fpid[i - 1] != 0)
            fpid[i] = fork();
        else
            break;
    }

    QApplication a(argc, argv);
    MainWindow w;

    // detect identity
    for(i = 0; i < childNum; i++)
    {
        if(fpid[i] == 0)
            break;
    }
    if(i >= childNum)
        w.setId(0, childNum);
    else
        w.setId(i + 1, childNum);

    // pass pipe handle to the mainwindow
    w.setPipeHandle(pipeHandle[0], pipeHandle[1]);
    w.show();
    return a.exec();
}
