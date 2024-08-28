#ifndef VIDEOCOMMENTMODEL_H
#define VIDEOCOMMENTMODEL_H

#include <QAbstractListModel>


class QNetworkAccessManager;
class QNetworkReply;


struct VideoComment {
    QString comment_id;
    QString video_id;
    QString account_id;
    QString content;
    QString user_name;
    QString user_avatar_source;
    QString time;
};


class VideoCommentModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum VideoCommentRoles {
        CommentIdRole = Qt::UserRole + 1,
        VideoIdRole,
        AccountIdRole,
        ContentRole,
        UserNameRole,
        UserAvatarSourceRole,
        TimeRole
    };

    explicit VideoCommentModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    void getComments(const QString& videoId) const;


    void clear();

    // 添加评论到Model中
    void addComment(const VideoComment& comment);

    void uploadComment(const QString& userId, const QString& videoId, const QString& content);


signals:
    void uploadVideoCommentsReply(bool status, const QString& errorMsg);

private:
    void OnGetComments(const QString& response);
    void OnUploadComments(const QString& response);

private slots:
    void managerFinished(QNetworkReply *reply);

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<VideoComment> m_comments;
    QNetworkAccessManager* m_manager;
};

#endif // VIDEOCOMMENTMODEL_H
