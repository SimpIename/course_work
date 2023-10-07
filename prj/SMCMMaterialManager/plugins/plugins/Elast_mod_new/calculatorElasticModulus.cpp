#include "calculatorElasticModulus.h"
#include <math.h>
//#include <conio.h>
#include <QVector>
#include <QDebug>

calculatorElasticModulus::calculatorElasticModulus()
{

}
calculatorElasticModulus::calculatorElasticModulus(int Num_points_e, double porosity_0_e, double heating_rate_e, double density_experiment_0_e, double *delta_Theta_e, double *Elastic_modulus_experiment_e, double* density_experiment_e, double density_pyrol_phase_e)
{
    Num_points = Num_points_e;
    porosity_0 = porosity_0_e;
    heating_rate = heating_rate_e;
    density_experiment_0 = density_experiment_0_e;
    density_pyrol_phase = density_pyrol_phase_e;
    delta_Theta = new double[Num_points];
    Elastic_modulus_experiment = new double[Num_points];
    delta_Theta = new double[Num_points];
    density_experiment = new double[Num_points];
    for (int i = 0; i < Num_points; i++) {
        delta_Theta[i] = delta_Theta_e[i];
        density_experiment[i] = density_experiment_e[i];
        Elastic_modulus_experiment[i] = Elastic_modulus_experiment_e[i];
    }
}

//Критерий выбора области ДО начала термодиструкции (вычисляем индекс температуры начала ТД)
int calculatorElasticModulus::termalDestruction()
{
    int j = 0;
    if (density_experiment[0] != density_experiment[Num_points - 1]) {
        while (((density_experiment[0] - density_experiment[j]) / (density_experiment[0] - density_experiment[Num_points - 1]) < 0.05) && (j < Num_points)) {
            j++;
        }
        return j;
    }
    else
        return Num_points;
}

//Вычисление Sb
double calculatorElasticModulus::S_b_(double porosity)
{
    return pow((porosity), 1.0 / 3);
}

//Вычисление Sp
double calculatorElasticModulus::S_p_(double vol_concentrat_pyrol_phase)
{
    //В области повышенных температур Sp=1=const
    return pow((1 - vol_concentrat_pyrol_phase), 1.0 / 3);
}

//Вычисление R
double calculatorElasticModulus::R_(double delta_Theta_temp)
{
    return delta_Theta_temp + ((P*heating_rate) / Q)*((delta_Theta_temp / heating_rate) - (1 / Q)*(1 - exp(-Q * delta_Theta_temp / heating_rate)));
}

//Вычисление Ф(S_b^0)
double calculatorElasticModulus::F_(double porosity)
{
    double S_b = S_b_(porosity);
    return (1 - S_b * S_b) / (1 - S_b * S_b + S_b * S_b*S_b);
}

//Вычисление значения а(0) (для teta_max вызов delta_Theta max)
double calculatorElasticModulus::a_0_(double delta_Theta_temp)
{
    double R = R_(delta_Theta_temp);
    return exp(-reversibility_coefficient_kelv * reversibility_coefficient_kelv * R * R);
}

//Вычисление модуля упругости полимерной фазы
double calculatorElasticModulus::E_()
{
    E = Elastic_modulus_experiment[0] / (F_(porosity_0));
    return Elastic_modulus_experiment[0] / (F_(porosity_0));
}

//Вычисление а1 при teta_j_ (для teta_max вызов при j=N-1)
double calculatorElasticModulus::a_1_(int j_)
{
    return Elastic_modulus_experiment[j_] / E_();
}

// Теоретическое вычисление модуля упругости
double calculatorElasticModulus::Elastic_modulus(double delta_Theta_temp, double porosity)
{
    return E * F_(porosity)*a_0_(delta_Theta_temp);
}

//Суммарная ошибка
double calculatorElasticModulus::Delta_(double x)
{
    //Модуль упругости полимерной фазы
    E = E_();
    reversibility_coefficient_kelv = x;

    int j = termalDestruction();

    double Sum = 0;//Подсчитаем сумму
    for (int i = 0; i < j; i++) {
        Sum += pow(1 - (Elastic_modulus(delta_Theta[i], porosity_0)) / Elastic_modulus_experiment[i], 2);
    }
    return Sum / j;
}

double alpha(double a, double b) {
    double gamma = 0.618;
    return gamma * a + (1 - gamma)*b;
}
//(2)
double betta(double a, double b) {
    double gamma = 0.618;
    return (1 - gamma)*a + gamma * b;
}

