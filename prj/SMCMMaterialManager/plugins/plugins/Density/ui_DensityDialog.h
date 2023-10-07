/********************************************************************************
** Form generated from reading UI file 'DensityDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DENSITYDIALOG_H
#define UI_DENSITYDIALOG_H

#include <PluginBaseElementsWidget/PluginBaseElementsWidget.h>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout;
    QPushButton *calculatorE;
    PluginBaseElementsWidget *base_elements_widget;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(1086, 650);
        gridLayout = new QGridLayout(Dialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        calculatorE = new QPushButton(Dialog);
        calculatorE->setObjectName(QString::fromUtf8("calculatorE"));

        gridLayout->addWidget(calculatorE, 1, 0, 1, 1);

        base_elements_widget = new PluginBaseElementsWidget(Dialog);
        base_elements_widget->setObjectName(QString::fromUtf8("base_elements_widget"));

        gridLayout->addWidget(base_elements_widget, 0, 0, 1, 1);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        calculatorE->setText(QCoreApplication::translate("Dialog", "\320\222\321\213\321\207\320\270\321\201\320\273\320\270\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DENSITYDIALOG_H
