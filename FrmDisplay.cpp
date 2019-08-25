#include "FrmDisplay.h"
#include "ui_FrmDisplay.h"
#include <QPainter>
#include <QDebug>
#include <QMessageBox>

CFrmDisplay::CFrmDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmDisplay)
{
    ui->setupUi(this);
    m_Rotation = 0;
    InitSeeta("d:\\Source\\build-Face-Desktop_Qt_5_12_4_MSVC2017_64bit-Release\\model\\");
}

CFrmDisplay::~CFrmDisplay()
{
    delete ui;
}

int CFrmDisplay::SetModelPath(const QString &szPath)
{
    return InitSeeta(szPath);
}

int CFrmDisplay::InitSeeta(const QString& szPath)
{
    m_FD_model.reset();
    m_FL_model.reset();
    m_FD.reset();
    m_FL.reset();
    
    m_Device = seeta::ModelSetting::CPU;
    int id = 0;
    try {
        QString szFD = szPath + "/fd_2_00.dat";
        m_FD_model = QSharedPointer<seeta::ModelSetting>(new seeta::ModelSetting(
                                            szFD.toStdString(), m_Device, id));
        QString szFL = szPath + "/pd_2_00_pts81.dat";
        m_FL_model = QSharedPointer<seeta::ModelSetting>(new seeta::ModelSetting(
                                            szFL.toStdString(), m_Device, id));
        m_FD = QSharedPointer<seeta::FaceDetector>(new seeta::FaceDetector(*m_FD_model));
        m_FL = QSharedPointer<seeta::FaceLandmarker>(new seeta::FaceLandmarker(*m_FL_model));
        
        m_FD->set(seeta::FaceDetector::PROPERTY_VIDEO_STABLE, 1);     
    } catch (...) {
        QMessageBox msg(QMessageBox::Critical, tr("Exception"), tr("Load model file exception, please set model file path"));
        msg.exec();
    }
    
    return 0;
}

void CFrmDisplay::slotDisplay(const QVideoFrame &frame)
{
    QImage out;
    QPainter painter(this);
    QVideoFrame videoFrame = frame;
    if(!videoFrame.isValid())
        return;
    if(!videoFrame.map(QAbstractVideoBuffer::ReadOnly))
        return;
    do{
        QImage::Format f = QVideoFrame::imageFormatFromPixelFormat(
                    videoFrame.pixelFormat());
        if(QImage::Format_Invalid == f)
            break;
        QImage image(videoFrame.bits(),
                     videoFrame.width(),
                     videoFrame.height(),
                     videoFrame.width() << 2,
                     f);
        if(m_Rotation)
            image = image.transformed(QTransform().rotate(m_Rotation));

        out = image.convertToFormat(QImage::Format_RGB888);
    }while(0);
    videoFrame.unmap();
    
    Detecetor(out);
    
    m_Image = out;
    update();
}

void CFrmDisplay::paintEvent(QPaintEvent *event)
{
    if(this->isHidden())
        return;
    QPainter painter(this);
    painter.drawImage(this->rect(), m_Image);
}

int CFrmDisplay::SetCameraAngle(int rotation)
{
    m_Rotation = rotation;
    return 0;
}

int CFrmDisplay::Detecetor(QImage &image)
{
    QPainter painter(&image);
    QPen pen(Qt::green);
    pen.setWidth(4);
    painter.setPen(pen);
    
    QImage inImage = image.rgbSwapped();

    SeetaImageData imageData;
    imageData.width = inImage.width();
    imageData.height = inImage.height();
    imageData.data = inImage.bits();
    imageData.channels = 3;
    
    if(!m_FD)
    {
        qCritical() << "seeta::FaceDetector isn't init";
        return -1;
    }
    auto faces = m_FD->detect(imageData);
    for (int i = 0; i < faces.size; ++i)
    {
        auto &face = faces.data[i];
        
        if(!m_FL) 
        {
            qCritical() << "seeta::FaceLandmarker isn't init";
            return -2;
        }
        auto points = m_FL->mark(imageData, face.pos);

        painter.drawRect(face.pos.x, face.pos.y, face.pos.width, face.pos.height);
        for (auto &point : points)
        {
            painter.drawPoint(point.x, point.y);
            //painter.drawEllipse(point.x - 1, point.y - 1, 2, 2);
        }
    }
    return 0;
}
