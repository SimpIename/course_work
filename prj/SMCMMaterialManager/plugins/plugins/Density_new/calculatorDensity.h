#ifndef CALCULATORDENSITY_H
#define CALCULATORDENSITY_H


#include<QVector>
class calculatorDensity
{
private:
    const double P = 0.000156;
    const double Q = 0.00025;
    double temperature_0 = 272;
    //Данные, которые мы вводим
    int Num_points; // количество точек

    double *delta_Theta;// Изменение температуры
    double *density_experiment;//Плотность матрицы (ро_m)

    double porosity_0;// Начальная пористость
    double heating_rate;// скорость нагрева
    double density_experiment_0;//Начальная плотность ро_m^0   [кг/м^3]
    double density_pyrol_phase;//Плотность твердой пиролитической фазы (ро_p)

    // Константы, которые необходимо вычислить
    double density_binder;//Плотность твердой полимерной фазы (ро_b)
    double gasification_coefficient;//Коэффициент газификации (Г)
    double velocity_volume_ablation_binder;//Предэкспоненциальный множитель скорости объемной абеляции матрицы (J_0)
    double ea_div_r;//Энергия активации процесса термодиструкции (E_A) деленная на газовую постоянную R


public:
    calculatorDensity();
    calculatorDensity(int Num_points_e, double porosity_0_e, double heating_rate_e, double density_experiment_0_e, double *delta_Theta_e, double* density_experiment_e, double density_pyrol_phase_e);


    int termalDestruction();//Критерий выбора области ДО начала термодиструкции (вычисляем индекс температуры начала ТД)

    private:
    double S_b_(double porosity);
    double S_p_(double vol_concentrat_pyrol_phase);
    double R_(double delta_Theta_temp);
    double F_(double porosity);

    double Fun_(double t_i);
    double I_(double delta_Theta_temp);
    void density_binder_f();
    double density_ratio(double delta_Theta_temp);
    double Delta_2(double *x);

    double goldenratio_(double *x, int j_);
    void variable_descent_method();

public:
    double Elast_mod_polymer_phase_();

    QVector<double> calculationDensity();

    double get_density_binder();
    double get_gasification_coefficient();
    double get_velocity_volume_ablation_binder();
    double get_ea_div_r();

    double get_Delta_2();

    ~calculatorDensity();
};


#endif // CALCULATORDENSITY_H
