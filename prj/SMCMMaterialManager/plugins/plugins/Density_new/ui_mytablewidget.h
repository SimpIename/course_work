/********************************************************************************
** Form generated from reading UI file 'mytablewidget.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYTABLEWIDGET_H
#define UI_MYTABLEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyTableWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;
    QLabel *label;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton;
    QTableView *tableView;

    void setupUi(QWidget *MyTableWidget)
    {
        if (MyTableWidget->objectName().isEmpty())
            MyTableWidget->setObjectName(QString::fromUtf8("MyTableWidget"));
        MyTableWidget->resize(458, 302);
        horizontalLayout = new QHBoxLayout(MyTableWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        vLayout = new QVBoxLayout();
        vLayout->setSpacing(0);
        vLayout->setObjectName(QString::fromUtf8("vLayout"));
        vLayout->setContentsMargins(-1, -1, -1, 0);
        hLayout = new QHBoxLayout();
        hLayout->setSpacing(0);
        hLayout->setObjectName(QString::fromUtf8("hLayout"));
        label = new QLabel(MyTableWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setEnabled(true);
        label->setAutoFillBackground(false);
        label->setScaledContents(true);
        label->setAlignment(Qt::AlignCenter);

        hLayout->addWidget(label);

        pushButton_2 = new QPushButton(MyTableWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy);
        pushButton_2->setMaximumSize(QSize(50, 16777215));
        pushButton_2->setAutoFillBackground(true);
        pushButton_2->setAutoDefault(true);

        hLayout->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(MyTableWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setMaximumSize(QSize(50, 16777215));

        hLayout->addWidget(pushButton_3);

        pushButton = new QPushButton(MyTableWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setMaximumSize(QSize(50, 16777215));
        pushButton->setAutoFillBackground(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/Plus.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon);
        pushButton->setCheckable(false);
        pushButton->setAutoDefault(true);
        pushButton->setFlat(false);

        hLayout->addWidget(pushButton);


        vLayout->addLayout(hLayout);

        tableView = new QTableView(MyTableWidget);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setEnabled(true);
        tableView->setStyleSheet(QString::fromUtf8(""));

        vLayout->addWidget(tableView);


        horizontalLayout->addLayout(vLayout);


        retranslateUi(MyTableWidget);

        pushButton->setDefault(false);


        QMetaObject::connectSlotsByName(MyTableWidget);
    } // setupUi

    void retranslateUi(QWidget *MyTableWidget)
    {
        MyTableWidget->setWindowTitle(QCoreApplication::translate("MyTableWidget", "Form", nullptr));
        label->setText(QCoreApplication::translate("MyTableWidget", "text", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MyTableWidget", "\320\222\321\201\320\265", nullptr));
        pushButton_3->setText(QString());
        pushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MyTableWidget: public Ui_MyTableWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYTABLEWIDGET_H
