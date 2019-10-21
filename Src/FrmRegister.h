/* Register 
 * Author: Kang Lin (kl222@126.com)
 * 
 * 
 * 
 */

#ifndef CFRMREGISTER_H
#define CFRMREGISTER_H

#include "FrmDisplay.h"

#include "seeta/FaceDetector.h"
#include "seeta/FaceLandmarker.h"
#include "seeta/FaceDatabase.h"

#include "facerecognizer_export.h"

#include <QVideoFrame>
#include <QSharedPointer>
#include <QMap>

namespace Ui {
class CFrmRegister;
}

class FACERECOGNIZER_EXPORT CFrmRegister : public QWidget
{
    Q_OBJECT
    
public:
    explicit CFrmRegister(QWidget *parent = nullptr);
    virtual ~CFrmRegister() override;

    int SetModelPath(const QString &szPath);
    
public Q_SLOTS:
    virtual void slotDisplay(const QImage &frame);
    
private slots:
    void on_pbRegister_clicked();
    void on_pbCancle_clicked();

protected:
    qint64 LoadDatabase();
    qint64 LoadDatabaseFromDirectory();
    int SaveDatabase(const qint64 &id = -1, const QString &szName = QString());
    virtual int InitSeeta(const QString &szPath = QString());
    virtual int Detecetor(QImage &image);
    qint64 Register(QImage &image);
    virtual int MarkFace(QImage &image);

private:
    Ui::CFrmRegister *ui;
    
    seeta::ModelSetting::Device m_Device;
    QSharedPointer<seeta::ModelSetting> m_FD_model;
    QSharedPointer<seeta::ModelSetting> m_FL_model;
    QSharedPointer<seeta::ModelSetting> m_FDB_model;
    QSharedPointer<seeta::ModelSetting> m_FR_model;

    QSharedPointer<seeta::FaceDetector> m_FD;
    QSharedPointer<seeta::FaceLandmarker> m_FL;
    QSharedPointer<seeta::FaceDatabase> m_FDB;
    QSharedPointer<seeta::FaceRecognizer> m_FR;
    
    SeetaFaceInfoArray m_Faces;
    QMap<int, std::vector<SeetaPointF> > m_LandmarksPoints;

    QImage m_Image;
    
    QString m_szDb;
};

#endif // CFRMREGISTER_H