//Метод золотого сечения для минимизации а
void calculatorElasticModulus::goldenratio_a()
{
    //Модуль упругости полимерной фазы
    E = E_();
    //Введем переменные:
    double a_ = 0, b_ = 1.0/272; //Отрезок

    double eps; //Точность
    int k;// Счетчик итераций
    double alf, bet;//Координаты золотого сечения
    reversibility_coefficient_kelv = 1;

    k = 1;
    //Вычисляем начальные координаты золотого сечения
    alf = alpha(a_, b_);
    bet = betta(a_, b_);
    //Точность
    eps = 0.000001;


    while ((b_ - a_) > eps*eps) {
        if (Delta_(alf) > Delta_(bet)) {
            a_ = alf;
            alf = bet;
            bet = betta(a_, b_);
        }
        //Шаг 3
        else {
            b_ = bet;
            bet = alf;
            alf = alpha(a_, b_);
        }
        k++;
    }

    reversibility_coefficient_kelv = (a_ + b_) / 2;
}

//Функция под I
double calculatorElasticModulus::Fun_(double t_i)
{
    return exp(-ea_div_r / (temperature_0 + heating_rate * t_i));
}

//Рассчет I с использованием метода трапеций
double calculatorElasticModulus::I_(double delta_Theta_temp)
{
    double SUM = 0;//Сумма
    double temp;// Текущее значение
    double prev;//Предыдущее значение
    int n_ = 1000;// Количество разбиений
    double t_i = delta_Theta_temp / heating_rate;//Верхний предел интеграла
    double h = t_i / (n_);//шаг
    double x_i = 0;

    if (delta_Theta_temp == 0) return 0;
    else {
        prev = 0;//Значение функции на левом конце
        for (int i = 1; i <= n_; i++) {
            x_i += h;
            temp = Fun_(x_i);
            SUM += (temp + prev)*h / 2;
            prev = temp;
        }
        return SUM;
    }
}

// Рассчет плотности твердой полимерной фазы (ро_b)
void calculatorElasticModulus::density_binder_f()
{
    density_binder = density_experiment_0 / (1 - porosity_0);
}

//Отношение плотностей (ро_m/ро_m^0)
double calculatorElasticModulus::density_ratio(double delta_Theta_temp)
{
    return 1 - gasification_coefficient + gasification_coefficient * exp(-(velocity_volume_ablation_binder / density_binder)*I_(delta_Theta_temp));
}

double calculatorElasticModulus::Delta_2(double *x)
{
    gasification_coefficient = x[0];//Коэффициент газификации (Г)
    velocity_volume_ablation_binder = x[1];//Предэкспоненциальный множитель (J_0)
    ea_div_r = x[2];//Энергия активации процесса термодиструкции (E_A)

    double Sum = 0;//Подсчитаем сумму
    for (int i = 0; i < Num_points; i++) {
        Sum += pow(density_experiment[i] - density_ratio(delta_Theta[i]), 2);
    }
    return Sum / Num_points;
}

//Решение задачи одномерной оптимизации шага s с использованим метода золотого сечения
double calculatorElasticModulus::goldenratio_(double *x, int j_)
{
    //Введем переменные:
    //В зависимости от переменной необходимо настроить промежуток значений, который может принимать переменная

    int n = 3;
    // Отрезок для ...
    double a = 0, b;
    // Для коэффициента газификации
    if (j_ == 0) {
        if (density_experiment[Num_points - 1] == density_experiment[Num_points - 2]) {
            return 1 - density_experiment[Num_points - 1];
        }
        a = 1 - density_experiment[Num_points - 1] - 0.2;
        b = 1; //Отрезок
    }
    // Для предэкспоненциального множителя
    if (j_ == 1) {
        a = 360000;
        b = 10000000000000; //Отрезок
    }
    // Для энергии активации процесса термодиструкции
    if (j_ == 2) {
        a = 1000;
        b = 23000; //Отрезок
    }
    //______________
    double eps; //Точность
    int k;// Счетчик итераций
    double alf, bet;//Координаты золотого сечения
    //Шаг 0
    k = 1;
    //Вычисляем начальные координаты золотого сечения
    alf = alpha(a, b);
    bet = betta(a, b);
    eps = 0.0001;
    double *x_alf = new double[n];
    double *x_bet = new double[n];
    for (int i_ = 0; i_ < n; i_++) {
        x_alf[i_] = x[i_];
        x_bet[i_] = x[i_];
    }
    x_bet[j_] = bet;
    x_alf[j_] = alf;
    //Шаг 1
    while ((b - a) > eps) {
        //Шаг 2
        if (Delta_2(x_alf) > Delta_2(x_bet)) {
            a = alf;
            alf = bet;
            bet = betta(a, b);
            x_bet[j_] = bet;
            x_alf[j_] = alf;
        }
        //Шаг 3
        else {
            b = bet;
            bet = alf;
            alf = alpha(a, b);
            x_bet[j_] = bet;
            x_alf[j_] = alf;
        }
        k++;
    }
    delete[] x_alf;
    delete[] x_bet;
    return (a + b) / 2;
}

