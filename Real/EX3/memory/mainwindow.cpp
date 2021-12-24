#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

#include <sys/mman.h>
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
    connect(ui->newData_PButton, &QPushButton::clicked, this, &MainWindow::onMutexPButton_clicked);
    connect(ui->newData_VButton, &QPushButton::clicked, this, &MainWindow::onMutexVButton_clicked);
    connect(ui->newData_valButton, &QPushButton::clicked, this, &MainWindow::onMutexValButton_clicked);
    connect(ui->busy_PButton, &QPushButton::clicked, this, &MainWindow::onMutexPButton_clicked);
    connect(ui->busy_VButton, &QPushButton::clicked, this, &MainWindow::onMutexVButton_clicked);
    connect(ui->busy_valButton, &QPushButton::clicked, this, &MainWindow::onMutexValButton_clicked);
    connect(ui->received_PButton, &QPushButton::clicked, this, &MainWindow::onMutexPButton_clicked);
    connect(ui->received_VButton, &QPushButton::clicked, this, &MainWindow::onMutexVButton_clicked);
    connect(ui->received_valButton, &QPushButton::clicked, this, &MainWindow::onMutexValButton_clicked);


    runningTimer->start(500);

    typeName = "memory";
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

    sem_wait(newDataMutex);
    ui->stateEdit->appendPlainText("Mutex: P(newData)");
    QApplication::processEvents(); // update UI

    num = sharedMsg->len;
    memcpy(buf, sharedMsg->data, num);
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

    sem_post(receivedMutex);
    ui->stateEdit->appendPlainText("Mutex: V(received)");
    QApplication::processEvents(); // update UI
}

void MainWindow::writeFile(int n)
{
    int num;

    sem_wait(busyMutex);
    ui->stateEdit->appendPlainText("Mutex: P(busy)");
    QApplication::processEvents(); // update UI
    if(n == -1)
    {
        QByteArray data = ui->dataEdit->toPlainText().toLocal8Bit();
        n = data.length();
        sharedMsg->len = n;
        memcpy(sharedMsg->data, data.data(), n);
    }
    else
    {
        for(int i = 0; i < n; i++)
            buf[i] = 0xFF;
        sharedMsg->len = n;
        memcpy(sharedMsg->data, buf, n);
    }
    if(num == 0)
        ui->stateEdit->appendPlainText(QString("Write: OK"));
    else
        ui->stateEdit->appendPlainText(QString("Write: Failed: %1").arg(num));
    sem_post(newDataMutex);
    ui->stateEdit->appendPlainText("Mutex: V(newData)");
    QApplication::processEvents(); // update UI
    sem_post(busyMutex);
    ui->stateEdit->appendPlainText("Mutex: V(busy)");
    QApplication::processEvents(); // update UI
    sem_wait(receivedMutex);
    ui->stateEdit->appendPlainText("Mutex: P(received)");
    ui->stateEdit->appendPlainText("Write: got a ACK");
    QApplication::processEvents(); // update UI

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
    QByteArray fileName;
    QByteArray absFileName;
    fileName = ui->nameEdit->text().toLocal8Bit();
    absFileName = "/dev/mqueue" + fileName;
    if(access(absFileName, F_OK) == 0)
    {
        ui->stateEdit->appendPlainText(typeName + ": file already exists");
        if(remove(absFileName) == 0)
            ui->stateEdit->appendPlainText(typeName + ": file removed");
        else
            ui->stateEdit->appendPlainText(typeName + ": failed to remove file");
    }
    fileHandle = shm_open(fileName, O_CREAT | O_EXCL | O_RDWR, 0644);
    ui->stateEdit->appendPlainText(typeName + ": create: " + QString::number(fileHandle));
    if(fileHandle > 0)
    {
        ui->stateEdit->appendPlainText(typeName + ": created");
        ftruncate(fileHandle, 1024);
        sharedMsg = (mymsg*)mmap(nullptr, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fileHandle, 0);
    }
    else
    {
        ui->stateEdit->appendPlainText(typeName + ": failed to create");
        ui->stateEdit->appendPlainText(QString("Error: ") + strerror(errno));
    }
    system("rm /dev/shm/*" + typeName.toLocal8Bit() + "_*");
    busyMutex = mySemaphoreOpen("/" + typeName.toLocal8Bit() + "_busyMutex", 1);
    newDataMutex = mySemaphoreOpen("/" + typeName.toLocal8Bit() + "_newDataMutex", 0);
    receivedMutex = mySemaphoreOpen("/" + typeName.toLocal8Bit() + "_receivedMutex", 0);
}


