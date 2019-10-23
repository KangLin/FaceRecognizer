#ifndef FRMREGISTERIMAGE_H
#define FRMREGISTERIMAGE_H

#include <QWidget>

#include "Factory.h"

namespace Ui {
class CFrmRegisterImage;
}

class CFrmRegisterImage : public QWidget
{
    Q_OBJECT
    
public:
    explicit CFrmRegisterImage(QWidget *parent = nullptr);
    ~CFrmRegisterImage();
    
private slots:
    void on_pbBrower_clicked();
    void on_pbRegister_clicked();
    
    void on_pbCancel_clicked();
    
    void on_pbReplace_clicked();
    
private:
    int MarkFace(QImage &image);
    int ShowReplaceUI(bool bReplace = true);
    int Check();
    
private:
    Ui::CFrmRegisterImage *ui;
    
    CFace* m_pFace;
    QImage m_Image;
    bool m_bRegister;
    bool m_bReplace;
};

#endif // FRMREGISTERIMAGE_H
