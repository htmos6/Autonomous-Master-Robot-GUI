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
    void getFromPico();
    std::function<void(QString)> receive_cb = [](QString received){    qDebug() << "Received: ";
                                                                       qDebug() << received;    };
signals:

private:
    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QString pico_ip;
    int pico_port;

};

std::vector<std::string> split (const std::string &s, char delim);

#endif // PICOCONNECTION_H
