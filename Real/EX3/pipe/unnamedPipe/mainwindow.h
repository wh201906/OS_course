#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <semaphore.h>

#include <QMainWindow>

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

    void setId(int id, int childNum);

    void appendText(const QString &text);
    void setPipeHandle(int r, int w);
protected:
    void showEvent(QShowEvent* e) override;
private slots:
    void onReadButtonClicked();
    void onWriteButtonClicked();
    void onTimeout();
    void on_clearDataButton_clicked();

    void on_clearStateButton_clicked();

    void on_childMutexBox_clicked(bool checked);

    void on_pipeWMutexBox_clicked(bool checked);

    void on_pipeW_PButton_clicked();

    void on_pipeW_VButton_clicked();

    void on_pipeW_valButton_clicked();

private:
    Ui::MainWindow *ui;
    int id = -1;
    int childNum;
    QTimer* runningTimer;

    int pipeR;
    int pipeW;
    static char buf[BUFLEN];

    bool allChildrenWrittenMutexOn = false;
    bool pipeWritingMutexOn = false;
    std::vector<sem_t*> childMutexList;
    sem_t* pipeWritingMutex;

    void readPipe(int n);
    void writePipe(int n);
    sem_t *mySemaphoreOpen(const char *name, int val);
};
#endif // MAINWINDOW_H
