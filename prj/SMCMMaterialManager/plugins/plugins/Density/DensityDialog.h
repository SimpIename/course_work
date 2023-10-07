#ifndef DENSITYDIALOG_H
#define DENSITYDIALOG_H

#include <QDialog>
#include "interface.h"
#include <QVector>
#include <QMap>
#include "PluginBaseElementsWidget/PluginBaseElementsWidget.h"

/// Заменить на реальное название
QString getMethodName();

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class DensityDialog : public AbstractInterfaceDialog
{
    Q_OBJECT

public:
    DensityDialog(QWidget *parent = nullptr);
    ~DensityDialog();

    QStringList getUsingProperties();

private slots:

    void updatePropertyValuesWidget();

    void on_calculatorE_clicked();

private:
    Ui::Dialog *ui;
    PluginBaseElementsWidget *m_plugin_base_elements_widget;
};

class DensityPluginInterface : public AbstractInterface
{
    Q_OBJECT
    Q_INTERFACES(AbstractInterface)
    Q_PLUGIN_METADATA(IID "SMCM_MATERIAL_MANAGER_PLUGIN" FILE "Interface.json")
public:
    explicit DensityPluginInterface();
    ~DensityPluginInterface();
    QString getPluginName();
    AbstractInterfaceDialog *getPluginDialog();
};

#endif // DENSITYDIALOG_H
