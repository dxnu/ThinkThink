#include "FileSender.h"
#include <QTcpSocket>
#include <QFile>
#include <QFileInfo>
#include <RequestMessage.h>

FileSender::FileSender(QObject* parent)
    : QObject(parent)
{
    // 处理socket
    m_socket = new QTcpSocket();
    connect(m_socket, &QTcpSocket::connected, this, &FileSender::conntedRequest);
    connect(m_socket, &QTcpSocket::disconnected, this, &FileSender::closeSocket);
    m_socket->connectToHost(QHostAddress::LocalHost, 3006); // 连接到文件服务器
}

void FileSender::sendFile(const QString& path)
{
    QFile file(path);
    QFileInfo info(path);
    int fileSize = info.size();

    file.open(QFile::ReadOnly);
    if(!file.isOpen()) {
        qDebug() << "cannot open file " << path;
        return;
    }

    int num = 0;
    while(!file.atEnd()) {
        if(num == 0) {
            QString header("SOFSOL" + QString::number(fileSize) + "EOLSOT" + info.suffix() + "EOTEOF");
            m_socket->write(header.toStdString().c_str());
        }

        QByteArray line = file.readLine();
        num += line.size();
        int percent = (num * 100 / fileSize);
        // here can emit the percent to other thread
        qDebug() << "current percent: " << percent << "% ";
        qDebug() << "current count: " << num << " total:" << fileSize << "\n";

        m_socket->write(line);
    }

    file.close();
}

void FileSender::conntedRequest()
{
    qDebug() << "connected to file server\n";
}

void FileSender::closeSocket()
{
    m_socket->close();
}
