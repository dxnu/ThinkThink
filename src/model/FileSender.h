#ifndef FILESENDER_H
#define FILESENDER_H

#include <QObject>

class QTcpSocket;


class FileSender : public QObject
{
    Q_OBJECT
public:
    explicit FileSender(QObject *parent = nullptr);

    void sendFile(const QString& path);

private slots:
    void conntedRequest();
    void closeSocket();

private:
    QTcpSocket* m_socket;
};

#endif // FILESENDER_H
