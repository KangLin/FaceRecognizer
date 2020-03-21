#ifndef RECOGNIZEROPENCV_H
#define RECOGNIZEROPENCV_H

#include "Recognizer.h"
#include "opencv2/core.hpp"
#include "opencv2/face.hpp"

class CRecognizerOpenCV : public CRecognizer
{
    Q_OBJECT
public:
    explicit CRecognizerOpenCV(CFace* pFace = nullptr, QObject *parent = nullptr);

    /**
     * @brief Register face and save register image
     * @param image: face image
     * @return register index. other return -1.
     */
    virtual qint64 Register(const QImage& image, const QRect &face = QRect());

    virtual int Delete(const qint64 &index);
    /**
     * @brief Query register face
     * @param image: query face image
     * @return find index. other return -1
     */
    virtual qint64 Query(const QImage& image, const QRect &face = QRect());

    /**
     * @brief Save feature to file
     * @param szFile: feature file name
     * @return 
     */
    virtual int Save(const QString &szFile = QString());
    /**
     * @brief Load feature from file
     * @param szFile: feature file name
     * @return 
     */
    virtual int Load(const QString &szFile = QString());

protected:
    virtual int UpdateParameter();

    cv::Ptr<cv::face::FaceRecognizer> m_Recognizer;
    qint64 m_nMaxIndex;
};

#endif // RECOGNIZEROPENCV_H
