/********************************************************************************
** Form generated from reading UI file 'PluginBaseElementsWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLUGINBASEELEMENTSWIDGET_H
#define UI_PLUGINBASEELEMENTSWIDGET_H

#include <MyTableWidget/mytablewidget.h>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PluginBaseElementsWidget
{
public:
    QGridLayout *gridLayout;
    MyTableWidget *propertyValuesWidget;
    MyTableWidget *scalarDataWidget;
    MyTableWidget *dependencesWidget;
    MyTableWidget *dependenceDataWidget;
    QPushButton *loadTextButton;
    QPushButton *saveTextButton;
    QPushButton *acceptButton;
    QWidget *chartWidget;

    void setupUi(QWidget *PluginBaseElementsWidget)
    {
        if (PluginBaseElementsWidget->objectName().isEmpty())
            PluginBaseElementsWidget->setObjectName(QString::fromUtf8("PluginBaseElementsWidget"));
        PluginBaseElementsWidget->resize(1099, 663);
        gridLayout = new QGridLayout(PluginBaseElementsWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        propertyValuesWidget = new MyTableWidget(PluginBaseElementsWidget);
        propertyValuesWidget->setObjectName(QString::fromUtf8("propertyValuesWidget"));
        propertyValuesWidget->setMinimumSize(QSize(350, 0));

        gridLayout->addWidget(propertyValuesWidget, 0, 0, 1, 1);

        scalarDataWidget = new MyTableWidget(PluginBaseElementsWidget);
        scalarDataWidget->setObjectName(QString::fromUtf8("scalarDataWidget"));
        scalarDataWidget->setMinimumSize(QSize(350, 0));

        gridLayout->addWidget(scalarDataWidget, 1, 0, 1, 1);

        dependencesWidget = new MyTableWidget(PluginBaseElementsWidget);
        dependencesWidget->setObjectName(QString::fromUtf8("dependencesWidget"));
        dependencesWidget->setMinimumSize(QSize(350, 0));

        gridLayout->addWidget(dependencesWidget, 1, 1, 1, 1);

        dependenceDataWidget = new MyTableWidget(PluginBaseElementsWidget);
        dependenceDataWidget->setObjectName(QString::fromUtf8("dependenceDataWidget"));
        dependenceDataWidget->setMinimumSize(QSize(350, 0));

        gridLayout->addWidget(dependenceDataWidget, 1, 2, 1, 1);

        loadTextButton = new QPushButton(PluginBaseElementsWidget);
        loadTextButton->setObjectName(QString::fromUtf8("loadTextButton"));

        gridLayout->addWidget(loadTextButton, 2, 0, 1, 1);

        saveTextButton = new QPushButton(PluginBaseElementsWidget);
        saveTextButton->setObjectName(QString::fromUtf8("saveTextButton"));

        gridLayout->addWidget(saveTextButton, 2, 1, 1, 1);

        acceptButton = new QPushButton(PluginBaseElementsWidget);
        acceptButton->setObjectName(QString::fromUtf8("acceptButton"));

        gridLayout->addWidget(acceptButton, 2, 2, 1, 1);

        chartWidget = new QWidget(PluginBaseElementsWidget);
        chartWidget->setObjectName(QString::fromUtf8("chartWidget"));

        gridLayout->addWidget(chartWidget, 0, 1, 1, 2);


        retranslateUi(PluginBaseElementsWidget);

        QMetaObject::connectSlotsByName(PluginBaseElementsWidget);
    } // setupUi

    void retranslateUi(QWidget *PluginBaseElementsWidget)
    {
        PluginBaseElementsWidget->setWindowTitle(QCoreApplication::translate("PluginBaseElementsWidget", "Form", nullptr));
        loadTextButton->setText(QCoreApplication::translate("PluginBaseElementsWidget", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214 \320\264\320\260\320\275\320\275\321\213\320\265", nullptr));
        saveTextButton->setText(QCoreApplication::translate("PluginBaseElementsWidget", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\264\320\260\320\275\320\275\321\213\320\265", nullptr));
        acceptButton->setText(QCoreApplication::translate("PluginBaseElementsWidget", "\320\237\321\200\320\270\320\274\320\265\320\275\320\270\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PluginBaseElementsWidget: public Ui_PluginBaseElementsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLUGINBASEELEMENTSWIDGET_H
