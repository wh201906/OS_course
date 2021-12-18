#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char MainWindow::buf[BUFLEN] = {0};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    runningTimer = new QTimer();
    ui->PIDLabel->setText("PID: " + QString::number(getpid()));
    ui->parentPIDLabel->setText("Parent PID: " + QString::number(getppid()));

    connect(ui->readSomeButton, &QPushButton::clicked, this, &MainWindow::onReadButtonClicked);
    connect(ui->read1Button, &QPushButton::clicked, this, &MainWindow::onReadButtonClicked);
    connect(ui->read128Button, &QPushButton::clicked, this, &MainWindow::onReadButtonClicked);
    connect(ui->read512Button, &QPushButton::clicked, this, &MainWindow::onReadButtonClicked);
    connect(ui->read1024Button, &QPushButton::clicked, this, &MainWindow::onReadButtonClicked);
    connect(ui->read4096Button, &QPushButton::clicked, this, &MainWindow::onReadButtonClicked);
    connect(ui->read8192Button, &QPushButton::clicked, this, &MainWindow::onReadButtonClicked);

    connect(ui->writeButton, &QPushButton::clicked, this, &MainWindow::onWriteButtonClicked);
    connect(ui->writeSomeButton, &QPushButton::clicked, this, &MainWindow::onWriteButtonClicked);
    connect(ui->write1Button, &QPushButton::clicked, this, &MainWindow::onWriteButtonClicked);
    connect(ui->write128Button, &QPushButton::clicked, this, &MainWindow::onWriteButtonClicked);
    connect(ui->write512Button, &QPushButton::clicked, this, &MainWindow::onWriteButtonClicked);
    connect(ui->write1024Button, &QPushButton::clicked, this, &MainWindow::onWriteButtonClicked);
    connect(ui->write4096Button, &QPushButton::clicked, this, &MainWindow::onWriteButtonClicked);
    connect(ui->write8192Button, &QPushButton::clicked, this, &MainWindow::onWriteButtonClicked);
    connect(runningTimer, &QTimer::timeout, this, &MainWindow::onTimeout);

    runningTimer->start(500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *e)
{
    QMainWindow::showEvent(e);
}

void MainWindow::setId(int id)
{
    this->id = id;
    if(id == 0)
    {
        ui->typeLabel->setText("Father");
        ui->dataEdit->setReadOnly(true);
        ui->writeWidget->setVisible(false);
        ui->writeButton->setVisible(false);
    }
    else if(id > 0)
    {
        ui->typeLabel->setText("Child" + QString::number(id));
        ui->readWidget->setVisible(false);
    }
}

void MainWindow::appendText(const QString& text)
{
    ui->stateEdit->appendPlainText(text);
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
    readPipe(num);
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
    writePipe(num);
}

void MainWindow::readPipe(int n)
{
    int num;
    int i = 0, j = 0;
    char currData;
    if(n == -2)
        n = ui->readNumEdit->text().toUInt();
    num = read(pipeR, buf, n);
    ui->stateEdit->appendPlainText(QString("Read: expected %1 bytes, get %2 bytes").arg(n).arg(num));

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
            for(j = i; buf[j] != (char)0xFF && j < num; j++)
                ;
            ui->dataEdit->appendPlainText(QByteArray(buf + i, j - i));
        }
        i = j;
    }
}

void MainWindow::writePipe(int n)
{
    int num;
    if(n == -1)
    {
        QByteArray data = ui->dataEdit->toPlainText().toLocal8Bit();
        n = data.length();
        num = write(pipeW, data.data(), n);
    }
    else
    {
        if(n == -2)
            n = ui->writeNumEdit->text().toUInt();
        for(int i = 0; i < n; i++)
            buf[i] = 0x80 + id;
        num = write(pipeW, buf, n);
    }
    ui->stateEdit->appendPlainText(QString("Write: expected %1 bytes, put %2 bytes").arg(n).arg(num));
}

void MainWindow::setPipeHandle(int r, int w)
{
    pipeR = r;
    pipeW = w;
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

