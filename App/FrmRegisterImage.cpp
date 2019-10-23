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
    m_bRegister(false),
    m_bReplace(false)
{
    ui->setupUi(this);
    ui->lbOldImage->setVisible(false);
    ui->pbCancel->setVisible(false);
    m_pFace = CFactory::Instance();
    ui->lbID->setText("");
}

CFrmRegisterImage::~CFrmRegisterImage()
{
    if(m_bRegister && m_pFace)
        m_pFace->GetRecognizer()->Save();
    delete ui;
}

int CFrmRegisterImage::MarkFace(QImage &image)
{
    if(!m_pFace) return -1;
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
    ShowReplaceUI(false);
}

void CFrmRegisterImage::on_pbRegister_clicked()
{
    if(!m_pFace) return;
    QImage image = m_Image;
    auto faces = m_pFace->GetDector()->Detect(image);
    if(faces.size() != 1)
    {
        QMessageBox::warning(this, tr("Face register"),
                             tr("Please select a photo with only one person"));
        return;
    }
    
    foreach (auto f, faces) {
        auto points = m_pFace->GetLandmarker()->Mark(image, f);
        
        if(m_bReplace)
        {
            m_pFace->GetRecognizer()->Delete(ui->lbID->text().toLongLong());
        } else {
            auto index = m_pFace->GetRecognizer()->Query(image, points);
            if(index > -1)
            {
                ui->lbID->setText(QString::number(index));
                ui->lbOldImage->setPixmap(QPixmap::fromImage(
                    QImage(m_pFace->GetRecognizer()->GetRegisterImage(index))));
                ShowReplaceUI(true);
                return;
            }
        }
        ShowReplaceUI(false);
        qint64 index = m_pFace->GetRecognizer()->Register(
                    image, points);
        
        ui->lbID->setText(QString::number(index));
        m_bRegister = true;
        //TODO: write database
        
    }
}

void CFrmRegisterImage::on_pbCancel_clicked()
{
    ShowReplaceUI(false);
}

int CFrmRegisterImage::ShowReplaceUI(bool bReplace)
{
    if(bReplace)
    {
        ui->lbOldImage->setVisible(true);
        ui->pbCancel->setVisible(true);
        ui->pbRegister->setText(tr("Replace"));
        m_bReplace = true;
    } else {
        ui->lbOldImage->setVisible(false);
        ui->pbCancel->setVisible(false);
        m_bReplace = false;
        ui->pbRegister->setText(tr("Register"));
    }
    return 0;
}
