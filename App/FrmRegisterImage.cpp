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
    ShowReplaceUI(false);
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
            qint64 index = ui->lbID->text().toLongLong();
            m_pFace->GetRecognizer()->Delete(index);
            // Delete item from database
            m_pFace->GetDatabase()->Delete(index);
        } else {
            auto index = m_pFace->GetRecognizer()->Query(image, points);
            if(index > -1)
            {
                CDataRegister data;
                m_pFace->GetDatabase()->GetRegisterInfo(index, &data);
                ui->leNoOld->setText(QString::number(data.getNo()));
                ui->leNameOld->setText(data.getName());
                ui->lbIDOld->setText(QString::number(index));
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
        //Write item to database
        CDataRegister data;
        data.setIdx(index);
        data.setNo(ui->leNo->text().toLongLong());
        data.setName(ui->leName->text());
        m_pFace->GetDatabase()->Register(index, &data);
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
        ui->lbNoOld->setVisible(true);
        ui->leNoOld->setVisible(true);
        ui->leNoOld->setEnabled(false);
        ui->lbNameOld->setVisible(true);
        ui->leNameOld->setVisible(true);
        ui->leNameOld->setEnabled(false);
        ui->lbOldID->setVisible(true);
        ui->lbIDOld->setVisible(true);
        ui->lbOldImage->setVisible(true);
        ui->pbCancel->setVisible(true);
        ui->pbRegister->setText(tr("Replace"));
        m_bReplace = true;
    } else {
        ui->lbNoOld->setVisible(false);
        ui->leNoOld->setVisible(false);
        ui->lbNameOld->setVisible(false);
        ui->leNameOld->setVisible(false);
        ui->lbOldID->setVisible(false);
        ui->lbIDOld->setVisible(false);
        ui->lbOldImage->setVisible(false);
        ui->pbCancel->setVisible(false);
        m_bReplace = false;
        ui->pbRegister->setText(tr("Register"));
    }
    return 0;
}
