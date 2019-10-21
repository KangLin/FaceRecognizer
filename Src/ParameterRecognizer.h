#ifndef CPARAMETERRECOGNIZER_H
#define CPARAMETERRECOGNIZER_H

#include <QObject>
#include "Parameter.h"

class FACERECOGNIZER_EXPORT CParameterRecognizer : public CParameter
{
    Q_OBJECT
public:
    explicit CParameterRecognizer(QObject *parent = nullptr);
    
    int SetFeatureFile(const QString &szFile);
    QString GetFeatureFile();
    
private:
    QString m_szFeatureFile;
};

#endif // CPARAMETERRECOGNIZER_H
