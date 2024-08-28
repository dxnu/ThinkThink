#include "FileSender.h"
#include "RequestMessage.h"
#include "VideoCategoryModel.h"
#include "VideoController.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QNetworkAccessManager>
#include <QNetworkReply>



VideoCategoryModel::VideoCategoryModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_manager = new QNetworkAccessManager();
    connect(m_manager, &QNetworkAccessManager::finished, this, &VideoCategoryModel::managerFinished);
    connect(this, &VideoCategoryModel::categoryChanged, this, &VideoCategoryModel::updateVideos);

    m_manager->get(QNetworkRequest(QUrl("http://127.0.0.1/get_video_categories")));
}

void VideoCategoryModel::addCategory(const Category &category)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_categories << category;
    endInsertRows();
}

void VideoCategoryModel::insertCategory(const QString &categoryName)
{
    // 包含有中文，需要转码
    QString url = QString("http://127.0.0.1/insert_category?category=%1").arg(categoryName);
    m_manager->get(QNetworkRequest(QUrl(url)));

    Category category;
    category.name = categoryName;
    addCategory(category);
}

int VideoCategoryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_categories.count();
}

QVariant VideoCategoryModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_categories.count())
        return QVariant();

    const Category& category = m_categories[index.row()];
    if(role == NameRole)
        return category.name;
    else if(role == Cover1SourceRole)
        return category.cover1_source;
    else if(role == Cover2SourceRole)
        return category.cover2_source;
    else if(role == Cover3SourceRole)
        return category.cover3_source;

    return QVariant();
}

QString VideoCategoryModel::at(int index) const
{
    return m_categories.at(index).name; // 只返回目录名
}

void VideoCategoryModel::clear()
{
    QAbstractListModel::beginResetModel();
    m_categories.clear();
    QAbstractListModel::endResetModel();
}

void VideoCategoryModel::updateCategories()
{
    m_manager->get(QNetworkRequest(QUrl("http://127.0.0.1/get_video_categories")));
}

void VideoCategoryModel::setVideoController(VideoController *controller)
{
    m_videoController = controller;
}

void VideoCategoryModel::deleteCategory(const QString &category, int index)
{
    QString url = QString("http://127.0.0.1/delete_category?category=%1").arg(category);
    m_manager->get(QNetworkRequest(QUrl(url)));

    // 更新model
    QAbstractListModel::beginResetModel();
    m_categories.removeAt(index);
    QAbstractListModel::endResetModel();
}

void VideoCategoryModel::updateCategoryCover(int coverNumber, int categoryIndex, const QString &coverPath)
{
    auto& category = m_categories[categoryIndex];
    QString url = QString("http://127.0.0.1/update_category_cover?category=%1&cover=%2&number=%3").arg(category.name).arg(coverPath).arg(coverNumber);
    m_manager->get(QNetworkRequest(QUrl(url)));

    if(coverNumber == 1) {
        category.cover1_source = "http://127.0.0.1/video/" + coverPath;
    } else if(coverNumber == 2) {
        category.cover2_source = "http://127.0.0.1/video/" + coverPath;
    } else if(coverNumber == 3) {
        category.cover3_source = "http://127.0.0.1/video/" + coverPath;
    }
}

QString VideoCategoryModel::getCoverSource(int index, int coverNumber)
{
    qDebug() << "cover source index:" << index;

    auto category = m_categories[index];
    if(coverNumber == 1) {
        return category.cover1_source;
    } else if(coverNumber == 2) {
        return category.cover2_source;
    } else

     return category.cover3_source;
}


void VideoCategoryModel::OnGetVideoCategories(const QString &response)
{
    QXmlStreamReader xml(response);
    if(xml.readNextStartElement())
    {
        if(xml.name().toString() == "ThinkThink") // 确保是ThinkThink消息
        {
            while(xml.readNextStartElement())
            {
                if(xml.name().toString() == "Category")
                {
                    Category category;
                    QString value;
                    while(xml.readNextStartElement())
                    {
                        if(xml.name().toString() == "Name")
                        {
                            value = xml.readElementText();
                            category.name = value;
                        }
                        else if(xml.name().toString() == "Cover1Source")
                        {
                            value = xml.readElementText();
                            category.cover1_source = "http://127.0.0.1/" + value;
                        }
                        else if(xml.name().toString() == "Cover2Source")
                        {
                            value = xml.readElementText();
                            category.cover2_source = "http://127.0.0.1/" + value;
                        }
                        else if(xml.name().toString() == "Cover3Source")
                        {
                            value = xml.readElementText();
                            category.cover3_source = "http://127.0.0.1/" + value;
                        }
                        else
                        {
                            xml.skipCurrentElement();
                        }
                    }
                    addCategory(category);

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

    // 更新视频
    if(m_categories.count() >= 0)
        emit categoryChanged(m_categories[0].cover1_source, m_categories[0].cover2_source, m_categories[0].cover3_source);
}

QHash<int, QByteArray> VideoCategoryModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[Cover1SourceRole] = "cover1_source";
    roles[Cover2SourceRole] = "cover2_source";
    roles[Cover3SourceRole] = "cover3_source";
    return roles;
}

void VideoCategoryModel::updateVideos()
{
    QString category = m_categories.at(0).name;
    m_videoController->updateVideos(category);
}

void VideoCategoryModel::managerFinished(QNetworkReply *reply)
{
    if(reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

//    QString data = QString::fromLocal8Bit(reply->readAll());
//    qDebug() << "reply:" << data;

//    QXmlStreamReader xml(data);
//    if(xml.readNextStartElement())
//    {
//        if(xml.name().toString() == "ThinkThink") // 确保是ThinkThink消息
//        {
//            while(xml.readNextStartElement())
//            {
//                if(xml.name().toString() == "Category")
//                {
//                    QString value = xml.readElementText();
//                    // qDebug() << value;
//                    addCategory(value);
//                }
//                else
//                {
//                    xml.skipCurrentElement();
//                }
//            }
//        }
//        else
//        {
//            xml.raiseError(QObject::tr("invalid response message"));
//        }
//    }

//    // 更新视频
//    if(m_categories.count() >= 0)
//        emit categoryChanged();

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
                    if(value == "GetVideoCategories") OnGetVideoCategories(response);
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
