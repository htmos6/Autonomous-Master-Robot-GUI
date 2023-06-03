#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawwidget.h"
#include <QColorDialog>
#include <QLayout>


class MainWindowPrivate {
public:
    MainWindowPrivate(MainWindow *owner);
    ~MainWindowPrivate();

    void pickColor();
    void clear();
    void printPoints();
    void connect();
    void drawPath();
    void sel1();
    void sel2();
    void sel3();
    void sel4();
    void sel5();
    void sel6();

private slots:
    void on_actionPrint_triggered();
    void on_actionConnect_triggered();
    void on_actionSend_triggered();

private:
    DrawWidget *m_drawWidget;
    MainWindow *m_Owner;
    Ui::MainWindow *m_ui;
};

MainWindowPrivate::MainWindowPrivate(MainWindow *owner) :
    m_drawWidget(new DrawWidget(owner)),
    m_Owner(owner),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(m_Owner);
    m_ui->centralWidget->setLayout(new QVBoxLayout(m_Owner));
    m_ui->centralWidget->layout()->addWidget(m_drawWidget);
}

void MainWindowPrivate::pickColor()
{
    QColor color = QColorDialog::getColor(
                m_drawWidget->drawColor(),
                m_Owner,
                QString("Select a draw color"),
                QColorDialog::ShowAlphaChannel
                );
    m_drawWidget->setDrawColor(color);
}

void MainWindowPrivate::clear()
{
    m_drawWidget->clear();
}

void MainWindowPrivate::printPoints()
{
    m_drawWidget->printPoints();
}

void MainWindowPrivate::connect()
{
    m_drawWidget->connectToPico();
}

void MainWindowPrivate::sel1()
{
    m_drawWidget->drawPath(1);
}

void MainWindowPrivate::sel2()
{
    m_drawWidget->drawPath(2);
}

void MainWindowPrivate::sel3()
{
    m_drawWidget->drawPath(3);
}

void MainWindowPrivate::sel4()
{
    m_drawWidget->drawPath(4);
}

void MainWindowPrivate::sel5()
{
    m_drawWidget->drawPath(5);
}

void MainWindowPrivate::sel6()
{
    m_drawWidget->drawPath(6);
}

MainWindowPrivate::~MainWindowPrivate(){
    delete m_ui;
}

void MainWindowPrivate::on_actionConnect_triggered(){
    ;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    d(new MainWindowPrivate(this))
{
}

MainWindow::~MainWindow()
{
    delete d;
}

void MainWindow::colorPickTriggered()
{
    d->pickColor();
}

void MainWindow::on_actionConnect_triggered()
{
    d->connect();
}

void MainWindow::on_actionSend_triggered()
{
    d->printPoints();
}

void MainWindow::sel1()
{
    d->sel1();
}

void MainWindow::sel2()
{
    d->sel2();
}

void MainWindow::sel3()
{
    d->sel3();
}

void MainWindow::sel4()
{
    d->sel4();
}

void MainWindow::sel5()
{
    d->sel5();
}

void MainWindow::sel6()
{
    d->sel6();
}

void MainWindow::clear()
{
    d->clear();
}

void MainWindow::printPoints(){
    d->printPoints();
}
