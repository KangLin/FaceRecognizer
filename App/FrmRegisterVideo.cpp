#include "FrmRegisterVideo.h"
#include "ui_FrmRegisterVideo.h"
#include "Performance.h"

#include <QPainter>
#include <Factory.h>

CFrmRegisterVideo::CFrmRegisterVideo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmRegisterVideo)
{
    m_nImageWidth = 0;
    m_nImageHeight = 0;
    ui->setupUi(this);
    SetStatusInformation(tr("Please face into box"));
    m_pFace = CFactory::Instance();
    if(!m_pFace)
    {
        throw std::runtime_error("CFrmRegisterVideo consturct allocte memory fail");
    }
}

CFrmRegisterVideo::~CFrmRegisterVideo()
{
    delete ui;
}

int CFrmRegisterVideo::SetStatusInformation(const QString &szInfo, int nRet, STATUS_TYPE type)
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

int CFrmRegisterVideo::Check()
{
    if(ui->leNo->text().isEmpty())
    {
        return SetStatusInformation(tr("Please input no"), -1, ERROR); 
    }
    
    if(ui->leName->text().isEmpty())
    {
        return SetStatusInformation(tr("Please input name"), -2, ERROR); 
    }
    return 0;
}

void CFrmRegisterVideo::on_pbRegister_clicked()
{
    if(Check())
        return;

    if(m_Faces.size() != 1)
    {
        SetStatusInformation(tr("Please only a person before the camera"));
        return;
    }

    if(CheckFace(m_box, m_Faces[0]))
        return;

    CParameterRegisterImage para;
    para.SetNo(ui->leNo->text().toLongLong());
    para.SetName(ui->leName->text());
    para.SetImage(m_Image);
    
    emit sigRegister(para);
}

int CFrmRegisterVideo::MarkFace(QPainter &painter, const QVector<QRect> &faces)
{
    QPen pen = painter.pen();
    foreach (auto f, faces) {
        if(CheckFace(m_box, f))
        {
            pen.setColor(Qt::red);
            painter.setPen(pen);
        } else {
            pen.setColor(Qt::green);
            painter.setPen(pen);
        }
        painter.drawRect(f.x(), f.y(), f.width(), f.height());
    }
    return 0;
}

void CFrmRegisterVideo::slotDisplay(const QImage &image)
{
    if(isHidden() || !m_pFace) return;

    m_Image = image;
    QImage img = image;

    if(!(image.width() == m_nImageWidth && image.height() == m_nImageHeight))
    {
        m_nImageWidth = image.width();
        m_nImageHeight = image.height();
        int nHeight = (qMin(m_nImageWidth, m_nImageHeight)) * 2 / 3;
        int nWidth = nHeight * 2 / 3;
        m_box = QRect((m_nImageWidth - nWidth) / 2,
                      (m_nImageHeight - nHeight) / 2,
                      nWidth,
                      nHeight);
    }

    QPainter painter(&img);
    QPen pen(Qt::yellow);
    pen.setWidth(4);
    painter.setPen(pen);
    painter.drawRect(m_box);
 
    PERFORMANCE(CFrmRegisterVideo)
    m_Faces.clear();
    m_pFace->GetDector()->Detect(image, m_Faces);
    PERFORMANCE_ADD_TIME(CFrmRegisterVideo, "Dectect")
    MarkFace(painter, m_Faces);
    PERFORMANCE_ADD_TIME(CFrmRegisterVideo, "MarkFace")
    if(m_Faces.size() > 1)
        SetStatusInformation(tr("Please only a person before the camera"));
    else if(m_Faces.size() == 0)
        SetStatusInformation(tr("Please face into box"));
    else if(CheckFace(m_box, m_Faces[0]))
        SetStatusInformation(tr("Please face into box"));
    else
        SetStatusInformation(tr("Please push register button"));

    ui->wgDisplay->slotDisplay(img);
    PERFORMANCE_ADD_TIME(CFrmRegisterVideo, "slotDisplay")
}

int CFrmRegisterVideo::CheckFace(const QRect &box, const QRect &face)
{
    if(box.width() < face.width() || box.height() < face.height())
    {
        SetStatusInformation(tr("Please stay away from the camera"));
        return -1;
    }
    if(box.width() > face.width() * 2 || box.height() > face.height() * 2)
    {
        SetStatusInformation(tr("Please be close to the camera"));
        return -2;
    }
    if(box.x() > face.x() || box.y() > face.y())
        return -3;
    if(((box.x() + box.width() / 4) < face.x())
            || (box.y() + box.height() / 4 < face.y()))
        return -4;
    return 0;
}
