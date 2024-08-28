#include "FileUploadController.h"
#include "FileUploader.h"
#include <QDir>


FileUploadController::FileUploadController(QObject *parent)
    : QObject{parent}
{
    fileUploader = new FileUploader();
}

void FileUploadController::setFilePath(const QString &filePath)
{
    QString nativePath = QDir::toNativeSeparators(filePath);
    qDebug() << nativePath;
    fileUploader->setFilePath(nativePath);
}

void FileUploadController::uploadFile(const QString &uniqueFileName)
{
    fileUploader->uploadFile(uniqueFileName);
}

QString FileUploadController::generateUniqueFileName()
{
    return fileUploader->generateUniqueFileName();
}
