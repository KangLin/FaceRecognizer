#include "FrmRegister.h"
#include "ui_FrmRegister.h"
#include "FrmDisplay.h"
#include "RabbitCommonDir.h"
#include "Tool.h"

#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QSettings>

CFrmRegister::CFrmRegister(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmRegister)
{
    ui->setupUi(this);

    m_Rotation = 0;
    QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                  QSettings::IniFormat);
    QString szFile = set.value("ModuleDir").toString();
    if(!szFile.isEmpty())
        InitSeeta(szFile);
}

CFrmRegister::~CFrmRegister()
{
    delete ui;
}

int CFrmRegister::SetModelPath(const QString &szPath)
{
    return InitSeeta(szPath);
}

int CFrmRegister::InitSeeta(const QString& szPath)
{
    m_FD_model.reset();
    m_FL_model.reset();
    m_FR_model.reset();
    m_FDB_model.reset();
    m_FD.reset();
    m_FL.reset();
    m_FDB.reset();
    m_FR.reset();
    
    m_Device = seeta::ModelSetting::CPU;
    int id = 0;
    try {
        QString szFD = szPath + "/fd_2_00.dat";
        m_FD_model = QSharedPointer<seeta::ModelSetting>(new seeta::ModelSetting(
                                            szFD.toStdString(), m_Device, id));
        QString szFL = szPath + "/pd_2_00_pts81.dat";
        m_FL_model = QSharedPointer<seeta::ModelSetting>(new seeta::ModelSetting(
                                            szFL.toStdString(), m_Device, id));
        QString szFDB= szPath + "/fr_2_10.dat";
        m_FDB_model = QSharedPointer<seeta::ModelSetting>(new seeta::ModelSetting(
                                            szFDB.toStdString(), m_Device, id));
        m_FD = QSharedPointer<seeta::FaceDetector>(new seeta::FaceDetector(*m_FD_model));
        m_FL = QSharedPointer<seeta::FaceLandmarker>(new seeta::FaceLandmarker(*m_FL_model));
        m_FDB = QSharedPointer<seeta::FaceDatabase>(new seeta::FaceDatabase(*m_FDB_model));
        
        m_FD->set(seeta::FaceDetector::PROPERTY_VIDEO_STABLE, 1);     
    } catch (...) {
        QMessageBox msg(QMessageBox::Critical, tr("Exception"), tr("Load model file exception, please set model file path"));
        msg.exec();
        qCritical() << "Load model file exception, please set model file path";
    }
    
    return 0;
}

void CFrmRegister::slotDisplay(const QVideoFrame &frame)
{
    QPainter painter(this);
    QImage image = CTool::ConverFormat(frame);
    if(m_Rotation)
        image = image.transformed(QTransform().rotate(m_Rotation));

    m_ImageOut = image.convertToFormat(QImage::Format_RGB888);
    
    QImage out = m_ImageOut.rgbSwapped();
    Detecetor(out);
    MarkFace(m_ImageOut);
    ui->frmDisplay->slotDisplay(m_ImageOut);
}

int CFrmRegister::SetCameraAngle(int rotation)
{
    m_Rotation = rotation;
    return 0;
}

int CFrmRegister::MarkFace(QImage &image)
{
    QPainter painter(&image);
    QPen pen(Qt::green);
    pen.setWidth(2);
    
    painter.setPen(pen);
    for (int i = 0; i < m_Faces.size; i++)
    {
        auto &face = m_Faces.data[i];
        painter.drawRect(face.pos.x, face.pos.y, face.pos.width, face.pos.height);
    }
    
    for (auto &v : m_LandmarksPoints)
    {
        for(auto &point: v)
            painter.drawPoint(point.x, point.y);
            //painter.drawEllipse(point.x - 1, point.y - 1, 2, 2);
    }
    return 0;
}

int CFrmRegister::Detecetor(QImage &image)
{   
    SeetaImageData imageData;
    imageData.width = image.width();
    imageData.height = image.height();
    imageData.data = image.bits();
    imageData.channels = 3;
    
    if(!m_FD)
    {
        qCritical() << "seeta::FaceDetector isn't init";
        return -1;
    }
    m_LandmarksPoints.clear();
    m_Faces = m_FD->detect(imageData);
    for (int i = 0; i < m_Faces.size; i++)
    {
        auto &face = m_Faces.data[i];
        if(!m_FL) 
        {
            qCritical() << "seeta::FaceLandmarker isn't init";
            return -2;
        }
        auto points = m_FL->mark(imageData, face.pos);
        m_LandmarksPoints[i] = points;
    }
    return 0;
}

qint64 CFrmRegister::Register(QImage &image)
{
    SeetaImageData imageData;
    imageData.width = image.width();
    imageData.height = image.height();
    imageData.data = image.bits();
    imageData.channels = 3;

    qint64 id = m_FDB->Register( imageData, m_LandmarksPoints[0].data() );
    ui->lbID->setText(QString::number(id));
    
    QString szFile = RabbitCommon::CDir::Instance()->GetDirUserImage() 
            + QDir::separator()
            + ui->leName->text() + "_" + ui->lbID->text() + ".jpg";
    m_Image.save(szFile);
    return id;
}

void CFrmRegister::on_pbRegister_clicked()
{
    Register(m_ImageOut);
}

void CFrmRegister::on_pbCancle_clicked()
{
    qApp->exit();
}
