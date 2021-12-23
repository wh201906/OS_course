#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

#include <mqueue.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

char MainWindow::buf[BUFLEN] = {0};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    runningTimer = new QTimer();
    ui->PIDLabel->setText("PID: " + QString::number(getpid()));
    ui->parentPIDLabel->setText("Parent PID: " + QString::number(getppid()));

    connect(ui->read512Button, &QPushButton::clicked, this, &MainWindow::onReadButtonClicked);

    connect(ui->writeButton, &QPushButton::clicked, this, &MainWindow::onWriteButtonClicked);
    connect(ui->write512Button, &QPushButton::clicked, this, &MainWindow::onWriteButtonClicked);
    connect(runningTimer, &QTimer::timeout, this, &MainWindow::onTimeout);

    runningTimer->start(500);

    typeName = "MQ";
    setWindowTitle(typeName);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onReadButtonClicked()
{
    QString name = sender()->objectName();
    int num = 0;
    if(name == "readSomeButton")
        num = -2;
    else
    {
        name.remove("read");
        name.remove("Button");
        num = name.toInt();
    }
    readFile(num);
}

void MainWindow::onWriteButtonClicked()
{
    QString name = sender()->objectName();
    int num = 0;
    if(name == "writeButton")
        num = -1;
    else if(name == "writeSomeButton")
        num = -2;
    else
    {
        name.remove("write");
        name.remove("Button");
        num = name.toInt();
    }
    writeFile(num);
}

void MainWindow::readFile(int n)
{
    int num;
    int i, j;
    char currData;

    num = mq_receive(fileHandle, buf, n, nullptr);
    ui->stateEdit->appendPlainText(QString("Read: expected %1 bytes, get %2 bytes").arg(n).arg(num));

    i = 0;
    j = 0;
    while(i < num && j < num)
    {
        if(buf[i] < ' ' || buf[i] > '~') // unprintable
        {
            currData = buf[i];
            for(j = i; buf[j] == currData && j < num; j++)
                ;
            ui->dataEdit->appendPlainText(QString("----- %1 * 0x%2 -----").arg(j - i).arg(QString(QByteArray(1, currData).toHex().toUpper())));
        }
        else // printable data
        {
            for(j = i; buf[j] >= ' ' && buf[j] <= '~' && j < num; j++)
                ;
            ui->dataEdit->appendPlainText(QByteArray(buf + i, j - i));
        }
        i = j;
    }
}

void MainWindow::writeFile(int n)
{
    int num;

    if(n == -1)
    {
        QByteArray data = ui->dataEdit->toPlainText().toLocal8Bit();
        n = data.length();
        num = mq_send(fileHandle, data.data(), n, 0);
    }
    else
    {
        for(int i = 0; i < n; i++)
            buf[i] = 0xFF;
        num = mq_send(fileHandle, buf, n, 0);
    }
    if(num == 0)
        ui->stateEdit->appendPlainText(QString("Write: OK"));
    else
        ui->stateEdit->appendPlainText(QString("Write: Failed: %1").arg(num));

}

void MainWindow::onTimeout()
{
    if(ui->runningLabel->styleSheet().isEmpty())
        ui->runningLabel->setStyleSheet("background-color:green;");
    else
        ui->runningLabel->setStyleSheet("");
}

void MainWindow::on_clearDataButton_clicked()
{
    ui->dataEdit->clear();
}


void MainWindow::on_clearStateButton_clicked()
{
    ui->stateEdit->clear();
}


void MainWindow::on_createButton_clicked()
{
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 512;
    QByteArray fileName;
    fileName = ui->nameEdit->text().toLocal8Bit();
    if(access(fileName, F_OK) == 0)
    {
        ui->stateEdit->appendPlainText(typeName + ": file already exists");
        if(remove(fileName) == 0)
            ui->stateEdit->appendPlainText(typeName + ": file removed");
        else
            ui->stateEdit->appendPlainText(typeName + ": failed to remove file");
    }
    fileHandle = mq_open(fileName, O_CREAT | O_EXCL | O_RDWR | O_NONBLOCK, 0644, &attr);
    ui->stateEdit->appendPlainText(typeName + ": create: " + QString::number(fileHandle));
    if(fileHandle > 0)
        ui->stateEdit->appendPlainText(typeName + ": created");
    else
    {
        ui->stateEdit->appendPlainText(typeName + ": failed to create");
        ui->stateEdit->appendPlainText(QString("Error: ") + strerror(errno));
    }
}


void MainWindow::on_openButton_clicked()
{
    QByteArray fileName;
    fileName = ui->nameEdit->text().toLocal8Bit();
    fileHandle = mq_open(fileName, O_RDWR | O_NONBLOCK);
    ui->stateEdit->appendPlainText(typeName + ": open: " + QString::number(fileHandle));
    if(fileHandle >= 0)
    {
        ui->stateEdit->appendPlainText(typeName + ": opened");
    }
    else
    {
        ui->stateEdit->appendPlainText(typeName + ": failed to open");
    }
}