//Метод циклического покординатного спуска
void calculatorElasticModulus::variable_descent_method()
{

    double *x;// Вектор ответа
    x = new double[3];

    for (int i_ = 0; i_ < 3; i_++) {
        x[i_] = 1;
    }

    double eps = 0.0003;// Точность
    double x_;//Координата j промежуточной точки
    double fun_ = 1;// Промежуточное значение функции
    int j = 0;// Счетчик числа направлений
    int k = 0;// Счетчик числа итераций
    int n = 3; //Кол-во вариативных переменных
    int fl = 0;// Флажек-костыль против зацикливания
    do {
        //Решение задачи одномерной оптимизации с направлением j
        x_ = goldenratio_(x, j);
        if ((fun_ - Delta_2(x))<eps) {
            fl++;
        }
        else
            fl = 0;
        if (j + 1 < n) {
            fun_ = Delta_2(x);
            x[j] = x_;
            j++;
        }
        else {
            fun_ = Delta_2(x);
            x[j] = x_;
            j = 0;
        }
        k++;
    } while ((Delta_2(x) > eps) && (fl < 6));
    x_ = goldenratio_(x, 1);

    gasification_coefficient = x[0];//Коэффициент газификации (Г)
    velocity_volume_ablation_binder = x[1];//Предэкспоненциальный множитель (J_0)
    ea_div_r = x[2];//Энергия активации процесса термодиструкции (E_A)
}

//Вычисление фи_b
double calculatorElasticModulus::volConcentration_polymerPhase_(int j_)
{
    double fi_b_temp;

    //Начальное фи_b=1-фи^0_g
    fi_b_temp = 1 - porosity_0;

    double delta_t;

    for (int i = 0; i < j_ - 1; i++) {
        delta_t = fabs((delta_Theta[i + 1] - delta_Theta[i]) / heating_rate);
        fi_b_temp = fi_b_temp * (1 / (1 + (delta_t / density_binder)*velocity_volume_ablation_binder*exp(-ea_div_r / (delta_Theta[i] + temperature_0))));
    }
    return fi_b_temp;
}

//Вычисление фи_p
double calculatorElasticModulus::volConcentration_pyrolyticPhase_(double volConcentration_polymerPhase)
{
    return ((1 - porosity_0) - volConcentration_polymerPhase)*(density_binder / density_pyrol_phase)*(1 - gasification_coefficient);
}

//Вычисление фи_g
double calculatorElasticModulus::porosity_(double volConcentration_polymerPhase)
{
    return 1 - volConcentration_polymerPhase - volConcentration_pyrolyticPhase_(volConcentration_polymerPhase);
}

double calculatorElasticModulus::a_1_fun(double porosity, double vol_concentrat_pyrol_phase, double a_0)
{
    double S_b = S_b_(porosity);
    double S_p = S_p_(vol_concentrat_pyrol_phase);
    return 1 / (S_b / (a_0*(S_p*S_p - S_b * S_b) + ratio_of_elastic_modulus * (1 - S_p * S_p)) + (S_p - S_b) / (a_0*S_p*S_p + ratio_of_elastic_modulus * (1 - S_p * S_p)) + (1 - S_p));
}

//Формула (**) для вычисления m_p
double calculatorElasticModulus::fun(double porosity, double vol_concentrat_pyrol_phase, double a_0, double a_1, double x) {

    double S_b = S_b_(porosity);
    double S_p = S_p_(vol_concentrat_pyrol_phase);
    ratio_of_elastic_modulus = x;

    return S_b / (a_0*(S_p*S_p - S_b * S_b) + ratio_of_elastic_modulus * (1 - S_p * S_p)) + (S_p - S_b) / (a_0*S_p*S_p + ratio_of_elastic_modulus * (1 - S_p * S_p)) + (1 - S_p) / ratio_of_elastic_modulus - 1 / a_1;
}

