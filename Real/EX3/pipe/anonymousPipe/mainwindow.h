#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

    void setId(int id);

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

private:
    Ui::MainWindow *ui;
    int id = -1;
    QTimer* runningTimer;

    int pipeR;
    int pipeW;
    static char buf[BUFLEN];

    void readPipe(int n);
    void writePipe(int n);
};
#endif // MAINWINDOW_H
