#include "dialog.h"
#include "ui_dialog.h"
#include "QDebug"
#include "QFileDialog"

QString getMethodName()
{
    return ("Тестовый метод");
}

Dialog::Dialog(QWidget *parent)
    : AbstractInterfaceDialog(parent),
      ui(new Ui::Dialog)
{
    ui->setupUi(this);
    AbstractInterfaceDialog::setWindowTitle(getMethodName());
    m_plugin_base_elements_widget = ui->base_elements_widget;
    m_plugin_base_elements_widget->set_interface_dialog(this);
    connect(m_plugin_base_elements_widget, &PluginBaseElementsWidget::accept, this, &Dialog::accept);
    m_chart_window = m_plugin_base_elements_widget->get_chart_window();
}

Dialog::~Dialog()
{
    delete ui;
}

QStringList Dialog::getUsingProperties()
{
    QStringList properties;
    properties.push_back("E");
    properties.push_back("nu");
    return properties;
}

void Dialog::updatePropertyValuesWidget()
{
    m_plugin_base_elements_widget->updatePropertyValuesWidget();
}

void Dialog::updateScalarDataWidget()
{
    m_plugin_base_elements_widget->updateScalarDataWidget();
}

void Dialog::updateDependencesWidget()
{
    m_plugin_base_elements_widget->updateDependencesWidget();
}

void Dialog::updateDependenceDataWidget()
{
    m_plugin_base_elements_widget->updateDependenceDataWidget();
}

PluginInterface::PluginInterface()
{
    m_dialog = qobject_cast<AbstractInterfaceDialog*>(new Dialog);
}

PluginInterface::~PluginInterface()
{

}

QString PluginInterface::getPluginName()
{
    return getMethodName();
}

AbstractInterfaceDialog *PluginInterface::getPluginDialog()
{
    return (m_dialog);
}
