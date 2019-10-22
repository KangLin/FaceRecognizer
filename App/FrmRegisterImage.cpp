#include "FrmRegisterImage.h"
#include "ui_FrmRegisterImage.h"
#include "Log.h"
#include "RabbitCommonDir.h"

#include <QMessageBox>
#include <QPainter>
#include <QDebug>

CFrmRegisterImage::CFrmRegisterImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmRegisterImage),
    m_bRegister(false)
{
    ui->setupUi(this);
    ui->lbOldImage->setVisible(false);
    
    m_pFace = CFactory::Instance();
    ui->lbID->setText(QString::number(m_pFace->GetRecognizer()->GetCount()));
}

CFrmRegisterImage::~CFrmRegisterImage()
{
    if(m_pFace->GetRecognizer()->GetCount() > 0 && m_bRegister)
        m_pFace->GetRecognizer()->Save();
    delete ui;
}

void CFrmRegisterImage::on_pbBrower_clicked()
{
    QString szFile = RabbitCommon::CDir::GetOpenFileName(this,
                                                         tr("Select image"),
                                                         ui->leFile->text(),
           tr("Image files(*.png *.gif *.jpeg *.jpg *.bmp);; All files(*.*)"));
    ui->leFile->setText(szFile);
    
    QImage image(szFile);
    m_Image = image;
    MarkFace(image);
    ui->lbImage->setPixmap(QPixmap::fromImage(image));
}

void CFrmRegisterImage::on_pbRegister_clicked()
{
    QImage image = ui->lbImage->pixmap()->toImage();
    auto faces = m_pFace->GetDector()->Detect(image);
    if(faces.size() != 1)
    {
        QMessageBox::warning(this, tr("Face register"),
                             tr("Please select a photo with only one person"));
        return;
    }
    
    foreach (auto f, faces) {
        auto points = m_pFace->GetLandmarker()->Mark(image, f);
        CRecognizer::_INFO info;
        info = m_pFace->GetRecognizer()->Query(image, points);
        if(info.index > -1)
        {
            qDebug() << "There is exists" << info.index;
            ui->lbOldImage->setPixmap(QPixmap::fromImage(
                                      QImage(info.szImageFile)));
            ui->lbOldImage->setVisible(true);
        }
        info.no = ui->leNo->text().toLongLong();
        info.szName = ui->leName->text();
        qint64 index = m_pFace->GetRecognizer()->Register(
                    image, points, info);
        
        ui->lbID->setText(QString::number(m_pFace->GetRecognizer()->GetCount()));
        m_bRegister = true;
    }
}

int CFrmRegisterImage::MarkFace(QImage &image)
{
    QPainter painter(&image);
    QPen pen(Qt::green);
    pen.setWidth(2);

    painter.setPen(pen);
    auto faces = m_pFace->GetDector()->Detect(image);   
    foreach (auto f, faces) {
        painter.drawRect(f.x(), f.y(), f.width(), f.height());
        auto points = m_pFace->GetLandmarker()->Mark(image, f);
        for (auto &point : points)
        {
            {
                painter.drawPoint(point.x(), point.y());
                //painter.drawEllipse(point.x - 1, point.y - 1, 2, 2);
            }
        }
    }
    return 0;
}
