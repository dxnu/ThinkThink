#ifndef VIDEOMODEL_H
#define VIDEOMODEL_H

#include <QAbstractListModel>
#include <QStringList>


class QNetworkAccessManager;
class QNetworkReply;


struct Video {
    QString video_id;
    QString video_name;
    QString uploader;
    QString cover_image;
    QString source;
    QString upload_date;
    QString describe;
};


class VideoModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum VideoRoles {
        VideoNameRole = Qt::UserRole + 1,
        UploaderRole,
        UploadDateRole,
        CoverImageRole,
        DescribeRole,
        VideoIdRole,
        SourceRole
    };
    explicit VideoModel(QObject *parent = nullptr);

    // 添加视频到Model中
    void addVideo(const Video& video);

    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    // 从服务器获取视频列表
    void updateVideos(const QString& category);

    void clear();

    // 上传视频
    void uploadVideo(const QString& title, const QString& uploader, const QString& coverImage, const QString& source, const QString& category, const QString& describe);

    void searchVideos(const QString& keyword);

    void getMyVideos(const QString& keyword);

    // 删除视频
    void deleteVideo(const QString& videoId, int index);

signals:
    void uploadVideoReply(bool status, const QString& errorMsg);

private slots:
    void managerFinished(QNetworkReply *reply);

private:
    void OnGetVideos(const QString& response);
    void OnUploadVideo(const QString& response);


protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<Video> m_videos;
    QNetworkAccessManager* m_manager;
};

#endif // VIDEOMODEL_H
