#ifndef FILEUPLOADER_H
#define FILEUPLOADER_H

#include <QObject>

class QNetworkReply;
class QFile;


class FileUploader : public QObject
{
public:
    FileUploader(QObject *parent = nullptr);

    void setFilePath(const QString& filePath);

    void uploadFile(const QString& uniqueFileName);

    QString generateUniqueFileName();

public slots:
    void httpUploadFileFinished();

private:
    QNetworkReply* uploadFileReply;
    QFile* file;
    QString m_filePath;
};

#endif // FILEUPLOADER_H
