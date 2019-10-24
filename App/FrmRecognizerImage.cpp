#include "FrmRecognizerImage.h"
#include "ui_FrmRecognizerImage.h"
#include "RabbitCommonDir.h"
#include "Factory.h"

#include <QPainter>
#include <QStandardItemModel>
#include <QStandardItem>

CFrmRecognizerImage::CFrmRecognizerImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmRecognizerImage)
{
    ui->setupUi(this);
    QStandardItemModel *model = new QStandardItemModel(this);
    ui->tvInformation->setModel(model);
    ui->tvInformation->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvInformation->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvInformation->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvInformation->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvInformation->verticalHeader()->setVisible(false);
    model->setHorizontalHeaderLabels(QStringList()
                                     << tr("index")
                                     << tr("no")
                                     << tr("name"));
    /*
    model->setHorizontalHeaderItem(0, new QStandardItem(tr("index")));
    model->setHorizontalHeaderItem(1, new QStandardItem(tr("no")));
    model->setHorizontalHeaderItem(2, new QStandardItem(tr("name")));
    */
    
    m_pFace = CFactory::Instance();
    ShowUI(false);
    SetStatusInformation(tr("Please select image"));
}

CFrmRecognizerImage::~CFrmRecognizerImage()
{
    delete ui;
}

void CFrmRecognizerImage::on_pbBrower_clicked()
{
    QString szFile = RabbitCommon::CDir::GetOpenFileName(this,
                                                         tr("Select image"),
                                                         ui->leFile->text(),
           tr("Image files(*.png *.gif *.jpeg *.jpg *.bmp);; All files(*.*)"));
    if(szFile.isEmpty())
        return;
    
    ShowUI(false);

    ui->leFile->setText(szFile);

    QImage image(szFile);
    RecognizeFace(image);
    MarkFace(image);
    ui->lbImage->setPixmap(QPixmap::fromImage(image));
}

int CFrmRecognizerImage::SetStatusInformation(const QString &szInfo, int nRet, STATUS_TYPE type)
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

int CFrmRecognizerImage::ShowUI(bool bShow)
{
    if(bShow)
    {
        ui->lbOldImage->setVisible(true);
        ui->tvInformation->setVisible(true);
        if(m_Info.size() > 1)
        {
            ui->pbNext->setVisible(true);
            ui->pbPrevious->setVisible(true);
        }
    } else {
        ui->lbOldImage->setVisible(false);
        ui->tvInformation->setVisible(false);
        ui->pbNext->setVisible(false);
        ui->pbPrevious->setVisible(false);
    }
    return 0;
}

int CFrmRecognizerImage::MarkFace(QImage &image)
{
    QPainter painter(&image);
    QPen pen(Qt::green);
    pen.setWidth(2);
    painter.setPen(pen);    
    QVector<FACE_INFO>::iterator it;
    for(it = m_Info.begin(); it != m_Info.end(); it++)
    {
        painter.drawRect(it->face.x(), it->face.y(), it->face.width(), it->face.height());
        if(!it->data.getName().isEmpty())
            painter.drawText(it->face.x(), it->face.y(), it->data.getName());
        for (auto &point: it->points)
        {
            painter.drawPoint(point.x(), point.y());
        }
    }
    return 0;
}

int CFrmRecognizerImage::RecognizeFace(QImage &image)
{
    if(!m_pFace) return -1;

    m_Info.clear();
    
    QStandardItemModel* pModel = dynamic_cast<QStandardItemModel*>(ui->tvInformation->model());
    pModel->removeRows(0, pModel->rowCount());
    
    auto faces = m_pFace->GetDector()->Detect(image);

    foreach (auto f, faces) {
        FACE_INFO info;
        info.face = f;
        info.points = m_pFace->GetLandmarker()->Mark(image, f);
        info.index = m_pFace->GetRecognizer()->Query(image, info.points);
        if(info.index < 0)
            continue;
        m_pFace->GetDatabase()->GetTableRegister()->GetRegisterInfo(info.index, &info.data);
        m_Info.push_back(info);
        
        QStandardItemModel* pModel = dynamic_cast<QStandardItemModel*>(ui->tvInformation->model());
        
        QList<QStandardItem*> items;
        QStandardItem *item = new QStandardItem(QString::number(info.index));
        items.push_back(item);
        item = new QStandardItem(QString::number(info.data.getNo()));
        items.push_back(item);
        item = new QStandardItem(info.data.getName());
        items.push_back(item);
        pModel->appendRow(items);
    }
    
    if(m_Info.size() > 0)
    {
        QModelIndex index = ui->tvInformation->model()->index(0, 0);
        ui->tvInformation->clicked(index);
        ShowUI(true);
        SetStatusInformation(tr("Recognizer %1 faces").arg(m_Info.size()));
    } else 
        SetStatusInformation(tr("Please select image"));
    return 0;
}

void CFrmRecognizerImage::on_tvInformation_clicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    ui->lbOldImage->setPixmap(QPixmap::fromImage(
                  QImage(m_pFace->GetRecognizer()->GetRegisterImage(
                                 m_Info[index.row()].index))));
}