void MainWindow::on_openButton_clicked()
{
    QByteArray fileName;
    fileName = ui->nameEdit->text().toLocal8Bit();
    fileHandle = shm_open(fileName, O_RDWR, 0);
    ui->stateEdit->appendPlainText(typeName + ": open: " + QString::number(fileHandle));
    if(fileHandle >= 0)
    {
        sharedMsg = (mymsg*)mmap(nullptr, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fileHandle, 0);
        ui->stateEdit->appendPlainText(typeName + ": opened");
    }
    else
    {
        ui->stateEdit->appendPlainText(typeName + ": failed to open");
    }
    busyMutex = mySemaphoreOpen("/" + typeName.toLocal8Bit() + "_busyMutex", 1);
    newDataMutex = mySemaphoreOpen("/" + typeName.toLocal8Bit() + "_newDataMutex", 0);
    receivedMutex = mySemaphoreOpen("/" + typeName.toLocal8Bit() + "_receivedMutex", 0);
}

sem_t* MainWindow::mySemaphoreOpen(const char* name, int val)
{
    sem_t* result;
    result = sem_open(name, O_CREAT | O_EXCL | O_RDWR, 0644, val);
    if(result == nullptr)
        result = sem_open(name, O_RDWR, 0644, val);
    return result;
}

void MainWindow::onMutexPButton_clicked()
{
    int val;
    sem_t* mutex = findMutexByWidget(sender());
    sem_getvalue(mutex, &val);
    if(val == 0)
    {
        ui->stateEdit->appendPlainText("Mutex: " + getMutexName(mutex) + " is 0! P(" + getMutexName(mutex) + ") will block!");
    }
    else
    {
        sem_wait(mutex);
        ui->stateEdit->appendPlainText("Mutex: P(" + getMutexName(mutex) + ")");
        QApplication::processEvents(); // update UI
        onMutexValButton_clicked(); // sender() from this slot function
    }
}


void MainWindow::onMutexVButton_clicked()
{
    int val;
    sem_t* mutex = findMutexByWidget(sender());
    sem_getvalue(mutex, &val);
    if(val == 1)
    {
        ui->stateEdit->appendPlainText("Mutex: " + getMutexName(mutex) + " is 1! " + getMutexName(mutex) + " is a mutex!");
    }
    else
    {
        sem_post(mutex);
        ui->stateEdit->appendPlainText("Mutex: V(" + getMutexName(mutex) + ")");
        QApplication::processEvents(); // update UI
        onMutexValButton_clicked(); // sender() from this slot function
    }
}


void MainWindow::onMutexValButton_clicked()
{
    int val;
    sem_t* mutex = findMutexByWidget(sender());
    sem_getvalue(mutex, &val);
    ui->stateEdit->appendPlainText(QString("Mutex: " + getMutexName(mutex) + ": %1").arg(val));
    QApplication::processEvents(); // update UI
}

sem_t* MainWindow::findMutexByWidget(QObject* widget)
{
    if(widget == ui->busy_PButton || widget == ui->busy_VButton || widget == ui->busy_valButton)
        return busyMutex;
    else if(widget == ui->newData_PButton || widget == ui->newData_VButton || widget == ui->newData_valButton)
        return newDataMutex;
    else if(widget == ui->received_PButton || widget == ui->received_VButton || widget == ui->received_valButton)
        return receivedMutex;
}

QString MainWindow::getMutexName(sem_t* mutex)
{
    if(mutex == busyMutex)
        return "busy";
    else if(mutex == newDataMutex)
        return "newData";
    else if(mutex == receivedMutex)
        return "received";
}
