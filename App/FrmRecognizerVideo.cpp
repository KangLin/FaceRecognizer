#include "FrmRecognizerVideo.h"
#include "ui_FrmRecognizerVideo.h"
#include "Factory.h"
#include "Performance.h"

#include <QPainter>

CFrmRecognizerVideo::CFrmRecognizerVideo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmRecognizerVideo)
{
    ui->setupUi(this);
    SetStatusInformation(tr("Please face into box"));
    m_pFace = CFactory::Instance();
    if(!m_pFace)
    {
        throw std::runtime_error("CFrmRegisterVideo consturct allocte memory fail");
    }
}

CFrmRecognizerVideo::~CFrmRecognizerVideo()
{
    delete ui;
}

int CFrmRecognizerVideo::SetStatusInformation(const QString &szInfo, int nRet, STATUS_TYPE type)
{
    QPalette palette;
    QString szMsg;
    switch (type) {
    case ERROR:
        szMsg = tr("ERROR: ");
        palette.setColor(QPalette::WindowText, QColor(255, 0, 0));
        break;
    case FAIL:
        szMsg = tr("FAIL: ");
        palette.setColor(QPalette::WindowText, QColor(255, 0, 0));
        break;
    case SUCCESS:
        szMsg = tr("SUCCESS: ");
        palette.setColor(QPalette::WindowText, QColor(0, 255, 0));
        break;
    default:
        break;
    }
    szMsg += szInfo;

    ui->lbStatus->setPalette(palette);
    ui->lbStatus->setText(szMsg);
    return nRet;
}

void CFrmRecognizerVideo::slotDisplay(const QImage &image)
{
    QImage img = image;
    QPainter painter(&img);
    QPen pen(Qt::green);
    pen.setWidth(2);
    painter.setPen(pen);
    PERFORMANCE(CFrmRecognizerVideo);
    auto faces = m_pFace->GetDector()->Detect(image);
    PERFORMANCE_ADD_TIME(CFrmRecognizerVideo,
                         "Dectect " + QString::number(faces.size()) + " faces");
    foreach (auto f, faces) {
        painter.drawRect(f.x(), f.y(), f.width(), f.height());
    }
    PERFORMANCE_ADD_TIME(CFrmRecognizerVideo, "MarkFace");
    
    ui->wgDisplay->slotDisplay(image);
    
    if(faces.size() > 1)
    {
        SetStatusInformation(tr("Please only a person before the camera"));
        return;
    } else if(faces.size() == 0) {
        SetStatusInformation(tr("Please face befor camera"));
        return;
    }
    
    //TOD: recognizer
    
}
