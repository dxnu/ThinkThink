#include "VideoModel.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTcpSocket>
#include <RequestMessage.h>
#include <QtAlgorithms>


VideoModel::VideoModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_manager = new QNetworkAccessManager();
    connect(m_manager, &QNetworkAccessManager::finished, this, &VideoModel::managerFinished);
}

void VideoModel::addVideo(const Video &video)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_videos << video;
    endInsertRows();
}

int VideoModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_videos.count();
}

QVariant VideoModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_videos.count())
        return QVariant();

    const Video& video = m_videos[index.row()];
    if(role == UploaderRole)
        return video.uploader;
    else if(role == UploadDateRole)
        return video.upload_date;
    else if(role == VideoNameRole)
        return video.video_name;
    else if(role == VideoIdRole)
        return video.video_id;
    else if(role == CoverImageRole)
        return video.cover_image;
    else if(role == SourceRole)
        return video.source;
    else if(role == DescribeRole)
        return video.describe;
    return QVariant();
}

void VideoModel::updateVideos(const QString &category)
{
    // 包含有中文，需要转码
    QString url = QString("http://127.0.0.1/get_videos?category=%1").arg(category);
    m_manager->get(QNetworkRequest(QUrl(url)));
}

void VideoModel::clear()
{
    QAbstractListModel::beginResetModel();
    m_videos.clear();
    QAbstractListModel::endResetModel();
}

void VideoModel::uploadVideo(const QString &title, const QString &uploader, const QString &coverImage, const QString &source, const QString &category, const QString &describe)
{
    QString url = QString("http://127.0.0.1/upload_video?title=%1&uploader=%2&cover=%3&category=%4&describe=%5&source=%6")
            .arg(title).arg(uploader).arg(coverImage).arg(category).arg(describe).arg(source);
    m_manager->get(QNetworkRequest(QUrl(url)));
}

void VideoModel::searchVideos(const QString &keyword)
{
    QString url = QString("http://127.0.0.1/search_videos?keyword=%1").arg(keyword);
    m_manager->get(QNetworkRequest(QUrl(url)));
}

void VideoModel::getMyVideos(const QString &keyword)
{
    QString url = QString("http://127.0.0.1/get_my_videos?keyword=%1").arg(keyword);
    qDebug() << "get my videos: " << url;
    m_manager->get(QNetworkRequest(QUrl(url)));
}

void VideoModel::deleteVideo(const QString &videoId, int index)
{
    QString url = QString("http://127.0.0.1/delete_video?video_id=%1").arg(videoId);
    m_manager->get(QNetworkRequest(QUrl(url)));

    // 更新model
    QAbstractListModel::beginResetModel();
    m_videos.removeAt(index);
    QAbstractListModel::endResetModel();
}

void VideoModel::managerFinished(QNetworkReply *reply)
{
    QString response = QString::fromLocal8Bit(reply->readAll());

    QXmlStreamReader xml(response);
    if(xml.readNextStartElement())
    {
        if(xml.name().toString() == "ThinkThink") // 确保是ThinkThink消息
        {
            while(xml.readNextStartElement())
            {
                if(xml.name().toString() == "ResponseType")
                {
                    QString value = xml.readElementText();
                    qDebug() << "response type" << value;
                    if(value == "GetVideos") OnGetVideos(response);
                    else if(value == "UploadVideo") OnUploadVideo(response);
                }
                else
                {
                    xml.skipCurrentElement();
                }
            }
        }
        else
        {
            xml.raiseError(QObject::tr("invalid response message"));
        }
    }
}

void VideoModel::OnGetVideos(const QString &response)
{
    QXmlStreamReader xml(response);
    if(xml.readNextStartElement())
    {
        if(xml.name().toString() == "ThinkThink") // 确保是ThinkThink消息
        {
            while(xml.readNextStartElement())
            {
                if(xml.name().toString() == "Video")
                {
                    Video video;
                    QString value;
                    while(xml.readNextStartElement())
                    {
                        if(xml.name().toString() == "Id")
                        {
                            value = xml.readElementText();
                            video.video_id = value;
                        }
                        else if(xml.name().toString() == "Name")
                        {
                            value = xml.readElementText();
                            video.video_name = value;
                        }
                        else if(xml.name().toString() == "Uploader")
                        {
                            value = xml.readElementText();
                            video.uploader = value;
                        }
                        else if(xml.name().toString() == "CoverImage")
                        {
                            value = xml.readElementText();
                            video.cover_image = "http://127.0.0.1/" + value;
                            qDebug() << "cover iamge: " << video.cover_image << '\n';
                        }
                        else if(xml.name().toString() == "Describe")
                        {
                            value = xml.readElementText();
                            video.describe = value;
                        }
                        else if(xml.name().toString() == "UploadDate")
                        {
                            value = xml.readElementText();
                            video.upload_date = value;
                        }
                        else if(xml.name().toString() == "Source")
                        {
                            value = xml.readElementText();
                            video.source = "http://127.0.0.1/" + value;
                            qDebug() << "video source: " << video.source << '\n';
                        }
                        else
                        {
                            xml.skipCurrentElement();
                        }
                    }
                    addVideo(video);

                }
                else
                {
                    xml.skipCurrentElement();
                }
            }
        }
        else
        {
            xml.raiseError(QObject::tr("invalid response message"));
        }
    }
}

void VideoModel::OnUploadVideo(const QString &response)
{
    qDebug() << "upload video reply: " << response;

    bool status{false};
    QString errorMsg;

    QXmlStreamReader xml(response);
    if(xml.readNextStartElement())
    {
        if(xml.name().toString() == "ThinkThink") // 确保是ThinkThink消息
        {
            while(xml.readNextStartElement())
            {
                if(xml.name().toString() == "Status")
                {
                    QString value = xml.readElementText();
                    if(value == "OK") status = true;
                    else status = false;
                    qDebug() << "status: " << value;
                }
                else if(xml.name().toString() == "ErrorMsg")
                {
                    errorMsg = xml.readElementText();
                    qDebug() << "lastError: " << errorMsg;
                }
                else
                {
                    xml.skipCurrentElement();
                }
            }
        }
        else
        {
            xml.raiseError(QObject::tr("invalid response message"));
        }
    }

    emit uploadVideoReply(status, errorMsg);
}

QHash<int, QByteArray> VideoModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[VideoIdRole] = "video_id";
    roles[VideoNameRole] = "video_name";
    roles[UploaderRole] = "uploader";
    roles[CoverImageRole] = "cover_image";
    roles[SourceRole] = "source";
    roles[UploadDateRole] = "upload_date";
    roles[DescribeRole] = "describe";
    return roles;
}


