#include "AppLauncher.hpp"

#include <QUrl>
#include <QApplication>
#include <QQmlApplicationEngine>
#include "Common/Finally.hpp"
#include "AppContext.hpp"
#include "Models.hpp"

namespace App {

std::unique_ptr<AppLauncher> AppLauncher::create()
{
    return std::make_unique<AppLauncher>();
}

AppLauncher::AppLauncher()
{
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QGuiApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication::setFallbackSessionManagementEnabled(false);

    // dummy
    QApplication::setApplicationName("noName");
    QApplication::setApplicationVersion("0.0.0");
    QApplication::setOrganizationName("0.0.0");
    QApplication::setOrganizationDomain("0.0.0");
}

AppLauncher::~AppLauncher() = default;

int AppLauncher::start(int argc, char** argv)
{
    qDebug() << "Start app";
    QApplication app(argc, argv);
    {
        Library::Common::Finally fin{
            [this] {
                destroy();
            }
        };
        auto ctx = AppContext::ptr();
        ctx->initializeGLContext();
        initializeQml();
        QQmlApplicationEngine qmlEngine;
        qmlEngine.load(QUrl("qrc:/UI/MainWindow.qml"));
        return app.exec();
    }
}

void AppLauncher::destroy()
{
    auto ctx = AppContext::ptr();
    ctx->deinitializeGLContext();
}

void AppLauncher::initializeQml()
{
    qmlRegisterType<Models::Surface3D>("Models", 1, 0, "OSurface3D");
    qmlRegisterType<Models::MainWindow>("Models", 1, 0, "OMainWindow");
}

}