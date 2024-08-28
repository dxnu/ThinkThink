#include "VideoCategoryController.h"

#include <FileSender.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <VideoCategoryModel.h>
#include <VideoController.h>
#include <VideoModel.h>
#include <AccountController.h>
#include <FileUploader.h>
#include <FileUploadController.h>
#include <VideoCommentController.h>


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

//    VideoCategoryModel *model = new VideoCategoryModel();
//    model->addCategory("Hahsh");
//    model->addCategory("sjalfkjas");

//    QQmlContext *context = engine.rootContext();
//    context->setContextProperty("mymodel", model);


    qmlRegisterType<VideoCategoryModel>("com.video.category.model", 1, 0,
                                                   "VideoCategoryModel");
    qmlRegisterType<VideoModel>("com.video.model", 1, 0, "VideoModel");
    qmlRegisterType<AccountModel>("com.account.model", 1, 0, "AccountModel");
    qmlRegisterType<FileUploader>("com.file.uploader", 1, 0, "FileUploader");
    qmlRegisterType<VideoCommentModel>("com.video.comment.model", 1, 0, "VideoCommentModel");

    VideoCategoryController* videoCateogryController = new VideoCategoryController(&engine);
    VideoController* videoController = new VideoController(&engine);
    AccountController* accountController = new AccountController(&engine);
    FileUploadController* fileUploadController = new FileUploadController(&engine);
    VideoCommentController* videoCommentController = new VideoCommentController(&engine);


    QQmlContext *context = engine.rootContext();
    context->setContextProperty("videoCategoryController", videoCateogryController);
    context->setContextProperty("videoController", videoController);
    context->setContextProperty("accountController", accountController);
    context->setContextProperty("fileUploadController", fileUploadController);
    context->setContextProperty("videoCommentController", videoCommentController);


    const QUrl url(QStringLiteral("qrc:/assets/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    videoCateogryController->setVideoController(videoController);

    engine.load(url);

    return app.exec();
}
