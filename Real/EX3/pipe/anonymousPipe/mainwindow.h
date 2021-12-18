#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
private:
    Ui::MainWindow *ui;
    int id = -1;
    QTimer* runningTimer;

    int pipeR;
    int pipeW;
    char buf[8192];

    void readPipe(int n);
    void writePipe(int n);
};
#endif // MAINWINDOW_H
