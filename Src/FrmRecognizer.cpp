#include "FrmRecognizer.h"
#include "ui_FrmRecognizer.h"
#include "ImageTool.h"
#include "RabbitCommonDir.h"

#include <QSettings>
#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QTime>

#define DEBUG_DISPLAY_TIME 0

CFrmRecognizer::CFrmRecognizer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmRecognizer)
{
    ui->setupUi(this);
    m_Threshold = 0.7f;
    QSettings set(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                  QSettings::IniFormat);
    QString szPath = set.value("ModuleDir").toString();
    if(!szPath.isEmpty() && (szPath.right(1) != QDir::separator()))
    {
        szPath += QDir::separator();
    }
    InitSeeta(szPath);
    
}

CFrmRecognizer::~CFrmRecognizer()
{
    delete ui;
}

int CFrmRecognizer::SetModelPath(const QString &szPath)
{
    QString szFile = szPath;
    if(!szFile.isEmpty() && (szFile.right(1) != QDir::separator()))
    {
        szFile += QDir::separator();
    }
    return InitSeeta(szFile);
}

int CFrmRecognizer::InitSeeta(const QString& szPath)
{
    m_FD_model.reset();
    m_FL_model.reset();
    m_FDB_model.reset();
    m_FD.reset();
    m_FL.reset();
    m_FDB.reset();

    m_Device = seeta::ModelSetting::CPU;
    int id = 0;
    try {
        QString szFD = szPath + "fd_2_00.dat";
        m_FD_model = QSharedPointer<seeta::ModelSetting>(new seeta::ModelSetting(
                                            szFD.toStdString(), m_Device, id));
        QString szFL = szPath + "pd_2_00_pts5.dat";
        m_FL_model = QSharedPointer<seeta::ModelSetting>(new seeta::ModelSetting(
                                            szFL.toStdString(), m_Device, id));
        QString szFDB= szPath + "fr_2_10.dat";
        m_FDB_model = QSharedPointer<seeta::ModelSetting>(new seeta::ModelSetting(
                                            szFDB.toStdString(), m_Device, id));
        m_FD = QSharedPointer<seeta::FaceDetector>(new seeta::FaceDetector(*m_FD_model));
        m_FL = QSharedPointer<seeta::FaceLandmarker>(new seeta::FaceLandmarker(*m_FL_model));
        m_FDB = QSharedPointer<seeta::FaceDatabase>(new seeta::FaceDatabase(*m_FDB_model));

        //m_FD->set(seeta::FaceDetector::PROPERTY_VIDEO_STABLE, 1); 
        //set face detector's min face size
        m_FD->set( seeta::FaceDetector::PROPERTY_MIN_FACE_SIZE, 40 );

        LoadDatabase();
    } catch (...) {
        QMessageBox msg(QMessageBox::Critical, tr("Exception"), tr("Load model file exception, please set model file path"));
        msg.exec();
        qCritical() << "Load model file exception, please set model file path";
    }

    return 0;
}

qint64 CFrmRecognizer::LoadDatabase()
{
    qint64 id = 0;

    m_Database.clear();
    QDir d(RabbitCommon::CDir::Instance()->GetDirUserImage());
    QFileInfoList fileInfoList = d.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "."|| fileInfo.fileName() == "..")
            continue;
        if(fileInfo.isFile())
        {
            QImage image;
            if(!image.load(fileInfo.filePath()))
                continue;
            if(image.format() != QImage::Format_RGB888)
            {                
                image = image.convertToFormat(QImage::Format_RGB888);
            }
            image = image.rgbSwapped();

            SeetaImageData imageData;
            imageData.width = image.width();
            imageData.height = image.height();
            imageData.data = image.bits();
            imageData.channels = 3;
            auto faces = m_FD->detect(imageData);
            //qDebug() << "face:" << faces.size;
            for (int i = 0; i < faces.size; i++)
            {
                auto &face = faces.data[i];
                if(!m_FL) 
                {
                    qCritical() << "seeta::FaceLandmarker isn't init";
                    return -2;
                }
                auto points = m_FL->mark(imageData, face.pos);
                try
                {
					qint64 id = m_FDB->Register( imageData, points.data() );
                    m_Database[id] = fileInfo.fileName();
                    qDebug() << "Register: id =" << id << "; file name:" << fileInfo.fileName();
                }
                catch (...)
                {
					continue;
                }
            }
        }
    }
    qInfo() << "Registed faces number:" << m_Database.size();
    return id;
}

