#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QSpinBox>

#include <QDataStream>
#include <QDialog>
#include <QtNetwork/QTcpSocket>


QT_BEGIN_NAMESPACE
namespace Ui 
{
    class MainWindow;
}
class QTcpSocket;
QT_END_NAMESPACE


class MainWindowPrivate;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void colorPickTriggered();
    void clear();
    void printPoints();
    void on_actionSend_triggered();
    void sel1();
    void sel2();
    void sel3();
    void sel4();
    void sel5();
    void sel6();
    void on_actionConnect_triggered();

private:
    MainWindowPrivate *d;
};

#endif // MAINWINDOW_H
