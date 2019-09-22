#include "FrmDisplay.h"
#include "ui_FrmDisplay.h"
#include <QPainter>
#include <QDebug>

CFrmDisplay::CFrmDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmDisplay)
{
    ui->setupUi(this);
}

CFrmDisplay::~CFrmDisplay()
{
    delete ui;
}

int CFrmDisplay::SetCameraAngle(int rotation)
{
    m_Rotation = rotation;
    return 0;
}

void CFrmDisplay::slotDisplay(const QImage &frame)
{
    m_Image = frame;
    update();
}

void CFrmDisplay::slotDisplay(const QVideoFrame& frame)
{
    qDebug() << "CFrmDisplay::slotDisplay(const QVideoFrame& frame)";
    QVideoFrame videoFrame = frame;
    if(!videoFrame.isValid())
        return;
    if(!videoFrame.map(QAbstractVideoBuffer::ReadOnly))
        return;
    do{
        QImage::Format f = QVideoFrame::imageFormatFromPixelFormat(
                    videoFrame.pixelFormat());
        if(QImage::Format_Invalid == f)
            break;
        QImage image(videoFrame.bits(),
                     videoFrame.width(),
                     videoFrame.height(),
                     videoFrame.width() << 2,
                     f);
        if(m_Rotation)
            image = image.transformed(QTransform().rotate(m_Rotation));

        m_Image = image;
    }while(0);
    videoFrame.unmap();
    
    update();
}

void CFrmDisplay::paintEvent(QPaintEvent *event)
{
    if(this->isHidden())
        return;
    QPainter painter(this);
    painter.drawImage(this->rect(), m_Image);
}

//参见： https://blog.csdn.net/junzia/article/details/76315120
void CFrmDisplay::YUV420_2_RGB(unsigned char* pYUV, unsigned char* pRGB, int width, int height)
{
	//找到Y、U、V在内存中的首地址  
	unsigned char* pY = pYUV;
	unsigned char* pU = pYUV + height * width;
	unsigned char* pV = pU + (height * width / 4);
 
 
	unsigned char* pBGR = nullptr;
	unsigned char R = 0;
	unsigned char G = 0;
	unsigned char B = 0;
	unsigned char Y = 0;
	unsigned char U = 0;
	unsigned char V = 0;
	double temp = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//找到相应的RGB首地址  
			pBGR = pRGB + i * width * 3 + j * 3;
 
			//取Y、U、V的数据值  
			Y = *(pY + i*width + j);
			U = *pU;
			V = *pV;
 
			//yuv转rgb公式  
			//yuv转rgb公式  
			temp = Y + ((1.773) * (U - 128));
			B = temp<0 ? 0 : (temp>255 ? 255 : static_cast<unsigned char>(temp));
 
			temp = (Y - (0.344) * (U - 128) - (0.714) * (V - 128));
			G = temp<0 ? 0 : (temp>255 ? 255 : static_cast<unsigned char>(temp));
 
			temp = (Y + (1.403)*(V - 128));
			R = temp<0 ? 0 : (temp>255 ? 255 : static_cast<unsigned char>(temp));
 
			//将转化后的rgb保存在rgb内存中，注意放入的顺序b是最低位  
			*pBGR = B;
			*(pBGR + 1) = G;
			*(pBGR + 2) = R;
 
 
			if (j % 2 != 0)
			{
				*pU++;
				*pV++;
			}
 
		}
		if (i % 2 == 0)
		{
			pU = pU - width / 2;
			pV = pV - width / 2;
		}
	}
}
