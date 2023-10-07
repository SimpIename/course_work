#ifndef MYTABLEWIDGET_H
#define MYTABLEWIDGET_H

#include <QtWidgets>
#include <QtSql/QSqlQueryModel>

namespace Ui {
class MyTableWidget;
}

class MyTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyTableWidget(QWidget *parent = nullptr, const QString& p_tableName = "");
    ~MyTableWidget();
    QTableView* getView();
    QAbstractItemModel* getModel();
    QLabel* getLabel();
    QPoint dragStartPosition;
    void setText(const QString& p_text);
    QPushButton* getPlusButton();
    QPushButton* getColumnButton();
    QPushButton* getSelectButton();
    void setStyleSheet(const QString& p_qss);
    QStringList selected_parent_data;

private:
    Ui::MyTableWidget *ui;

signals:
    void update_child(QStringList selected_row);
};

inline void initialise_MyTableWidget(MyTableWidget* wdg, QString wdgText, QString plusHint, QString allHint, QString styleSheet = "background:#1dacd6")
{
    wdg->setText(wdgText);
    wdg->getPlusButton()->setToolTip(plusHint);
    wdg->getSelectButton()->setToolTip(allHint);
    wdg->getView()->setAlternatingRowColors(true);
    wdg->getView()->setSelectionMode(QAbstractItemView::ExtendedSelection);
    wdg->getView()->setDragEnabled(true);
    wdg->getView()->setAcceptDrops(true);
    wdg->getView()->setDropIndicatorShown(true);
    wdg->getView()->setDragDropMode(QAbstractItemView::DragDrop);
    wdg->getView()->setContextMenuPolicy(Qt::CustomContextMenu);
    wdg->getView()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    wdg->setStyleSheet("background:#1dacd6");
}

#endif // MYTABLEWIDGET_H
