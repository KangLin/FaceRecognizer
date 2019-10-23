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
    ui->lbStatus->setText(tr("Please select image"));
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
    ui->lbID->setText(QString());
    ui->lbStatus->setText(tr("Have selected images"));
    ShowReplaceUI(false);
}

void CFrmRegisterImage::on_pbRegister_clicked()
{
    if(!m_pFace) return;
    if(Check())
        return;
    
    QString szMsg;
    QImage image = m_Image;
    auto faces = m_pFace->GetDector()->Detect(image);
    if(faces.size() != 1)
    {
        QString szMsg = tr("Error: Please select a photo with only one person");
        ui->lbStatus->setText(szMsg);
        QMessageBox::warning(this, tr("Face register"),
                             szMsg);
        return;
    }
    
    foreach (auto f, faces) {
        auto points = m_pFace->GetLandmarker()->Mark(image, f);
        if(!m_bReplace)
        {
            auto index = m_pFace->GetRecognizer()->Query(image, points);
            if(index > -1)
            {
                CDataRegister data;
                if(m_pFace->GetDatabase()->GetRegisterInfo(index, &data))
                    return;
                ShowReplaceUI(true);
                ui->leNoOld->setText(QString::number(data.getNo()));
                ui->leNameOld->setText(data.getName());
                ui->lbIDOld->setText(QString::number(index));
                ui->lbOldImage->setPixmap(QPixmap::fromImage(
                    QImage(m_pFace->GetRecognizer()->GetRegisterImage(index))));               

                QString szMsg = tr("Error: This person already exists. index:");
                szMsg += QString::number(data.getIdx());
                szMsg += "; no:" + QString::number(data.getNo());
                szMsg += "; name:" + data.getName();
                ui->lbStatus->setText(szMsg);
                return;
            }
        }
        ShowReplaceUI(false);
        qint64 index = m_pFace->GetRecognizer()->Register(
                    image, points);
        if(-1 == index)
        {
            ui->lbStatus->setText(tr("Face register fail"));
            return;
        }
        ui->lbID->setText(QString::number(index));
        m_bRegister = true;
        //Write item to database
        CDataRegister data;
        data.setIdx(index);
        data.setNo(ui->leNo->text().toLongLong());
        data.setName(ui->leName->text());
        if(m_pFace->GetDatabase()->Register(index, &data))
        {            
            m_pFace->GetRecognizer()->Delete(index);
            szMsg = "Error: Write database fail. The no is exists?";
        } else {
            szMsg = tr("Regist success. index:");
            szMsg += QString::number(data.getIdx());
            szMsg += "; no:" + QString::number(data.getNo());
            szMsg += "; name:" + data.getName();
        }
        ui->lbStatus->setText(szMsg);
    }
}

void CFrmRegisterImage::on_pbCancel_clicked()
{
    ShowReplaceUI(false);
    ui->lbStatus->setText(tr("Please select image"));
}

void CFrmRegisterImage::on_pbReplace_clicked()
{
    if(!m_pFace) return;
    if(Check())
        return;
    
    QString szMsg;
    QImage image = m_Image;
    auto faces = m_pFace->GetDector()->Detect(image);
    if(faces.size() != 1)
    {
        QString szMsg = tr("Error: Please select a photo with only one person");
        ui->lbStatus->setText(szMsg);
        QMessageBox::warning(this, tr("Face register"),
                             szMsg);
        return;
    }

    foreach (auto f, faces) {
        auto points = m_pFace->GetLandmarker()->Mark(image, f);
        
        if(m_bReplace)
        {
            qint64 index = ui->lbIDOld->text().toLongLong();
            m_pFace->GetRecognizer()->Delete(index);
            // Delete item from database
            m_pFace->GetDatabase()->Delete(index);
        } 
        
        ShowReplaceUI(false);
        qint64 index = m_pFace->GetRecognizer()->Register(
                    image, points);
        if(-1 == index)
        {
            ui->lbStatus->setText(tr("Face register fail"));
            return;
        }
        ui->lbID->setText(QString::number(index));
        m_bRegister = true;
        //Write item to database
        CDataRegister data;
        data.setIdx(index);
        data.setNo(ui->leNo->text().toLongLong());
        data.setName(ui->leName->text());
        if(m_pFace->GetDatabase()->Register(index, &data))
        {            
            m_pFace->GetRecognizer()->Delete(index);
            szMsg = "Error: Write database fail. The no is exists?";
        } else {
            szMsg = tr("Regist success. index:");
            szMsg += QString::number(data.getIdx());
            szMsg += "; no:" + QString::number(data.getNo());
            szMsg += "; name:" + data.getName();
        }
        ui->lbStatus->setText(szMsg);
    }
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
        ui->pbReplace->setVisible(true);
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
        ui->pbReplace->setVisible(false);
        m_bReplace = false;
    }
    return 0;
}

int CFrmRegisterImage::Check()
{
    if(ui->leNo->text().isEmpty())
    {
        ui->lbStatus->setText(tr("Error: Please input no"));
        return -1;
    }
    
    if(ui->leName->text().isEmpty())
    {
        ui->lbStatus->setText(tr("Error: Please input name"));
        return -2;
    }
    
    return 0;
}
