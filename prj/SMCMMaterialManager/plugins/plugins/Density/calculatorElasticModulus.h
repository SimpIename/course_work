#ifndef CALCULATORELASTICMODULUS_H
#define CALCULATORELASTICMODULUS_H

#include<QVector>
class calculatorElasticModulus
{
private:
    const double P = 0.000156;
    const double Q = 0.00025;
    double temperature_0 = 272;
    //Данные, которые мы вводим
    int Num_points; // количество точек

    double *delta_Theta;// Изменение температуры
    double *Elastic_modulus_experiment;// Экспериментальные данные модуля упругости
    double *density_experiment;//Плотность матрицы (ро_m)

    double porosity_0;// Начальная пористость
    double heating_rate;// скорость нагрева
    double density_experiment_0;//Начальная плотность ро_m^0   [кг/м^3]
    double density_pyrol_phase;//Плотность твердой пиролитической фазы (ро_p)

    // Константы, которые необходимо вычислить
    double Elast_mod_polymer_phase;//Модуль упругости полимерной фазы
    double a_coeffElasticProperties;//Коэффициент изменения упругих свойств полимерной фазы

    double density_polymer_phase;//Плотность твердой полимерной фазы (ро_b)
    double gas_coef;//Коэффициент газификации (Г)
    double preExponental_factor;//Предэкспоненциальный множитель (J_0)
    double Energy_activThermDestruct;//Энергия активации процесса термодиструкции (E_A)

    double a_1_max;
    double a_0_max;

    double m_p_ElastModulsRatio;// Отношение Ep/Eb^0
public:
    calculatorElasticModulus();
    calculatorElasticModulus(int Num_points_e, double porosity_0_e, double heating_rate_e, double density_experiment_0_e, double *delta_Theta_e, double *Elastic_modulus_experiment_e, double* density_experiment_e, double density_pyrol_phase_e);


    int termalDestruction();//Критерий выбора области ДО начала термодиструкции (вычисляем индекс температуры начала ТД)

    private:
    double S_b_(double porosity);
    double S_p_(double vol_concentrat_pyrol_phase);
    double R_(double delta_Theta_temp);
    double F_(double porosity);
    double a_0_(double delta_Theta_temp);
    double a_1_(int j_);
    double Elastic_modulus(double delta_Theta_temp, double porosity);
    double Delta_(double x);

    void goldenratio_a();

    double Fun_(double t_i);
    double I_(double delta_Theta_temp);
    void Density_polymer_phase_f();
    double density_ratio(double delta_Theta_temp);
    double Delta_2(double *x);

    double goldenratio_(double *x, int j_);
    void variable_descent_method();

    double volConcentration_polymerPhase_(int j_);
    double volConcentration_pyrolyticPhase_(double volConcentration_polymerPhase);
    double porosity_(double volConcentration_polymerPhase);
    double a_1_fun(double porosity, double vol_concentrat_pyrol_phase, double a_0);
    double fun(double porosity, double vol_concentrat_pyrol_phase, double a_0, double a_1, double x);

    void bisection_method();

public:
    double Elast_mod_polymer_phase_();

    QVector<double> calculationDensity();
    QVector<double> calculationElasticModulus();

    double get_Elast_mod_polymer_phase();
    double get_a_coeffElasticProperties();
    double get_density_polymer_phase();
    double get_gas_coef();
    double get_preExponental_factor();
    double get_Energy_activThermDestruct();
    double get_a_1_max();
    double get_a_0_max();
    double get_m_p_ElastModulsRatio();

    double get_Delta_1();
    double get_Delta_2();

    ~calculatorElasticModulus();
};



#endif // CALCULATORELASTICMODULUS_H
