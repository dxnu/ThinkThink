#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H

#include <QObject>

class Video;
class VideoModel;
class VideoCategoryController;


class VideoController : public QObject
{
    Q_OBJECT
public:
    explicit VideoController(QObject *parent = nullptr);

    void addVideo(const Video& video);

    Q_INVOKABLE VideoModel* model() const;

    Q_INVOKABLE void updateVideos(const QString& category);

    Q_INVOKABLE void uploadVideo(const QString& title, const QString& uploader, const QString& coverImage, const QString& source, const QString& category, const QString& describe);

    Q_INVOKABLE void searchVideos(const QString& keyword);

    Q_INVOKABLE void getMyVideos(const QString& keyword);

    Q_INVOKABLE void deleteVideo(const QString& videoId, int index);

private:
    VideoModel* videoModel;

};

#endif // VIDEOCONTROLLER_H
