#ifndef CPLUGSMANAGER_H
#define CPLUGSMANAGER_H

#include <QObject>
#include <QDir>

#include "facerecognizer_export.h"
#include "ConverFormat.h"

class FACERECOGNIZER_EXPORT CPlugsManager : public QObject
{
    Q_OBJECT
public:
    explicit CPlugsManager(QObject *parent = nullptr);

    static CPlugsManager* Instance();

    int LoadPlugs();
    int FindPlugins(QDir dir);

    CConverFormat* GetConverFormat(const QString &szName = QString());

private:
    QMap<QString, CConverFormat*> m_ConverFormat;
};

#endif // CPLUGSMANAGER_H
