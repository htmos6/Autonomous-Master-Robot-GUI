#ifndef PICOCONNECTION_H
#define PICOCONNECTION_H

#include <QObject>

#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
class QTcpSocket;
QT_END_NAMESPACE

class PicoConnection : public QObject
{
    Q_OBJECT
public:
    explicit PicoConnection(QObject *parent = nullptr);
    ~PicoConnection();
    bool connect(QString ip, int port);
    void send(QString msg);
    void disconnect();
    void disconnected();
signals:

private:
    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QString pico_ip;
    int pico_port;

};

#endif // PICOCONNECTION_H
