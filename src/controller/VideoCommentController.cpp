#include "VideoCommentController.h"

#include <VideoCommentModel.h>


VideoCommentController::VideoCommentController(QObject *parent)
    : QObject{parent}
{
    videoCommentModel = new VideoCommentModel();
}

VideoCommentModel *VideoCommentController::model() const
{
    return videoCommentModel;
}

void VideoCommentController::getComments(const QString &videoId) const
{
    videoCommentModel->clear();
    videoCommentModel->getComments(videoId);
}

void VideoCommentController::uploadComment(const QString &userId, const QString &videoId, const QString &content)
{
    qDebug() << userId << ": " << videoId << ": " << content;
    videoCommentModel->uploadComment(userId, videoId, content);
}
