#include "MainWindow.hpp"

#include "Surface3D.hpp"
#include "Imaging/Helpers.hpp"

namespace Models {

MainWindow::MainWindow(QObject* parent)
    : QObject(parent)
{
}

void MainWindow::initSurface(QObject *objSurface)
{
    auto surface = qobject_cast<Models::Surface3D*>(objSurface);
    if(surface == nullptr) {
        qWarning() << "Error when surface initializing";
        return;
    }
    surface->loadImageUrl(QUrl("file:///home/kuia/Pictures/logo2.png"));
}

}