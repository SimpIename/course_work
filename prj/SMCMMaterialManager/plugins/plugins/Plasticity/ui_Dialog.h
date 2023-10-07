/********************************************************************************
** Form generated from reading UI file 'Dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <PluginBaseElementsWidget/PluginBaseElementsWidget.h>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout;
    QPushButton *calculatorE;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    PluginBaseElementsWidget *base_elements_widget;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(1080, 650);
        gridLayout = new QGridLayout(Dialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        calculatorE = new QPushButton(Dialog);
        calculatorE->setObjectName(QString::fromUtf8("calculatorE"));

        gridLayout->addWidget(calculatorE, 1, 2, 1, 1);

        radioButton = new QRadioButton(Dialog);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        gridLayout->addWidget(radioButton, 1, 1, 1, 1);

        radioButton_2 = new QRadioButton(Dialog);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));
        radioButton_2->setEnabled(true);
        radioButton_2->setChecked(true);

        gridLayout->addWidget(radioButton_2, 1, 0, 1, 1);

        base_elements_widget = new PluginBaseElementsWidget(Dialog);
        base_elements_widget->setObjectName(QString::fromUtf8("base_elements_widget"));

        gridLayout->addWidget(base_elements_widget, 0, 0, 1, 3);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        calculatorE->setText(QCoreApplication::translate("Dialog", "\320\222\321\213\321\207\320\270\321\201\320\273\320\270\321\202\321\214", nullptr));
        radioButton->setText(QCoreApplication::translate("Dialog", "\320\241\320\264\320\262\320\270\320\263", nullptr));
        radioButton_2->setText(QCoreApplication::translate("Dialog", "\320\240\320\260\321\201\321\202\321\217\320\266\320\265\320\275\320\270\320\265", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