void CFrmRecognizer::slotDisplay(const QImage &frame)
{
    QPainter painter(this);
#if DEBUG_DISPLAY_TIME
    QTime t = QTime::currentTime();
#endif
    m_Image = frame;
    QImage out = frame.rgbSwapped();
    Recognizer(out);
    MarkFace(m_Image);
#if DEBUG_DISPLAY_TIME
    qDebug() << "Process time:" << t.msecsTo(QTime::currentTime()) << "ms";
#endif
    ui->frmDisplay->slotDisplay(m_Image);
}

int CFrmRecognizer::Recognizer(QImage &image)
{
    SeetaImageData imageData;
    imageData.width = image.width();
    imageData.height = image.height();
    imageData.data = image.bits();
    imageData.channels = 3;
    //qDebug() << "frame width:" << image.width() << image.height();
    if(!m_FD)
    {
        qCritical() << "seeta::FaceDetector isn't init";
        return -1;
    }
    m_FaceInfo.clear();
#if DEBUG_DISPLAY_TIME
    QTime t1 = QTime::currentTime();
#endif
    
    m_Faces = m_FD->detect(imageData);
    
#if DEBUG_DISPLAY_TIME
    QTime t2 = QTime::currentTime();
    qDebug() << "detect time:" << t1.msecsTo(t2) << "ms";
#endif
    for (int i = 0; i < m_Faces.size; i++)
    {
        auto &face = m_Faces.data[i];
        if(!m_FL) 
        {
            qCritical() << "seeta::FaceLandmarker isn't initialization";
            return -2;
        }
        auto points = m_FL->mark(imageData, face.pos);
        
#if DEBUG_DISPLAY_TIME
        QTime t3 = QTime::currentTime();
        qDebug() << "Landmark time:" << t2.msecsTo(t3) << "ms";
#endif
        _FACE &f = m_FaceInfo[i];
        m_FaceInfo[i].LandmarkPoints = points;
        
        // Query top 1
        int64_t index = -1;
        float similarity = 0;
        auto queried = m_FDB->QueryTop(imageData, points.data(), 1, &index, &similarity);
#if DEBUG_DISPLAY_TIME
        QTime t4 = QTime::currentTime();
        qDebug() << "Recognize time:" << t3.msecsTo(t4) << "ms";
#endif
        // no face queried from database
        if (queried < 1) continue;

        // similarity greater than threshold, means recognized
        if( similarity > m_Threshold )
        {
            f.szName = m_Database[index];
            f.similarity = similarity;
            qDebug() << f.szName << f.similarity;
        }
    }
    return 0;
}

int CFrmRecognizer::MarkFace(QImage &image)
{
    QPainter painter(&image);
    QPen pen(Qt::green);
    pen.setWidth(2);

    painter.setPen(pen);
    for (int i = 0; i < m_Faces.size; i++)
    {
        auto &face = m_Faces.data[i];
        _FACE f = m_FaceInfo[i];
        
        painter.drawRect(face.pos.x, face.pos.y, face.pos.width, face.pos.height);
        painter.drawText(face.pos.x, face.pos.y, f.szName);
        
        for (auto &point : f.LandmarkPoints)
        {
            {
                painter.drawPoint(point.x, point.y);
                //painter.drawEllipse(point.x - 1, point.y - 1, 2, 2);
            }
        }
    }

    return 0;
}
