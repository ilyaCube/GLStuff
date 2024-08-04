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
    surface->loadImageUrl(QUrl("file:///home/bblkycu/Pictures/appearence/volk_siluet_holmy_130119_1920x1080  Shot.jpg"));
}

}