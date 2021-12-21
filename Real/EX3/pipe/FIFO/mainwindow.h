#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFile>

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

    void on_createFIFOButton_clicked();
    void on_openFIFOButton_clicked();
private:
    Ui::MainWindow *ui;
    QTimer* runningTimer;

    static char buf[BUFLEN];
    int fifoHandle;
    char fifoPath[32] = "/tmp/FIFOTest";
    QFile::OpenModeFlag mode = QFile::NotOpen;

    void readPipe(int n);
    void writePipe(int n);
};
#endif // MAINWINDOW_H
