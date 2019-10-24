#include "FrmRegisterImage.h"
#include "ui_FrmRegisterImage.h"
#include "Log.h"
#include "RabbitCommonDir.h"

#include <QMessageBox>
#include <QPainter>
#include <QDebug>
#include <QPalette>

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
    SetStatusInformation(tr("Please select image"));
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
    SetStatusInformation(tr("Have selected images"));
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
        QString szMsg = tr("Please select a photo with only one person");
        SetStatusInformation(szMsg, -1, ERROR);
        QMessageBox::critical(this, tr("Face register"),
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
                if(m_pFace->GetDatabase()->GetTableRegister()->GetRegisterInfo(index, &data))
                    return;
                ShowReplaceUI(true);
                ui->leNoOld->setText(QString::number(data.getNo()));
                ui->leNameOld->setText(data.getName());
                ui->lbIDOld->setText(QString::number(index));
                ui->lbOldImage->setPixmap(QPixmap::fromImage(
                    QImage(m_pFace->GetRecognizer()->GetRegisterImage(index))));               

                QString szMsg = tr("This person already exists. index:");
                szMsg += QString::number(data.getIdx()) + "; ";
                szMsg += tr("no:") + QString::number(data.getNo()) + "; ";
                szMsg += tr("name:") + data.getName();
                SetStatusInformation(szMsg, -1, ERROR);
                return;
            }
        }
        ShowReplaceUI(false);
        qint64 index = m_pFace->GetRecognizer()->Register(
                    image, points);
        if(-1 == index)
        {
            SetStatusInformation(tr("Face register fail"), -2, FAIL);
            return;
        }
        ui->lbID->setText(QString::number(index));
        m_bRegister = true;
        //Write item to database
        CDataRegister data;
        data.setIdx(index);
        data.setNo(ui->leNo->text().toLongLong());
        data.setName(ui->leName->text());
        if(m_pFace->GetDatabase()->GetTableRegister()->Register(index, &data))
        {            
            m_pFace->GetRecognizer()->Delete(index);
            szMsg = "Write database fail. The no is exists?";
            SetStatusInformation(szMsg, -1, ERROR);
        } else {
            szMsg = tr("Regist success. index:");
            szMsg += QString::number(data.getIdx()) + "; ";
            szMsg += tr("no:") + QString::number(data.getNo()) + "; ";
            szMsg += tr("name:") + data.getName();
            SetStatusInformation(szMsg, 0, SUCCESS);
        }
        
    }
}

void CFrmRegisterImage::on_pbCancel_clicked()
{
    ShowReplaceUI(false);
    SetStatusInformation(tr("Please select image"));
}

void CFrmRegisterImage::on_pbReplace_clicked()
{
    if(!m_pFace) return;
    
    QString szMsg;
    QImage image = m_Image;
    auto faces = m_pFace->GetDector()->Detect(image);
    if(faces.size() != 1)
    {
        QString szMsg = tr("Please select a photo with only one person");
        SetStatusInformation(szMsg, -1, ERROR);
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
            m_pFace->GetDatabase()->GetTableRegister()->Delete(index);
        }
        
        if(Check())
            return;
        
        ShowReplaceUI(false);
        qint64 index = m_pFace->GetRecognizer()->Register(
                    image, points);
        if(-1 == index)
        {
            SetStatusInformation(tr("Face register fail"), -1, FAIL);
            return;
        }
        ui->lbID->setText(QString::number(index));
        m_bRegister = true;
        //Write item to database
        CDataRegister data;
        data.setIdx(index);
        data.setNo(ui->leNo->text().toLongLong());
        data.setName(ui->leName->text());
        if(m_pFace->GetDatabase()->GetTableRegister()->Register(index, &data))
        {            
            m_pFace->GetRecognizer()->Delete(index);
            szMsg = tr("Write database fail. The no is exists?");
            SetStatusInformation(szMsg, -1, ERROR); 
        } else {
            szMsg = tr("Regist success. index:");
            szMsg += QString::number(data.getIdx()) + "; ";
            szMsg += tr("NO:") + QString::number(data.getNo()) + "; ";
            szMsg += tr("name:") + data.getName();
            SetStatusInformation(szMsg, 0, SUCCESS); 
        }
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
        return SetStatusInformation(tr("Please input no"), -1, ERROR); 
    }
    
    if(ui->leName->text().isEmpty())
    {
        return SetStatusInformation(tr("Please input name"), -2, ERROR); 
    }
    
    if(m_pFace->GetDatabase()->GetTableRegister()->IsExistNo(ui->leNo->text().toLongLong()))
    {
        return SetStatusInformation(tr("The no is exists"), -3, ERROR); 
    }
    
    return 0;
}

int CFrmRegisterImage::SetStatusInformation(const QString &szInfo, int nRet, STATUS_TYPE type)
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
