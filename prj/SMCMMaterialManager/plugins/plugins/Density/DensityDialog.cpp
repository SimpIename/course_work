#include "DensityDialog.h"
#include "ui_DensityDialog.h"
#include "QDebug"
#include "QFileDialog"
#include "calculatorDensity.h"

QString getMethodName()
{
    return ("Характеристики плотности полимерных матриц");
}

DensityDialog::DensityDialog(QWidget *parent)
    : AbstractInterfaceDialog(parent),
      ui(new Ui::Dialog)
{
    ui->setupUi(this);
    AbstractInterfaceDialog::setWindowTitle(getMethodName());
    m_plugin_base_elements_widget = ui->base_elements_widget;
    m_plugin_base_elements_widget->set_interface_dialog(this);
    connect(m_plugin_base_elements_widget, &PluginBaseElementsWidget::accept, this, &DensityDialog::accept);
}

DensityDialog::~DensityDialog()
{
    delete ui;
}

QStringList DensityDialog::getUsingProperties()
{
    QStringList properties;
    properties.push_back("density_binder");
    properties.push_back("gasification_coefficient");
    properties.push_back("velocity_volume_ablation_binder");
    properties.push_back("ea_div_r");
    return properties;
}

DensityPluginInterface::DensityPluginInterface()
{
    m_dialog = qobject_cast<AbstractInterfaceDialog*>(new DensityDialog);
}

DensityPluginInterface::~DensityPluginInterface()
{

}

QString DensityPluginInterface::getPluginName()
{
    return getMethodName();
}

AbstractInterfaceDialog *DensityPluginInterface::getPluginDialog()
{
    return (m_dialog);
}

void DensityDialog::updatePropertyValuesWidget()
{
    m_plugin_base_elements_widget->updatePropertyValuesWidget();
}

void DensityDialog::on_calculatorE_clicked()
{
    qDebug() << "calculator";

    int N_e = m_experimental_scalars.value("N");

    double *delta_Theta_e;// Изменение температуры
    double *density_experiment_e;//Плотность матрицы (ро_m)

    double porosity_0_e = m_experimental_scalars.value("porosity_0");
    double heating_rate_e = m_experimental_scalars.value("heating_rate");
    double density_experiment_0_e = m_experimental_scalars.value("density_experiment_0");
    double density_pyrol_phase_e = m_experimental_scalars.value("density_pyrol_phase");
    delta_Theta_e = new double[N_e];
    density_experiment_e = new double[N_e];

    for (int i = 0; i < N_e; i++) {
        delta_Theta_e[i] = m_experimental_tables.value("teta,po/po^0").values[i][0];
        density_experiment_e[i] = m_experimental_tables.value("teta,po/po^0").values[i][1];
    }

    //Вычисление
    calculatorDensity test (N_e, porosity_0_e, heating_rate_e, density_experiment_0_e, delta_Theta_e, density_experiment_e, density_pyrol_phase_e);

    QVector<double> density_test = test.calculationDensity();



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
    updatePropertyValuesWidget();
}
