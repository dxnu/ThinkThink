#ifndef FILEUPLOADCONTROLLER_H
#define FILEUPLOADCONTROLLER_H

#include <QObject>

class FileUploader;


class FileUploadController : public QObject
{
    Q_OBJECT
public:
    explicit FileUploadController(QObject *parent = nullptr);

    Q_INVOKABLE void setFilePath(const QString& filePath);

    Q_INVOKABLE void uploadFile(const QString& uniqueFileName);

    Q_INVOKABLE QString generateUniqueFileName();

private:
    FileUploader* fileUploader;
};

#endif // FILEUPLOADCONTROLLER_H
