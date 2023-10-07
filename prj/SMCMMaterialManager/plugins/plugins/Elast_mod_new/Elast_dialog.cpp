#include "Elast_dialog.h"
#include "ui_dialog.h"
#include "QDebug"
#include "QFileDialog"

#include "calculatorElasticModulus.h"

QString getMethodName()
{
    return ("Упругие характеристики полимерных матриц");
}

ElastDialog::ElastDialog(QWidget *parent)
    : AbstractInterfaceDialog(parent),
      ui(new Ui::Dialog)
{
    ui->setupUi(this);
    AbstractInterfaceDialog::setWindowTitle(getMethodName());
    m_plugin_base_elements_widget = ui->base_elements_widget;
    m_plugin_base_elements_widget->set_interface_dialog(this);
    connect(m_plugin_base_elements_widget, &PluginBaseElementsWidget::accept, this, &ElastDialog::accept);
    m_chart_window = m_plugin_base_elements_widget->get_chart_window();
}

ElastDialog::~ElastDialog()
{
    delete ui;
}

QStringList ElastDialog::getUsingProperties()
{
    QStringList properties;
    properties.push_back("E");
    properties.push_back("reversibility_coefficient");
    properties.push_back("density_binder");
    properties.push_back("gasification_coefficient");
    properties.push_back("velocity_volume_ablation_binder");
    properties.push_back("ea_div_r");
    properties.push_back("ratio_of_elastic_modulus");
    return properties;
}

void ElastDialog::updatePropertyValuesWidget()
{
    m_plugin_base_elements_widget->updatePropertyValuesWidget();
}

void ElastDialog::updateScalarDataWidget()
{
    m_plugin_base_elements_widget->updateScalarDataWidget();
}

void ElastDialog::updateDependencesWidget()
{
    m_plugin_base_elements_widget->updateDependencesWidget();
}

void ElastDialog::updateDependenceDataWidget()
{
    m_plugin_base_elements_widget->updateDependenceDataWidget();
}

PluginInterface::PluginInterface()
{
    m_dialog = qobject_cast<AbstractInterfaceDialog*>(new ElastDialog);
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

void ElastDialog::on_calculatorE_clicked()
{
    int N_e = m_experimental_scalars.value("N");

    double *delta_Theta_e;// Изменение температуры
    double *Elastic_modulus_experiment_e;// Экспериментальные данные модуля упругости
    double *density_experiment_e;//Плотность матрицы (ро_m)

    double porosity_0_e = m_experimental_scalars.value("porosity_0");
    double heating_rate_e = m_experimental_scalars.value("heating_rate");
    double density_experiment_0_e = m_experimental_scalars.value("density_experiment_0");
    double density_pyrol_phase_e = m_experimental_scalars.value("density_pyrol_phase");
    delta_Theta_e = new double[N_e];
    Elastic_modulus_experiment_e = new double[N_e];
    density_experiment_e = new double[N_e];

    for (int i = 0; i < N_e; i++) {
        delta_Theta_e[i] = m_experimental_tables.value("teta,E,po/po^0").values[i][0];
        Elastic_modulus_experiment_e[i] = m_experimental_tables.value("teta,E,po/po^0").values[i][1];
        density_experiment_e[i] = m_experimental_tables.value("teta,E,po/po^0").values[i][2];
    }

    //Вычисление
    calculatorElasticModulus test (N_e, porosity_0_e, heating_rate_e, density_experiment_0_e, delta_Theta_e, Elastic_modulus_experiment_e, density_experiment_e, density_pyrol_phase_e);

    QVector<double> ElasticModulus_test = test.calculationElasticModulus();
    QVector<double> density_test = test.calculationDensity();

    if (m_material_properties.contains("E"))
    {
        MaterialProperty &property = m_material_properties["E"];
        property.min = test.get_E();
        property.max = test.get_E();
        property.steps = 1;
    }else{
        MaterialProperty &property = m_material_properties["E"];
        property.min = test.get_E();
        property.max = test.get_E();
        property.steps = 1;
        m_material_properties.insert("E",property);
    }

    if (m_material_properties.contains("reversibility_coefficient"))
    {
        MaterialProperty &property = m_material_properties["reversibility_coefficient"];
        property.min = test.get_reversibility_coefficient();
        property.max = test.get_reversibility_coefficient();
        property.steps = 1;
    }else{
        MaterialProperty &property = m_material_properties["reversibility_coefficient"];
        property.min = test.get_reversibility_coefficient();
        property.max = test.get_reversibility_coefficient();
        property.steps = 1;
        m_material_properties.insert("reversibility_coefficient",property);
    }

    if (m_material_properties.contains("density_binder"))
    {
        MaterialProperty &property = m_material_properties["density_binder"];
        property.min = test.get_density_binder();
        property.max = test.get_density_binder();
        property.steps = 1;
    }else{
        MaterialProperty &property = m_material_properties["density_binder"];
        property.min = test.get_density_binder();
        property.max = test.get_density_binder();
        property.steps = 1;
        m_material_properties.insert("density_binder",property);
    }
    if (m_material_properties.contains("gasification_coefficient"))
    {
        MaterialProperty &property = m_material_properties["gasification_coefficient"];
        property.min = test.get_gasification_coefficient();
        property.max = test.get_gasification_coefficient();
        property.steps = 1;
    }else{
        MaterialProperty &property = m_material_properties["gasification_coefficient"];
        property.min = test.get_gasification_coefficient();
        property.max = test.get_gasification_coefficient();
        property.steps = 1;
        m_material_properties.insert("gasification_coefficient",property);
    }

    if (m_material_properties.contains("velocity_volume_ablation_binder"))
    {
        MaterialProperty &property = m_material_properties["velocity_volume_ablation_binder"];
        property.min = test.get_velocity_volume_ablation_binder();
        property.max = test.get_velocity_volume_ablation_binder();
        property.steps = 1;
    }else{
        MaterialProperty &property = m_material_properties["velocity_volume_ablation_binder"];
        property.min = test.get_velocity_volume_ablation_binder();
        property.max = test.get_velocity_volume_ablation_binder();
        property.steps = 1;
        m_material_properties.insert("velocity_volume_ablation_binder",property);
    }
    if (m_material_properties.contains("ea_div_r"))
    {
        MaterialProperty &property = m_material_properties["ea_div_r"];
        property.min = test.get_ea_div_r();
        property.max = test.get_ea_div_r();
        property.steps = 1;
    }else{
        MaterialProperty &property = m_material_properties["ea_div_r"];
        property.min = test.get_ea_div_r();
        property.max = test.get_ea_div_r();
        property.steps = 1;
        m_material_properties.insert("ea_div_r",property);
    }
    if (m_material_properties.contains("ratio_of_elastic_modulus"))
    {
        MaterialProperty &property = m_material_properties["ratio_of_elastic_modulus"];
        property.min = test.get_ratio_of_elastic_modulus();
        property.max = test.get_ratio_of_elastic_modulus();
        property.steps = 1;
    }else{
        MaterialProperty &property = m_material_properties["ratio_of_elastic_modulus"];
        property.min = test.get_ratio_of_elastic_modulus();
        property.max = test.get_ratio_of_elastic_modulus();
        property.steps = 1;
        m_material_properties.insert("ratio_of_elastic_modulus",property);
    }
    updatePropertyValuesWidget();
}
