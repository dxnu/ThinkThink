#include "VideoCategoryController.h"




VideoCategoryController::VideoCategoryController(QObject *parent)
    : QObject{parent}
{
    videoCategoryModel = new VideoCategoryModel();
}

void VideoCategoryController::addCategory(const Category &categoryName)
{
    videoCategoryModel->addCategory(categoryName);
}

VideoCategoryModel* VideoCategoryController::model() const
{
    return videoCategoryModel;
}

bool VideoCategoryController::empty() const
{
    return videoCategoryModel->rowCount() == 0 ? true : false;
}

QString VideoCategoryController::at(int index) const
{
    return videoCategoryModel->at(index);
}

void VideoCategoryController::setVideoController(VideoController *controller) const
{
    videoCategoryModel->setVideoController(controller);
}

void VideoCategoryController::updateCategories()
{
    videoCategoryModel->clear();
    videoCategoryModel->updateCategories();
}

void VideoCategoryController::insertCategory(const QString &categoryName)
{
    videoCategoryModel->insertCategory(categoryName);
}

void VideoCategoryController::deleteCategory(const QString &category, int index)
{
    videoCategoryModel->deleteCategory(category, index);
}

void VideoCategoryController::updateCategoryCover(int coverNumber, int categoryIndex, const QString &coverPath)
{
    qDebug() << "update-- cover number:" << coverNumber << " categoryIndex:" << categoryIndex << " cover path: " << coverPath;
    videoCategoryModel->updateCategoryCover(coverNumber, categoryIndex, coverPath);
}

QString VideoCategoryController::getCoverSource(int index, int coverNumber)
{
    return videoCategoryModel->getCoverSource(index, coverNumber);
}
