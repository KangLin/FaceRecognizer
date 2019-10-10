#include "FrmRegister.h"
#include "ui_FrmRegister.h"
#include "FrmDisplay.h"
#include "RabbitCommonDir.h"
#include "Log.h"

#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QSettings>

CFrmRegister::CFrmRegister(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmRegister)
{
    ui->setupUi(this);
    ui->lbInformation->setText(QString());
    ui->lbID->setText(QString::number(0));
    
    QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                  QSettings::IniFormat);
    QString szPath = set.value("ModuleDir").toString();
    if(!szPath.isEmpty() && (szPath.right(1) != QDir::separator()))
    {
        szPath += QDir::separator();
    }
    InitSeeta(szPath);
}

CFrmRegister::~CFrmRegister()
{
    delete ui;
}

int CFrmRegister::SetModelPath(const QString &szPath)
{
    QString szFile = szPath;
    if(!szFile.isEmpty() && (szFile.right(1) != QDir::separator()))
    {
        szFile += QDir::separator();
    }
    return InitSeeta(szFile);
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
        QString szFD = szPath + QDir::separator() + "fd_2_00.dat";
        m_FD_model = QSharedPointer<seeta::ModelSetting>(new seeta::ModelSetting(
                                            szFD.toStdString(), m_Device, id));
        QString szFL = szPath + QDir::separator() + "pd_2_00_pts81.dat";
        m_FL_model = QSharedPointer<seeta::ModelSetting>(new seeta::ModelSetting(
                                            szFL.toStdString(), m_Device, id));
        QString szFDB= szPath + QDir::separator() + "fr_2_10.dat";
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

void CFrmRegister::slotDisplay(const QImage &frame)
{
    QPainter painter(this);

    m_Image = frame;
    QImage img = frame;
    QImage out = frame.rgbSwapped();
    Detecetor(out);
    MarkFace(img);
    ui->frmDisplay->slotDisplay(img);
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

    if(m_LandmarksPoints.size() != 1)
    {
        LOG_MODEL_DEBUG("CFrmRegister", "Don't detecetor face");
        return -1;
    }
    qint64 id = m_FDB->Register( imageData, m_LandmarksPoints[0].data() );
    ui->lbID->setText(QString::number(id));
    QString szFile = RabbitCommon::CDir::Instance()->GetDirUserImage() 
            + QDir::separator()
            + ui->leName->text() + "_" + ui->lbID->text() + ".jpg";
    if(!m_Image.save(szFile))
        LOG_MODEL_ERROR("CFrmRegister", "Save register picture fail: %s",
                        szFile.toStdString().c_str());
    return id;
}

void CFrmRegister::on_pbRegister_clicked()
{
    qint64 id = Register(m_Image);
    if(id >= 0)
        ui->lbInformation->setText("Id: " + QString::number(id)
                                   + " " + tr("Name: ") + ui->leName->text()
                                   + " " + tr("register success"));
    else
        ui->lbInformation->setText(tr("Register fail"));
}

void CFrmRegister::on_pbCancle_clicked()
{
    qApp->exit();
}
