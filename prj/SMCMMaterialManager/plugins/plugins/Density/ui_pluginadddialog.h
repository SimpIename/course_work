/********************************************************************************
** Form generated from reading UI file 'pluginadddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLUGINADDDIALOG_H
#define UI_PLUGINADDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_pluginAddDialog
{
public:
    QGridLayout *gridLayout;
    QLineEdit *lineEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *pluginAddDialog)
    {
        if (pluginAddDialog->objectName().isEmpty())
            pluginAddDialog->setObjectName(QString::fromUtf8("pluginAddDialog"));
        pluginAddDialog->resize(400, 73);
        pluginAddDialog->setMaximumSize(QSize(16777215, 73));
        gridLayout = new QGridLayout(pluginAddDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lineEdit = new QLineEdit(pluginAddDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(pluginAddDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 1);


        retranslateUi(pluginAddDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, pluginAddDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, pluginAddDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(pluginAddDialog);
    } // setupUi

    void retranslateUi(QDialog *pluginAddDialog)
    {
        pluginAddDialog->setWindowTitle(QCoreApplication::translate("pluginAddDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class pluginAddDialog: public Ui_pluginAddDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLUGINADDDIALOG_H
