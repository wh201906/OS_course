#include "mainwindow.h"
#include <unistd.h>
#include <vector>
#include <semaphore.h>

#include <QApplication>

#define CHILD_NUM 3 // default num
int main(int argc, char *argv[])
{
    int childNum = CHILD_NUM;
    int pipeHandle[2];
    int i;
    std::vector<int> fpid(childNum);

    sem_t pipeWritingMutex;
    std::vector<sem_t> childMutexList;

    // handle arguments
    if(argc > 1)
    {
        sscanf(argv[1], "%d", &childNum);
        if(childNum < 1 || childNum > 64)
            childNum = CHILD_NUM;
    }

    // some operations before fork
    sem_init(&pipeWritingMutex, true, 1);
    for(i = 0; i < childNum; i++)
    {
        sem_t s;
        fpid[i] = -1;
        sem_init(&s, true, 0);
        childMutexList.push_back(s);
    }
    pipe(pipeHandle);

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
        w.setId(0);
    else
        w.setId(i + 1);

    // pass pipe handle to the mainwindow
    w.setPipeHandle(pipeHandle[0], pipeHandle[1]);
    w.setMutex(&childMutexList, &pipeWritingMutex);
    w.show();
    return a.exec();
}
