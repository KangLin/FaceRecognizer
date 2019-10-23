#ifndef CPARAMETERRECOGNIZER_H
#define CPARAMETERRECOGNIZER_H

#include <QObject>
#include "Parameter.h"

class FACERECOGNIZER_EXPORT CParameterRecognizer : public CParameter
{
    Q_OBJECT
public:
    explicit CParameterRecognizer(QObject *parent = nullptr);
    
    /**
     * @brief set feature file name
     * @param szFile: feature file name
     * @return 
     */
    virtual int SetFeatureFile(const QString &szFile);
    virtual QString GetFeatureFile();
    
    /**
     * @brief Set register image path
     * @param szPath: image path
     * @return 
     */
    virtual int SetRegisterImagePath(const QString &szPath);
    virtual QString GetRegisterImagePath();

private:
    QString m_szFeatureFile;
    QString m_szRegisterImagePath;
};

#endif // CPARAMETERRECOGNIZER_H
