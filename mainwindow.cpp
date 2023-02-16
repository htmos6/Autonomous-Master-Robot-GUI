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

private slots:
    void on_actionPrint_triggered();

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

MainWindowPrivate::~MainWindowPrivate(){
    delete m_ui;
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

void MainWindow::clear()
{
    d->clear();
}

void MainWindow::printPoints(){
    d->printPoints();
}
