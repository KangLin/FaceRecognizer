#ifndef FRMRECOGNIZER_H
#define FRMRECOGNIZER_H

#include <QWidget>
#include <QVideoFrame>
#include <QMap>
#include <vector>

#include "seeta/FaceDetector.h"
#include "seeta/FaceLandmarker.h"
#include "seeta/FaceDatabase.h"

#include "facerecognizer_export.h"

namespace Ui {
class CFrmRecognizer;
}

class FACERECOGNIZER_EXPORT CFrmRecognizer : public QWidget
{
    Q_OBJECT
    
public:
    explicit CFrmRecognizer(QWidget *parent = nullptr);
    virtual ~CFrmRecognizer();

    int SetModelPath(const QString &szPath);
    
public Q_SLOTS:
    virtual void slotDisplay(const QImage &frame);
    
private:
    virtual int InitSeeta(const QString &szPath = QString());
    qint64 LoadDatabase();
    int Recognizer(QImage &image);
    virtual int MarkFace(QImage &image);
    
private:
    Ui::CFrmRecognizer *ui;
    
    seeta::ModelSetting::Device m_Device;
    QSharedPointer<seeta::ModelSetting> m_FD_model;
    QSharedPointer<seeta::ModelSetting> m_FL_model;
    QSharedPointer<seeta::ModelSetting> m_FDB_model;
    QSharedPointer<seeta::ModelSetting> m_FR_model;
     
    QSharedPointer<seeta::FaceDetector> m_FD;
    QSharedPointer<seeta::FaceLandmarker> m_FL;
    QSharedPointer<seeta::FaceDatabase> m_FDB;
    QSharedPointer<seeta::FaceRecognizer> m_FR;
    
    struct _FACE{
        qint64 id;
        std::vector<SeetaPointF> LandmarkPoints;
        QString szName;
        float similarity;
    };

    SeetaFaceInfoArray m_Faces;
    QMap<int, _FACE> m_FaceInfo;
    
    QMap<int, QString> m_Database;
    
    QImage m_Image;
    QImage m_ImageOut;
    
    float m_Threshold;  // recognization threshold
};

#endif // FRMRECOGNIZER_H
