#ifndef READFRAMESFROMUDP_H
#define READFRAMESFROMUDP_H

#include <QObject>
#include <QUdpSocket>
#include <QQueue>


class ReadFramesFromUDP : public QObject
{
    Q_OBJECT

public:
    explicit ReadFramesFromUDP(QObject *parent = nullptr);
    ~ReadFramesFromUDP();

    QByteArray frameUDPBuffer;
    QQueue<QByteArray> LFramesUDPBuffer;
    QQueue<QByteArray> HFramesUDPBuffer;
    QString byteToBinary(unsigned char byte);

signals:
    void newFrameAvailable();

private slots:
    void readyRead();

private:
    QUdpSocket *udpSocket = nullptr;
    void processData(const QByteArray &datagram);
    bool isHF = false;
};

#endif // READFRAMESFROMUDP_H
