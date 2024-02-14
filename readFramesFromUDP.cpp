#include "readFramesFromUDP.h"
#include <QTime>
#include <QDebug>
#include <QByteArray>
#include <QBitArray>
#include <QQueue>
#include <QFile>
#include <QTextStream>


ReadFramesFromUDP::ReadFramesFromUDP(QObject *parent) :
    QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    bool result =  udpSocket->bind(QHostAddress::AnyIPv4, 8081);
    qDebug() << result;
    if(result)
    {
        qDebug() << "PASS";
    }
    else
    {
        qDebug() << "FAIL";
    }
    // processPendingDatagrams();
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

ReadFramesFromUDP::~ReadFramesFromUDP()
{
}

void ReadFramesFromUDP::readyRead() {
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        processData(datagram);
    }
}

void ReadFramesFromUDP::processData(const QByteArray &datagram) {
    int i = 0;

    while (i < datagram.size()) {
        QByteArray udpBuffer = datagram.mid(i, 5);
        // Extract bytes
        unsigned char byte0 = static_cast<unsigned char>(udpBuffer.at(0));
        unsigned char byte1 = static_cast<unsigned char>(udpBuffer.at(1));
        unsigned char byte2 = static_cast<unsigned char>(udpBuffer.at(2));
        unsigned char byte3 = static_cast<unsigned char>(udpBuffer.at(3));
        unsigned char byte4 = static_cast<unsigned char>(udpBuffer.at(4));

        // Convert to binary representation
        QString intensity = byteToBinary(byte3) + byteToBinary(byte4);
        QString coordinate = byteToBinary(byte0) + byteToBinary(byte1) + byteToBinary(byte2);
        QString intensity_trunc = intensity.left(16);
        QString coordinate_trunc = coordinate.mid(5, 19);

        // low frequency
        if (coordinate_trunc == "1101110111011101110") {
            if (!frameUDPBuffer.isEmpty()) {
                if (isHF) {
                    HFramesUDPBuffer.enqueue(frameUDPBuffer);
                } else if (!isHF) {
                    LFramesUDPBuffer.enqueue(frameUDPBuffer);
                }
                emit newFrameAvailable();
                frameUDPBuffer.clear();
            }
            isHF = false;
            qDebug() << "LFramesUDPBuffer length: " << LFramesUDPBuffer.length();
            qDebug() << "i is: " << i << " coordinate_trunc is :" << coordinate_trunc;
            qDebug() << QTime::currentTime();
        }

        // high frequency
        if (coordinate_trunc == "1111111111111111111") {
            if (!frameUDPBuffer.isEmpty()) {
                if (isHF) {
                    HFramesUDPBuffer.enqueue(frameUDPBuffer);
                } else if (!isHF) {
                    LFramesUDPBuffer.enqueue(frameUDPBuffer);
                }
                emit newFrameAvailable();
                frameUDPBuffer.clear();
            }
            isHF = true;
            qDebug() << "HFramesUDPBuffer length: " << LFramesUDPBuffer.length();
            qDebug() << "i is: " << i << " coordinate_trunc is :" << coordinate_trunc;
            qDebug() << QTime::currentTime();
        }


        frameUDPBuffer.append((coordinate_trunc + intensity_trunc).toLatin1());

        i += 5;
    }
}

QString ReadFramesFromUDP::byteToBinary(unsigned char byte) {
    QString binaryString;
    for (int i = 7; i >= 0; --i) {
        binaryString.append((byte & (1 << i)) ? '1' : '0');
    }
    return binaryString;
}
