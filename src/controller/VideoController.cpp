#include "VideoController.h"
#include "VideoModel.h"


VideoController::VideoController(QObject *parent)
    : QObject{parent}
{
    videoModel = new VideoModel();
}

void VideoController::addVideo(const Video &video)
{
    videoModel->addVideo(video);
}

VideoModel *VideoController::model() const
{
    return videoModel;
}

void VideoController::updateVideos(const QString &category)
{
    videoModel->clear();
    videoModel->updateVideos(category);
}

void VideoController::uploadVideo(const QString &title, const QString &uploader, const QString &coverImage, const QString &source, const QString &category, const QString &describe)
{
    videoModel->uploadVideo(title, uploader, coverImage, source, category, describe);
}

void VideoController::searchVideos(const QString &keyword)
{
    videoModel->clear();
    videoModel->searchVideos(keyword);
}

void VideoController::getMyVideos(const QString &keyword)
{
    videoModel->clear();
    videoModel->getMyVideos(keyword);
}

void VideoController::deleteVideo(const QString &videoId, int index)
{
    videoModel->deleteVideo(videoId, index);
}
