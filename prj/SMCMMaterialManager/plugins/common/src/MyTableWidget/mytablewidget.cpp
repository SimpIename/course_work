#include "MyTableWidget/mytablewidget.h"
#include "ui_mytablewidget.h"
#include <QIcon>
#include <QPixmap>

MyTableWidget::MyTableWidget(QWidget *parent, const QString& p_tableName) :
    QWidget(parent),
    ui(new Ui::MyTableWidget)
{
    ui->setupUi(this);
    ui->label->setText(p_tableName);
    connect(ui->pushButton_2,SIGNAL(clicked()),ui->tableView, SLOT(selectAll()));
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSortingEnabled(true);
    QStandardItemModel *model = new QStandardItemModel(ui->tableView);
    ui->tableView->setModel(model);
    ui->pushButton_3->hide();
    ui->pushButton_3->setIcon(QIcon(":/resources/columnPlus.png"));
}

QTableView* MyTableWidget::getView(){
    return ui->tableView;
}
QAbstractItemModel* MyTableWidget::getModel(){
    return ui->tableView->model();
}

QLabel* MyTableWidget::getLabel(){
    return  ui->label;
}

void MyTableWidget::setText(const QString& p_text){
    ui->label->setText(p_text);
}

QPushButton* MyTableWidget::getPlusButton(){
    return ui->pushButton;
}

QPushButton *MyTableWidget::getColumnButton()
{
    return ui->pushButton_3;
}

QPushButton *MyTableWidget::getSelectButton()
{
    return ui->pushButton_2;
}

void MyTableWidget::setStyleSheet(const QString& p_qss){
    ui->label->setStyleSheet(p_qss);
    ui->pushButton->setStyleSheet(p_qss);
    ui->pushButton_2->setStyleSheet(p_qss);
}

MyTableWidget::~MyTableWidget()
{
    delete ui;
}

