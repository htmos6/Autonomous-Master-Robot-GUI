#include "mainwindow.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon(":/kt007.ico"));
    w.setWindowTitle("Shinobi Technology KT-007");
    w.show();

    return a.exec();
}
