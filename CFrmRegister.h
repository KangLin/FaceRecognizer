#ifndef CFRMREGISTER_H
#define CFRMREGISTER_H

#include <QWidget>

namespace Ui {
class CFrmRegister;
}

class CFrmRegister : public QWidget
{
    Q_OBJECT
    
public:
    explicit CFrmRegister(QWidget *parent = nullptr);
    ~CFrmRegister();
    
private:
    Ui::CFrmRegister *ui;
};

#endif // CFRMREGISTER_H
