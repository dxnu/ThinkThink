#ifndef VIDEOCOMMENTCONTROLLER_H
#define VIDEOCOMMENTCONTROLLER_H

#include <QObject>
#include <VideoCommentModel.h>


class VideoCommentController : public QObject
{
    Q_OBJECT
public:
    explicit VideoCommentController(QObject *parent = nullptr);

    Q_INVOKABLE VideoCommentModel* model() const;

    Q_INVOKABLE void getComments(const QString& videoId) const;

    Q_INVOKABLE void uploadComment(const QString& userId, const QString& videoId, const QString& content);

signals:

private:
    VideoCommentModel* videoCommentModel;

};

#endif // VIDEOCOMMENTCONTROLLER_H