//метод деления отрезка пополам
void calculatorElasticModulus::bisection_method() {

    //Вычисление констант, которые мы далее подставим в формулу

//Твердая полимерная фаза фи_b для температуры teta_max
    double vol_concentrat_polymer_phase = volConcentration_polymerPhase_(Num_points);

    //Твердая пиролитическая фаза фи_p для температуры teta_max
    double vol_concentrat_pyrol_phase = volConcentration_pyrolyticPhase_(vol_concentrat_polymer_phase);

    //Пористость фи_g
    double porosity = porosity_(vol_concentrat_polymer_phase);

    double a_1_max = a_1_(Num_points - 1);
    double a_0_max = a_0_(delta_Theta[Num_points - 1]);

    double a_ot = 0;//левый конец отрезка
    double b_ot = 1;//правый конец отрезка
    double eps = 0.00000001;//точность
    if (fun(porosity, vol_concentrat_pyrol_phase, a_0_max, a_1_max, a_ot) != 0) {
        if (fun(porosity, vol_concentrat_pyrol_phase, a_0_max, a_1_max, b_ot) != 0) {
            if (fun(porosity, vol_concentrat_pyrol_phase, a_0_max, a_1_max, a_ot)*fun(porosity, vol_concentrat_pyrol_phase, a_0_max, a_1_max, b_ot) > 0)
                ratio_of_elastic_modulus = -1;
                //printf_s("Корня нет");
            else {
                do {
                    double temp = (a_ot + b_ot) / 2;
                    if (fun(porosity, vol_concentrat_pyrol_phase, a_0_max, a_1_max, a_ot)*fun(porosity, vol_concentrat_pyrol_phase, a_0_max, a_1_max, temp) < 0)
                        b_ot = temp;
                    else
                        a_ot = temp;

                } while (fabs(b_ot - a_ot) >= eps);
            }
        }
        else
            a_ot = b_ot;
    }
    else
        b_ot = a_ot;
    ratio_of_elastic_modulus = (a_ot + b_ot) / 2.0;
}


QVector<double> calculatorElasticModulus::calculationDensity()
{
    //Определим необходимые константы
    E = E_(); //E
    goldenratio_a(); //reversibility_coefficient_kelv
    density_binder_f();// density_binder
    variable_descent_method(); //gasification_coefficient, velocity_volume_ablation_binder, ea_div_r

    a_1_max = a_1_(Num_points - 1);
    a_0_max = a_0_(delta_Theta[Num_points - 1]);

    bisection_method(); //ratio_of_elastic_modulus

    QVector<double> density(Num_points);// Рассчитанный модуль упругости

    for (int i = 0; i < Num_points; i++) {
        density[i] = density_ratio(delta_Theta[i]);
    }
    return density;
}

QVector<double> calculatorElasticModulus::calculationElasticModulus()
{
    calculationDensity();
    //Модуль упругости полимерной фазы
    double E = E_();
    QVector<double> Elastic_modulus_(Num_points);// Рассчитанный модуль упругости
    //Elastic_modulus_ = new double[Num_points];

    double volConcentration_polymerPhase;
    double volConcentrat_pyrolPhase;
    double porosity_temp;
    double a_1_temp;
    double a_0_temp;

    int j = termalDestruction();
    for (int i = 0; i < Num_points; i++) {
        if (i < j) {
            Elastic_modulus_[i] = Elastic_modulus(delta_Theta[i], porosity_0);
            a_1_temp = F_(porosity_0)*a_0_(delta_Theta[i]);
        }
        else {
            volConcentration_polymerPhase = volConcentration_polymerPhase_(i + 1);
            volConcentrat_pyrolPhase = volConcentration_pyrolyticPhase_(volConcentration_polymerPhase);
            porosity_temp = porosity_(volConcentration_polymerPhase);
            a_0_temp = a_0_(delta_Theta[i]);
            a_1_temp = a_1_fun(porosity_temp, volConcentrat_pyrolPhase, a_0_temp);
            Elastic_modulus_[i] = E * a_1_temp;
        }
    }
    return Elastic_modulus_;
}

double calculatorElasticModulus::get_E()
{
    return E;
}

double calculatorElasticModulus::get_reversibility_coefficient_kelv()
{
    return reversibility_coefficient_kelv;
}

double calculatorElasticModulus::get_reversibility_coefficient()
{
    return reversibility_coefficient_kelv*temperature_0;
}

double calculatorElasticModulus::get_density_binder()
{
    return density_binder;
}

double calculatorElasticModulus::get_gasification_coefficient()
{
    return gasification_coefficient;
}

double calculatorElasticModulus::get_velocity_volume_ablation_binder()
{
    return velocity_volume_ablation_binder;
}

double calculatorElasticModulus::get_ea_div_r()
{
    return ea_div_r;
}

double calculatorElasticModulus::get_a_1_max()
{
    return a_1_max;
}

double calculatorElasticModulus::get_a_0_max()
{
    return a_0_max;
}

double calculatorElasticModulus::get_ratio_of_elastic_modulus()
{
    return ratio_of_elastic_modulus;
}

double calculatorElasticModulus::get_Delta_1() {
    return Delta_(reversibility_coefficient_kelv);
}
double calculatorElasticModulus::get_Delta_2() {
    double x[3];
    x[0] = gasification_coefficient;
    x[1] = velocity_volume_ablation_binder;
    x[2] = ea_div_r;
    return Delta_2(x);
}

calculatorElasticModulus::~calculatorElasticModulus()
{
    delete density_experiment;
    delete Elastic_modulus_experiment;
    delete delta_Theta;
}
