#include "VideoCommentModel.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>


VideoCommentModel::VideoCommentModel(QObject *parent)
    : QAbstractListModel {parent}
{
    m_manager = new QNetworkAccessManager();
    connect(m_manager, &QNetworkAccessManager::finished, this, &VideoCommentModel::managerFinished);
}

int VideoCommentModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_comments.count();
}

QVariant VideoCommentModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_comments.count())
        return QVariant();

    const VideoComment& comment = m_comments[index.row()];
    if(role == CommentIdRole)
        return comment.comment_id;
    else if(role == VideoIdRole)
        return comment.video_id;
    else if(role == AccountIdRole)
        return comment.account_id;
    else if(role == ContentRole)
        return comment.content;
    else if(role == UserNameRole)
        return comment.user_name;
    else if(role == UserAvatarSourceRole)
        return comment.user_avatar_source;
    else if(role == TimeRole)
        return comment.time;

    return QVariant();
}

void VideoCommentModel::getComments(const QString &videoId) const
{
    QString url = QString("http://127.0.0.1/get_comments?video_id=%1").arg(videoId);
    m_manager->get(QNetworkRequest(QUrl(url)));
}

void VideoCommentModel::clear()
{
    QAbstractListModel::beginResetModel();
    m_comments.clear();
    QAbstractListModel::endResetModel();
}

void VideoCommentModel::addComment(const VideoComment &comment)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_comments << comment;
    endInsertRows();
}

void VideoCommentModel::uploadComment(const QString &userId, const QString &videoId, const QString &content)
{
    QString url = QString("http://127.0.0.1/upload_comments?user_id=%1&video_id=%2&content=%3").arg(userId).arg(videoId).arg(content);
    m_manager->get(QNetworkRequest(QUrl(url)));
}

void VideoCommentModel::OnGetComments(const QString &response)
{
    qDebug() << "on get comments";

    // 处理评论消息
    QXmlStreamReader xml(response);
    if(xml.readNextStartElement())
    {
        if(xml.name().toString() == "ThinkThink") // 确保是ThinkThink消息
        {
            while(xml.readNextStartElement())
            {
                if(xml.name().toString() == "VideoComments")
                {
                    VideoComment comment;
                    QString value;
                    while(xml.readNextStartElement())
                    {
                        if(xml.name().toString() == "CommentId")
                        {
                            value = xml.readElementText();
                            comment.comment_id = value;
                        }
                        else if(xml.name().toString() == "VideoId")
                        {
                            value = xml.readElementText();
                            comment.video_id = value;
                        }
                        else if(xml.name().toString() == "AccountId")
                        {
                            value = xml.readElementText();
                            comment.account_id = value;
                        }
                        else if(xml.name().toString() == "UserAvatarSource")
                        {
                            value = xml.readElementText();
                            comment.user_avatar_source = "http://127.0.0.1/" + value;
                        }
                        else if(xml.name().toString() == "Content")
                        {
                            value = xml.readElementText();
                            comment.content = value;
                        }
                        else if(xml.name().toString() == "UserName")
                        {
                            value = xml.readElementText();
                            comment.user_name = value;
                        }
                        else if(xml.name().toString() == "Time")
                        {
                            value = xml.readElementText();
                            comment.time = value;
                        }
                        else
                        {
                            xml.skipCurrentElement();
                        }
                    }
                    qDebug() << "添加Comment";
                    addComment(comment);

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

void VideoCommentModel::OnUploadComments(const QString &response)
{
    qDebug() << "upload comments reply: " << response;

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

    emit uploadVideoCommentsReply(status, errorMsg);
}

void VideoCommentModel::managerFinished(QNetworkReply *reply)
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
                    if(value == "GetComments") OnGetComments(response);
                    else if(value == "UploadComments") OnUploadComments(response);
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

QHash<int, QByteArray> VideoCommentModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CommentIdRole] = "comment_id";
    roles[VideoIdRole] = "video_id";
    roles[AccountIdRole] = "account_id";
    roles[ContentRole] = "content";
    roles[UserNameRole] = "user_name";
    roles[UserAvatarSourceRole] = "user_avatar_source";
    roles[TimeRole] = "time";

    return roles;
}
