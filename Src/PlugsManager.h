#ifndef CPLUGSMANAGER_H
#define CPLUGSMANAGER_H

#include <QObject>
#include <QDir>

#include "facerecognizer_export.h"

class FACERECOGNIZER_EXPORT CPlugsManager : public QObject
{
    Q_OBJECT
public:
    explicit CPlugsManager(QObject *parent = nullptr);

    int LoadPlugs();
    int FindPlugins(QDir dir);
};

#endif // CPLUGSMANAGER_H
