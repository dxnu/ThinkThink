#include "FileUploader.h"

#include <QEventLoop>
#include <QFileInfo>
#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <string>
#include <chrono>


FileUploader::FileUploader(QObject *parent)
    : QObject{parent}
{
}

void FileUploader::setFilePath(const QString &filePath)
{
    m_filePath = filePath;
}

void FileUploader::uploadFile(const QString& uniqueFileName)
{
    QFileInfo info(m_filePath);
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart httpPart;

    qDebug() << "file size: " << info.size() << " file suffix: " << info.suffix() << " file name: " << info.fileName();
    httpPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/" + info.suffix()));
    QString str = QString("form-data; name=\"file\"; filename=\"%1\"").arg(uniqueFileName);
    qDebug() << "str: " << str;
    httpPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(str));

    file = new QFile(m_filePath);
    file->open(QIODevice::ReadOnly);

    httpPart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(httpPart);

    QUrl url("http://127.0.0.1/upload_multipart");
    QNetworkRequest request(url);
    QNetworkAccessManager m_httpNAM;
    uploadFileReply = m_httpNAM.post(request, multiPart);
    multiPart->setParent(uploadFileReply);

    connect(uploadFileReply, &QNetworkReply::finished, this, &FileUploader::httpUploadFileFinished);
    QEventLoop eventLoop;
    connect(&m_httpNAM, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
}

QString FileUploader::generateUniqueFileName()
{
    QFileInfo info(m_filePath);
    auto tp = std::chrono::high_resolution_clock::now();
    auto nano = tp.time_since_epoch().count();

    QString newFileName(std::to_string(nano).c_str());
    newFileName.append(".");
    newFileName.append(info.suffix());

    return newFileName;
}

void FileUploader::httpUploadFileFinished()
{
    if(uploadFileReply->error() == QNetworkReply::NoError)
    {
        qDebug() << "upload file finished";
        qDebug() << "read string: " << uploadFileReply->readAll();
        file->flush();
        file->close();
        uploadFileReply->deleteLater();
        uploadFileReply = nullptr;
        delete file;
        file = nullptr;
    }
    else
    {
        qDebug() << "error string: " << uploadFileReply->errorString();
        uploadFileReply->deleteLater();
    }
}























