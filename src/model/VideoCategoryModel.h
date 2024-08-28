#ifndef VIDEOCATEGORYMODEL_H
#define VIDEOCATEGORYMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QTcpSocket>

class VideoController;
class QNetworkAccessManager;
class QNetworkReply;

struct Category {
    QString name;
    QString cover1_source;
    QString cover2_source;
    QString cover3_source;
};

class VideoCategoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum VideoCategoryRoles {
        NameRole = Qt::UserRole + 1,
        Cover1SourceRole,
        Cover2SourceRole,
        Cover3SourceRole
    };
    VideoCategoryModel(QObject *parent = nullptr);

    void addCategory(const Category& category); // 往界面上添加

    void insertCategory(const QString& categoryName); // 往数据库中添加

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QString at(int index) const;

    void clear();

    void updateCategories();

    void setVideoController(VideoController* controller);

    // 删除目录
    void deleteCategory(const QString& category, int index);

    void updateCategoryCover(int coverNumber, int categoryIndex, const QString& coverPath);

    QString getCoverSource(int index, int coverNumber);

private:
    void OnGetVideoCategories(const QString& response);


protected:
    QHash<int, QByteArray> roleNames() const;

signals:
    void categoryChanged(const QString& cover1, const QString& cover2, const QString& cover3);

private slots:
    void updateVideos();

    ///////////////////
    void managerFinished(QNetworkReply* reply);

private:
    QList<Category> m_categories;
    VideoController* m_videoController;
    QNetworkAccessManager* m_manager;
};

#endif // VIDEOCATEGORYMODEL_H
