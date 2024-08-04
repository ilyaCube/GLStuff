#pragma once

#include <QUrl>
#include <QObject>
#include <QOpenGLContext>
#include <QOffscreenSurface>

namespace Models {

class MainWindow : public QObject
{
    Q_OBJECT
public:
    MainWindow(QObject* parent = nullptr);

public slots:
    void initSurface(QObject *objSurface);
};

}