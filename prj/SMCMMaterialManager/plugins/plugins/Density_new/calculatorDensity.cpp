#include "calculatorDensity.h"
#include <math.h>
//#include <conio.h>
#include <QVector>
#include <QDebug>

calculatorDensity::calculatorDensity()
{

}
calculatorDensity::calculatorDensity(int Num_points_e, double porosity_0_e, double heating_rate_e, double density_experiment_0_e, double *delta_Theta_e, double* density_experiment_e, double density_pyrol_phase_e)
{
    Num_points = Num_points_e;
    porosity_0 = porosity_0_e;
    heating_rate = heating_rate_e;
    density_experiment_0 = density_experiment_0_e;
    density_pyrol_phase = density_pyrol_phase_e;
    delta_Theta = new double[Num_points];
    density_experiment = new double[Num_points];
    for (int i = 0; i < Num_points; i++) {
        delta_Theta[i] = delta_Theta_e[i];
        density_experiment[i] = density_experiment_e[i];
    }
}

//Вычисление Sb
double calculatorDensity::S_b_(double porosity)
{
    return pow((porosity), 1.0 / 3);
}

//Вычисление Sp
double calculatorDensity::S_p_(double vol_concentrat_pyrol_phase)
{
    //В области повышенных температур Sp=1=const
    return pow((1 - vol_concentrat_pyrol_phase), 1.0 / 3);
}

//Вычисление R
double calculatorDensity::R_(double delta_Theta_temp)
{
    return delta_Theta_temp + ((P*heating_rate) / Q)*((delta_Theta_temp / heating_rate) - (1 / Q)*(1 - exp(-Q * delta_Theta_temp / heating_rate)));
}

//Вычисление Ф(S_b^0)
double calculatorDensity::F_(double porosity)
{
    double S_b = S_b_(porosity);
    return (1 - S_b * S_b) / (1 - S_b * S_b + S_b * S_b*S_b);
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

//Функция под I
double calculatorDensity::Fun_(double t_i)
{
    return exp(-ea_div_r / (temperature_0 + heating_rate * t_i));
}

//Рассчет I с использованием метода трапеций
double calculatorDensity::I_(double delta_Theta_temp)
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
void calculatorDensity::density_binder_f()
{
    density_binder = density_experiment_0 / (1 - porosity_0);
}

//Отношение плотностей (ро_m/ро_m^0)
double calculatorDensity::density_ratio(double delta_Theta_temp)
{
    return 1 - gasification_coefficient + gasification_coefficient * exp(-(velocity_volume_ablation_binder / density_binder)*I_(delta_Theta_temp));
}

double calculatorDensity::Delta_2(double *x)
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
double calculatorDensity::goldenratio_(double *x, int j_)
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
void calculatorDensity::variable_descent_method()
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




QVector<double> calculatorDensity::calculationDensity()
{
    //Определим необходимые константы
    density_binder_f();// density_binder
    variable_descent_method(); //gasification_coefficient, velocity_volume_ablation_binder, ea_div_r

    QVector<double> density(Num_points);// Рассчитанный модуль упругости

    for (int i = 0; i < Num_points; i++) {
        density[i] = density_ratio(delta_Theta[i]);
    }
    return density;
}


double calculatorDensity::get_density_binder()
{
    return density_binder;
}

double calculatorDensity::get_gasification_coefficient()
{
    return gasification_coefficient;
}

double calculatorDensity::get_velocity_volume_ablation_binder()
{
    return velocity_volume_ablation_binder;
}

double calculatorDensity::get_ea_div_r()
{
    return ea_div_r;
}

double calculatorDensity::get_Delta_2() {
    double x[3];
    x[0] = gasification_coefficient;
    x[1] = velocity_volume_ablation_binder;
    x[2] = ea_div_r;
    return Delta_2(x);
}

calculatorDensity::~calculatorDensity()
{
    delete density_experiment;
    delete delta_Theta;
}
