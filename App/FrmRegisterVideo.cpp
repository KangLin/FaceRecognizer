#include "FrmRegisterVideo.h"
#include "ui_FrmRegisterVideo.h"

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
    
}

int CFrmRegisterVideo::MarkFace(QPainter &painter, const QVector<QRect> faces)
{
    QPen pen = painter.pen();
    pen.setColor(Qt::green);
    painter.setPen(pen);
    foreach (auto f, faces) {
        painter.drawRect(f.x(), f.y(), f.width(), f.height());
    }
    return 0;
}

void CFrmRegisterVideo::slotDisplay(const QImage &image)
{
    if(!m_pFace) return;

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
 
    auto faces = m_pFace->GetDector()->Detect(image);
    MarkFace(painter, faces);
    if(faces.size() > 1)
        SetStatusInformation(tr("Please only a person before the camera"));
    else if(faces.size() == 0)
        SetStatusInformation(tr("Please face into box"));
    else
        SetStatusInformation(tr("Please push register button"));

    ui->wgDisplay->slotDisplay(img);
}
