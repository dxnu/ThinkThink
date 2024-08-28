#ifndef VIDEOCATEGORYCONTROLLER_H
#define VIDEOCATEGORYCONTROLLER_H

#include <QObject>
#include <VideoCategoryModel.h>

class VideoCategoryModel;
class VideoController;


class VideoCategoryController : public QObject
{
    Q_OBJECT

public:
    explicit VideoCategoryController(QObject *parent = nullptr);
    void addCategory(const Category& categoryName);
    Q_INVOKABLE VideoCategoryModel* model() const;

    bool empty() const;

    Q_INVOKABLE QString at(int index) const;

    void setVideoController(VideoController* controller) const;

    Q_INVOKABLE void updateCategories();

    Q_INVOKABLE void insertCategory(const QString& categoryName); // 往数据库中添加

    // 删除目录
    Q_INVOKABLE void deleteCategory(const QString& category, int index);

    Q_INVOKABLE void updateCategoryCover(int coverNumber, int categoryIndex, const QString& coverPath);

    Q_INVOKABLE QString getCoverSource(int index, int coverNumber);

//signals:
//    void modelChanged();

private:
    VideoCategoryModel* videoCategoryModel;
};

#endif // VIDEOCATEGORYCONTROLLER_H
