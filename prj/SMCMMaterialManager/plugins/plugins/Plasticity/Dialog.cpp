#include "Dialog.h"
#include "ui_Dialog.h"
#include "QDebug"
#include "QFileDialog"

QString getMethodName()
{
    return ("Аппроксимация диаграмм пластичности");
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
    properties.push_back("omega0");
    properties.push_back("b");
    properties.push_back("epsilon_s");
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

void Dialog::on_calculatorE_clicked()
{
    auto table = m_experimental_tables.value("Strain,Stress").values;

    std::vector<double> strains;
    strains.reserve(table.size());
    std::vector<double> stresses;
    stresses.reserve(table.size());

    bool isTension = ui->radioButton->isChecked();

    double E, nu;

    if (m_experimental_scalars.contains("E")) {
        E = m_experimental_scalars["E"];
    } else {
        E = m_material_properties["E"].min;
    }

    if (m_experimental_scalars.contains("nu")) {
        nu = m_experimental_scalars["nu"];
    } else {
        nu = m_material_properties["nu"].min;
    }

    for (auto &row : table) {
        strains.push_back(row[0]);
        stresses.push_back(row[1]);
    }

           // Предполагается, что в таблице были указаны деформации XX и напряжения XX
           // при нагружении в напряжениях, т.е. материал ещё деформировался по YY и по ZZ,
           // а все напряжения, кроме XX равны 0.
           // Для аппроксимации нужны вторые инварианты деформаций и напряжений.
           // Инвариант напряжений получается равным по модулю значению напряжения,
           // а инвариант деформаций вычисляется по выведенной формуле, если известны E и nu материала.
    if (isTension) {
        for (int i = 0; i < strains.size(); ++i) {
            double E1 = stresses[i] / strains[i];
            strains[i] = fabs(strains[i] * ((2 * nu - 1) * E1 + 3 * E) / E) / 2.;
            stresses[i] = fabs(stresses[i]);
        }
    } else {
        for (int i = 0; i < strains.size(); ++i) {
            strains[i] = sqrt(3) * fabs(strains[i]);
            stresses[i] = sqrt(3) * fabs(stresses[i]);
        }
    }

    PlasticityAproximator func(strains, stresses);

    func.prepLSMCycle();
    double arr_omega0;
    double arr_b;
    double arr_epsilon_s;
    arr_omega0 = func.new_omega0;
    arr_b = func.new_b;
    arr_epsilon_s = func.new_epsilon_s;

    {
        MaterialProperty &property = m_material_properties["omega0"];
        property.min = arr_omega0;
        property.max = arr_omega0;
        property.steps = 1;
    }
    {
        MaterialProperty &property = m_material_properties["b"];
        property.min = arr_b;
        property.max = arr_b;
        property.steps = 1;
    }
    {
        MaterialProperty &property = m_material_properties["epsilon_s"];
        property.min = arr_epsilon_s;
        property.max = arr_epsilon_s;
        property.steps = 1;
    }
    {
        MaterialProperty &property = m_material_properties["E"];
        property.min = E;
        property.max = E;
        property.steps = 1;
    }
    {
        MaterialProperty &property = m_material_properties["nu"];
        property.min = nu;
        property.max = nu;
        property.steps = 1;
    }
    m_chart_window->clear();
    {
        charts::Table o_table;

        charts::Column strains_column("Strain", QList<Real>(strains.size()+1));
        charts::Column stresses_column("Stress", QList<Real>(strains.size()+1));
        strains_column.rows[0] = 0;
        stresses_column.rows[0] = 0;
        for (unsigned int i = 0; i < strains.size(); ++i) {
            strains_column.rows[i+1] = strains[i];
            stresses_column.rows[i+1] = stresses[i];
        }
        o_table.columns.push_back(strains_column);
        o_table.columns.push_back(stresses_column);
        m_chart_window->addData(o_table);
        m_chart_window->addCoordinateColumns(0, 1);
    }
    {
        charts::Table o_table;
        charts::Column strains_column("Strain", QList<Real>(strains.size()+1));
        charts::Column stresses_column("Stress", QList<Real>(strains.size()+1));
        strains_column.rows[0] = 0;
        stresses_column.rows[0] = 0;
        for (unsigned int i = 0; i < strains.size(); ++i) {
            strains_column.rows[i+1] = strains[i];
            stresses_column.rows[i+1] = func.comp(strains[i]);
        }
        o_table.columns.push_back(strains_column);
        o_table.columns.push_back(stresses_column);
        m_chart_window->addData(o_table);
        m_chart_window->addCoordinateColumns(0, 1);
    }
    updatePropertyValuesWidget();
}

