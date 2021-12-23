﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFile>

#include <semaphore.h>

#define BUFLEN 32 * 1024 * 1024 // 32M

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onReadButtonClicked();
    void onWriteButtonClicked();
    void onTimeout();
    void on_clearDataButton_clicked();
    void on_clearStateButton_clicked();

    void on_createButton_clicked();

    void on_openButton_clicked();

    void onMutexPButton_clicked();
    void onMutexVButton_clicked();
    void onMutexValButton_clicked();
private:
    Ui::MainWindow *ui;
    QTimer* runningTimer;

    static char buf[BUFLEN];
    int fileHandle;
    QString typeName;
    QFile::OpenModeFlag mode = QFile::NotOpen;

    sem_t* busyMutex;
    sem_t* newDataMutex;
    sem_t* receivedMutex;

    void readFile(int n);
    void writeFile(int n);
    sem_t *mySemaphoreOpen(const char *name, int val);
    sem_t *findMutexByWidget(QObject *widget);
    QString getMutexName(sem_t *mutex);
};
#endif // MAINWINDOW_H
