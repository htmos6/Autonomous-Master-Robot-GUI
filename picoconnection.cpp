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
    qDebug() << "Received: ";
    qDebug() << socketBuffer;
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
    std::string command_std = msg.toStdString();
    const char* p = command_std.c_str();
    QByteArray msg_b;
    msg_b.append(p);
    tcpSocket->write(msg_b);
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
