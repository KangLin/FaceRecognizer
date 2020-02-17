#ifndef FRMREGISTERIMAGE_H
#define FRMREGISTERIMAGE_H

#include <QWidget>

#include "FactoryFace.h"
#include "ParameterRegisterImage.h"

namespace Ui {
class CFrmRegisterImage;
}

class CFrmRegisterImage : public QWidget
{
    Q_OBJECT

public:
    explicit CFrmRegisterImage(QWidget *parent = nullptr);
    ~CFrmRegisterImage();

Q_SIGNALS:
    void sigFinish();

public slots:
    void slotParameter(const CParameterRegisterImage &para);

private slots:
    void on_pbBrower_clicked();
    void on_pbRegister_clicked();
    void on_pbCancel_clicked();
    void on_pbReplace_clicked();

private:
    int ProcessImage(const QImage &image);
    int MarkFace(QImage &image);
    int ShowReplaceUI(bool bReplace = true);
    int Check();
    enum STATUS_TYPE
    {
        NORMAL,
        ERROR,
        FAIL,
        SUCCESS
    };
    int SetStatusInformation(const QString &szInfo, int nRet = 0, STATUS_TYPE type = NORMAL);

private:
    Ui::CFrmRegisterImage *ui;

    CFactoryFace* m_pFace;
    QImage m_Image;
    bool m_bRegister;
    bool m_bReplace;
};

#endif // FRMREGISTERIMAGE_H
