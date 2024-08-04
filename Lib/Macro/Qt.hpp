#pragma once

#include <QObject>

#define READONLY_PROPERTY(PROPERTY, GETTER) \
    Q_PROPERTY(QVariant PROPERTY READ GETTER NOTIFY PROPERTY##Changed) \
    Q_SIGNAL void PROPERTY##Changed(const QVariant& value);

#define PROPERTY_CHANGED(PROPERTY, NEWVALUE) \
    emit PROPERTY##Changed(NEWVALUE)