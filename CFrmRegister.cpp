#include "CFrmRegister.h"
#include "ui_CFrmRegister.h"

CFrmRegister::CFrmRegister(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmRegister)
{
    ui->setupUi(this);
}

CFrmRegister::~CFrmRegister()
{
    delete ui;
}
