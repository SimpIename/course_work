#ifndef PLUGINADDDIALOG_H
#define PLUGINADDDIALOG_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class pluginAddDialog;
}

class pluginAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit pluginAddDialog(QWidget *parent = nullptr);
    ~pluginAddDialog();
    QLineEdit* getLineEdit();

private:
    Ui::pluginAddDialog *ui;
};

#endif // PLUGINADDDIALOG_H
