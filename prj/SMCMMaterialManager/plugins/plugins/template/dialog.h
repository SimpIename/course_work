#ifndef DIALOG_H
#define DIALOG_H

//#include <QDialog>
////#include "interface.h"
//#include "../../../../../src/Shared/plugin/interface.h"
//#include <QVector>
//#include <QMap>
//#include "PluginBaseElementsWidget/PluginBaseElementsWidget.h"

#include <QDialog>
#include "Shared/plugin/interface.h"
#include <QVector>
#include <QMap>

#include "PluginBaseElementsWidget/PluginBaseElementsWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

/// \brief название плагина, уникально для каждого модуля, заменить на своё
/// Заменить на реальное название
QString getMethodName();

/// Добавить вызов расчетного метода
class Dialog : public AbstractInterfaceDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

/// \brief описание списка характеристик, уникален для каждого модуля, заменить на свой
    QStringList getUsingProperties();

private slots:

    void updatePropertyValuesWidget();
    void updateScalarDataWidget();
    void updateDependencesWidget();
    void updateDependenceDataWidget();



private:
    Ui::Dialog *ui;
    PluginBaseElementsWidget *m_plugin_base_elements_widget;
    ChartWindow * m_chart_window;
};

class PluginInterface : public AbstractInterface
{
    Q_OBJECT
    Q_INTERFACES(AbstractInterface)
    Q_PLUGIN_METADATA(IID "SMCM_MATERIAL_MANAGER_PLUGIN" FILE "Interface.json")
public:
    explicit PluginInterface();
    ~PluginInterface();
    QString getPluginName();
    AbstractInterfaceDialog *getPluginDialog();
};

#endif // DIALOG_H
