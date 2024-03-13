/**
  @author: Kang Lin<kl222@126.com>
  */

#include "FrmRecognizerImage.h"
#include "ui_FrmRecognizerImage.h"
#include "RabbitCommonDir.h"
#include "FactoryFace.h"

#include <QPainter>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QPen>
#include <stdexcept>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(log, "App.Recognizer.Image")

CFrmRecognizerImage::CFrmRecognizerImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmRecognizerImage)
{
    ui->setupUi(this);
    QStandardItemModel *model = new QStandardItemModel(this);
    if(!model)
        throw std::runtime_error("CFrmRecognizerImage construct allocate memory fail"); 
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
    
    m_pFace = CFactoryFace::Instance();
    if(!m_pFace)
        throw std::runtime_error("CFrmRecognizerImage construct allocate memory fail");
    
    ShowUI(false);
    SetStatusInformation(tr("Please select image"));
}

CFrmRecognizerImage::~CFrmRecognizerImage()
{
    qDebug(log) << "CFrmRecognizerImage::~CFrmRecognizerImage()";
    delete ui;
}

void CFrmRecognizerImage::on_pbBrower_clicked()
{
    QString szFile = QFileDialog::getOpenFileName(this,
                                                         tr("Select image"),
                                                         ui->leFile->text(),
           tr("Image files(*.png *.gif *.jpeg *.jpg *.bmp);; All files(*.*)"));
    if(szFile.isEmpty())
        return;
    
    ShowUI(false);

    ui->leFile->setText(szFile);
    m_Image.load(szFile);
    RecognizeFace(m_Image);
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
        ui->frmOldImage->setVisible(true);
        ui->tvInformation->setVisible(true);
    } else {
        ui->frmOldImage->setVisible(false);
        ui->tvInformation->setVisible(false);
    }
    return 0;
}

int CFrmRecognizerImage::MarkFace(QImage &image, int nSelect)
{
    QPainter painter(&image);
    QPen pen(Qt::green);
    pen.setWidth(2);
    painter.setPen(pen);
    QVector<FACE_INFO>::iterator it;
    int i = 0;
    for(it = m_Info.begin(); it != m_Info.end(); it++)
    {
        if(-1 != nSelect)
        {
            if(i == nSelect)
                pen.setColor(Qt::red);
            else
                pen.setColor(Qt::green);
            painter.setPen(pen);
        }
        painter.drawRect(it->face.x(), it->face.y(), it->face.width(), it->face.height());
        if(!it->data.getName().isEmpty())
            painter.drawText(it->face.x(), it->face.y(), it->data.getName());
        i++;
    }
    ui->frmImage->slotDisplay(image);
    return 0;
}

int CFrmRecognizerImage::RecognizeFace(QImage &image)
{
    if(!m_pFace->bIsValid()) return -1;

    m_Info.clear();
    
    QStandardItemModel* pModel = dynamic_cast<QStandardItemModel*>(ui->tvInformation->model());
    if(!pModel) return -2;
    pModel->removeRows(0, pModel->rowCount());
    
    QVector<QRect> faces;
    m_pFace->GetDector()->Detect(image, faces);
    foreach (auto f, faces) {
        FACE_INFO info;
        info.face = f;
        info.index = m_pFace->GetRecognizer()->Query(image, f);
        if(info.index < 0)
            continue;
        m_pFace->GetDatabase()->GetTableRegister()->GetRegisterInfo(info.index, &info.data);
        m_Info.push_back(info);

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
        ShowUI(true);
        QModelIndex index = ui->tvInformation->model()->index(0, 0);
        ui->tvInformation->clicked(index);
        SetStatusInformation(tr("Recognizer %1 faces").arg(m_Info.size()));
    } else {
        MarkFace(m_Image);   
        SetStatusInformation(tr("Please select image"));
    }
    return 0;
}

void CFrmRecognizerImage::on_tvInformation_clicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    if(!m_pFace->bIsValid()) return;
    
    ui->tvInformation->selectRow(index.row());
    ui->frmOldImage->slotDisplay(
                        QImage(m_pFace->GetRecognizer()->GetRegisterImage(
                                             m_Info[index.row()].index)));
    MarkFace(m_Image, index.row());
}
