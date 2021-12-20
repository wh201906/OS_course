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

    on_childMutexBox_clicked(ui->childMutexBox->isChecked());
    on_pipeWMutexBox_clicked(ui->pipeWMutexBox->isChecked());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *e)
{
    QMainWindow::showEvent(e);
}

sem_t* MainWindow::mySemaphoreOpen(const char* name, int val)
{
    sem_t* result;
    result = sem_open(name, O_CREAT | O_EXCL | O_RDWR, 0644, val);
    if(result == nullptr)
        result = sem_open(name, O_RDWR, 0644, val);
    return result;
}

void MainWindow::setId(int id, int childNum)
{
    this->id = id;
    this->childNum = childNum;
    char prefix[32] = "/UnnamedPipeTest_childMutex";
    char idStr[4];
    int prefixLen = strlen(prefix);
    childMutexList.resize(childNum + 1);
    if(id == 0)
    {
        ui->typeLabel->setText("Father");
        ui->dataEdit->setReadOnly(true);
        ui->writeWidget->setVisible(false);
        ui->writeButton->setVisible(false);
        ui->pipeWMutexBox->setVisible(false);
        ui->pipeW_PButton->setVisible(false);
        ui->pipeW_VButton->setVisible(false);
        ui->pipeW_valButton->setVisible(false);
        for(int i = 1; i <= childNum; i++)
        {
            prefix[prefixLen] = '\0';
            sprintf(idStr, "%d", i);
            strcat(prefix, idStr);
            childMutexList[i] = mySemaphoreOpen(prefix, 0);
        }
    }
    else if(id > 0)
    {
        ui->typeLabel->setText("Child" + QString::number(id));
        ui->readWidget->setVisible(false);
        ui->childMutexBox->setVisible(false);
        sprintf(idStr, "%d", id);
        strcat(prefix, idStr);
        childMutexList[id] = mySemaphoreOpen(prefix, 0);
        pipeWritingMutex = mySemaphoreOpen("/UnnamedPipeTest_pipeWritingMutex", 1);
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
    int i, j;
    char currData;

    if(allChildrenWrittenMutexOn)
    {
        for(i = 1; i <= childNum;  i++)
        {
            sem_wait(childMutexList[i]);
            ui->stateEdit->appendPlainText(QString("Mutex: V(childWritten[%1])").arg(i));
            QApplication::processEvents(); // update UI
        }
    }

    if(n == -2)
        n = ui->readNumEdit->text().toUInt();
    num = read(pipeR, buf, n);
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

void MainWindow::writePipe(int n)
{
    int val;
    int num;
    if(pipeWritingMutexOn)
    {
        sem_wait(pipeWritingMutex);
        ui->stateEdit->appendPlainText("Mutex: P(pipeWriting)");
    }
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
    sem_getvalue(childMutexList[id], &val);
    if(val < 1)
    {
        sem_post(childMutexList[id]);
        ui->stateEdit->appendPlainText(QString("Mutex: V(childWritten[%1])").arg(id));
    }
    else
    {
        ui->stateEdit->appendPlainText(QString("Mutex: V(childWritten[%1]) is ignored").arg(id));
    }
    ui->stateEdit->appendPlainText(QString("Write: expected %1 bytes, put %2 bytes").arg(n).arg(num));
    if(pipeWritingMutexOn)
    {
        sem_post(pipeWritingMutex);
        ui->stateEdit->appendPlainText("Mutex: V(pipeWriting)");
    }
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


void MainWindow::on_childMutexBox_clicked(bool checked)
{
    allChildrenWrittenMutexOn = checked;
}


void MainWindow::on_pipeWMutexBox_clicked(bool checked)
{
    pipeWritingMutexOn = checked;
}


void MainWindow::on_pipeW_PButton_clicked()
{
    int val;
    sem_getvalue(pipeWritingMutex, &val);
    if(val == 0)
    {
        ui->stateEdit->appendPlainText("Mutex: pipeWriting is 0! P() will block!");
    }
    else
    {
        sem_wait(pipeWritingMutex);
        ui->stateEdit->appendPlainText("Mutex: P(pipeWriting)");
        on_pipeW_valButton_clicked();
    }
}


void MainWindow::on_pipeW_VButton_clicked()
{
    int val;
    sem_getvalue(pipeWritingMutex, &val);
    if(val == 1)
    {
        ui->stateEdit->appendPlainText("Mutex: pipeWriting is 1! pipeWriting is a mutex!");
    }
    else
    {
        sem_post(pipeWritingMutex);
        ui->stateEdit->appendPlainText("Mutex: V(pipeWriting)");
        on_pipeW_valButton_clicked();
    }
}


void MainWindow::on_pipeW_valButton_clicked()
{
    int val;
    sem_getvalue(pipeWritingMutex, &val);
    ui->stateEdit->appendPlainText(QString("Mutex: pipeWriting: %1").arg(val));
}

