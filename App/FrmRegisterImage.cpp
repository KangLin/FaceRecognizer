#include "FrmRegisterImage.h"
#include "ui_FrmRegisterImage.h"
#include "Log.h"
#include "RabbitCommonDir.h"

#include <QMessageBox>
#include <QPainter>
#include <QDebug>

CFrmRegisterImage::CFrmRegisterImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmRegisterImage)
{
    ui->setupUi(this);
    m_pFace = CFactory::Instance();
    ui->lbID->setText(QString::number(m_pFace->GetRecognizer()->GetCount()));
}

CFrmRegisterImage::~CFrmRegisterImage()
{
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
        qint64 index = m_pFace->GetRecognizer()->Register(
                    ui->lbImage->pixmap()->toImage(), points);
        qDebug() << "Register index:" << index;
        QString szFile = RabbitCommon::CDir::Instance()->GetDirUserImage()
                + QDir::separator()
                + QString::number(index)
                + "_" + ui->leNo->text()
                + "_" + ui->leName->text();
        m_Image.save(szFile, "png");
        ui->lbID->setText(QString::number(m_pFace->GetRecognizer()->GetCount()));
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
