#include "pluginadddialog.h"
#include "ui_pluginadddialog.h"

pluginAddDialog::pluginAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pluginAddDialog)
{
    ui->setupUi(this);
}

pluginAddDialog::~pluginAddDialog()
{
    delete ui;
}

QLineEdit *pluginAddDialog::getLineEdit()
{
    return ui->lineEdit;
}
