#include "picoconnection.h"

PicoConnection::PicoConnection(QObject *parent)
    : QObject{parent}
{
    tcpSocket = new QTcpSocket(this);

    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    QObject::connect(tcpSocket, &QAbstractSocket::disconnected, this, &PicoConnection::disconnected);
    QObject::connect(tcpSocket, &QIODevice::readyRead, this, &PicoConnection::getFromPico);
}

void PicoConnection::getFromPico(){
    QByteArray socketBuffer = tcpSocket->readAll();
    receive_cb(QString(socketBuffer));
}

bool PicoConnection::connect(QString ip, int port){
    tcpSocket->connectToHost(ip, port, QIODevice::ReadWrite);

    if(tcpSocket->waitForConnected()){
        pico_ip = ip;
        pico_port = port;
        return true;
    }

    return false;
}

void PicoConnection::send(QString msg){
    if(!tcpSocket->isOpen()){
        qDebug() << "No connection to pico!" << Qt::endl;
        return;
    }
    std::string command_std = msg.toStdString();
    const char* p = command_std.c_str();
    QByteArray msg_b;
    msg_b.append(p);
    try {
        tcpSocket->write(msg_b);
    } catch (bool err) {
        qDebug() << "No connection to pico!" << Qt::endl;
    }

}

void PicoConnection::disconnect(){
    tcpSocket->close();
}

void PicoConnection::disconnected(){
    tcpSocket->close();
    for(int i = 0; i < 20; i++){
        QTimer::singleShot(1000, [this]() { connect(pico_ip, pico_port); } );
    }
}

PicoConnection::~PicoConnection(){
    tcpSocket->close();
    delete tcpSocket;
    qDebug() << "Socket closed";
}

std::vector<std::string> split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}
