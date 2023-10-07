#ifndef ELASTDIALOG_H
#define ELASTDIALOG_H

#include <QDialog>
#include "interface.h"
#include <QVector>
#include <QMap>
#include "PluginBaseElementsWidget/PluginBaseElementsWidget.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

/// Заменить на реальное название
QString getMethodName();

/// Добавить вызов расчетного метода
class ElastDialog : public AbstractInterfaceDialog
{
    Q_OBJECT

public:
    ElastDialog(QWidget *parent = nullptr);
    ~ElastDialog();

    QStringList getUsingProperties();


private slots:
    void updatePropertyValuesWidget();

    void on_calculatorE_clicked();


private:
    Ui::Dialog *ui;
    PluginBaseElementsWidget *m_plugin_base_elements_widget;
};

class ElastPluginInterface : public AbstractInterface
{
    Q_OBJECT
    Q_INTERFACES(AbstractInterface)
    Q_PLUGIN_METADATA(IID "SMCM_MATERIAL_MANAGER_PLUGIN" FILE "Interface.json")
public:
    explicit ElastPluginInterface();
    ~ElastPluginInterface();
    QString getPluginName();
    AbstractInterfaceDialog *getPluginDialog();
};

#endif // ELASTDIALOG_H
