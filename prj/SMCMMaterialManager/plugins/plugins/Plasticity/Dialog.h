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


    void on_calculatorE_clicked();

private:
    Ui::Dialog *ui;
    PluginBaseElementsWidget *m_plugin_base_elements_widget;
    ChartWindow* m_chart_window;
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

class PlasticityAproximator
{
public:
    PlasticityAproximator(const std::vector<double> &p_strains, const std::vector<double> &p_stresses)
        : m_strains(p_strains), m_stresses(p_stresses)
    {}

    double new_b;
    double new_epsilon_s;
    double new_omega0;
    std::vector<double> m_strains;
    std::vector<double> m_stresses;

    double new_G;
    double new_a;
    double new_q;
    std::vector<double> gaussSolve1(std::vector<std::vector<double>> &p_A,
                                    std::vector<double> &p_b) {
        int n = p_A.size();

        for (int i=0; i<n; i++) {
            // Search for maximum in this column
            double maxEl = fabs(p_A[i][i]);
            int maxRow = i;
            for (int k=i+1; k<n; k++) {
                if (fabs(p_A[k][i]) > maxEl) {
                    maxEl = fabs(p_A[k][i]);
                    maxRow = k;
                }
            }

                   // Swap maximum row with current row (column by column)
            for (int k=i; k<n;k++) {
                double tmp = p_A[maxRow][k];
                p_A[maxRow][k] = p_A[i][k];
                p_A[i][k] = tmp;
            }
            double tmp = p_b[maxRow];
            p_b[maxRow] = p_b[i];
            p_b[i] = tmp;

                   // Make all rows below this one 0 in current column
            for (int k=i+1; k<n; k++) {
                double c = -p_A[k][i]/p_A[i][i];
                for (int j=i; j<n; j++) {
                    if (i==j) {
                        p_A[k][j] = 0;
                    } else {
                        p_A[k][j] += c * p_A[i][j];
                    }
                }
                p_b[k] += c * p_b[i];
            }
        }

               // Solve equation Ax=p_b for an upper triangular matrix p_A
        std::vector<double> x(n);
        for (int i=n-1; i>=0; i--) {
            x[i] = p_b[i]/p_A[i][i];
            for (int k=i-1;k>=0; k--) {
                p_b[k] -= p_A[k][i] * x[i];
            }
        }
        return x;
    }

    double dOmega0(double x)
    {
        if (x < new_epsilon_s) {
            return -0.0001;
        }
        return -2 * new_G * pow(1 - new_epsilon_s / x, new_b) * x;
    }

    double dB(double x)
    {
        if (x < new_epsilon_s) {
            return 0.0001;
        }
        return -2 * new_G * new_omega0 * pow(1 - new_epsilon_s / x, new_b) *
               log(1 - new_epsilon_s / x) * x;
    }

    double dEpsilonS(double x)
    {
        if (x <= new_epsilon_s) {
            return 0.0001;
        }
        return 2 * new_G * new_omega0 * pow(1 - new_epsilon_s / x, new_b-1) * new_b;
    }

    double dA(double x, double x1)
    {
        double xx = sqrt(x*x + new_a*pow(x1,new_q));
        if (xx < new_epsilon_s) {
            return 0.0;
        }
        return -new_G * new_omega0 * pow(1 - new_epsilon_s / xx, new_b-1) *
               new_b * new_epsilon_s * pow(x1, new_q) *
               x / pow(x*x + new_a * pow(x1, new_q), 1.5);
    }

    double dQ(double x, double x1)
    {
        double xx = sqrt(x*x + new_a*pow(x1,new_q));
        if (xx < new_epsilon_s) {
            return 0.0;
        }
        return -new_G * new_omega0 * pow(1 - new_epsilon_s / xx, new_b-1) *
               new_b * new_epsilon_s * new_a * pow(x1, new_q)*
               log(x1)*x / pow(x*x + new_a * pow(x1, new_q), 1.5);
    }

    void prepLSM()
    {
        new_epsilon_s = m_strains[0];
        new_G = m_stresses[0] / 2.0 / m_strains[0];
        new_b = 3;
        new_omega0 = 0.9;

        for (int cc = 0; cc < 100; ++cc) {
            std::vector<std::vector<double>> A(m_strains.size()-1);
            std::vector<double> R(m_strains.size()-1);
            for (size_t i = 0; i < m_strains.size()-1; ++i) {
                R[i] = m_stresses[i+1] - comp(m_strains[i+1]);
            }

            for (size_t i = 0; i < m_strains.size()-1; ++i) {
                A[i].push_back(dOmega0(m_strains[i+1]));
                A[i].push_back(dB(m_strains[i+1]));
            }

            std::vector<std::vector<double>> AtA(A[0].size());
            std::vector<double> AtR(A[0].size());
            for (size_t i = 0; i < A[0].size(); ++i) {
                AtA[i].resize(A[0].size());
                for (size_t j = 0; j < A[0].size(); ++j) {
                    AtA[i][j] = 0;
                    for (size_t k = 0; k < m_strains.size()-1; ++k) {
                        AtA[i][j] += A[k][i] * A[k][j];
                    }
                }
                AtR[i] = 0;
                for (size_t k = 0; k < m_strains.size()-1; ++k) {
                    AtR[i] += A[k][i] * R[k];
                }
            }
            std::vector<double> res = gaussSolve1(AtA, AtR);

            new_omega0 += res[0];
            new_b += res[1];
            if (std::max(res[0]*res[0], res[1]*res[1]) < 1e-20) {
                break;
            }
        }
    }

    void prepLSMEpsS(double p_epsilon_s)
    {
        new_epsilon_s = p_epsilon_s;
        new_G = m_stresses[0] / 2.0 / m_strains[0];
        new_b = 3;
        new_omega0 = 0.9;

        for (int cc = 0; cc < 100; ++cc) {
            std::vector<std::vector<double>> A(m_strains.size()-1);
            std::vector<double> R(m_strains.size()-1);
            for (size_t i = 0; i < m_strains.size()-1; ++i) {
                R[i] = m_stresses[i+1] - comp(m_strains[i+1]);
            }

            for (size_t i = 0; i < m_strains.size()-1; ++i) {
                A[i].push_back(dOmega0(m_strains[i+1]));
                A[i].push_back(dB(m_strains[i+1]));
            }

            std::vector<std::vector<double>> AtA(A[0].size());
            std::vector<double> AtR(A[0].size());
            for (size_t i = 0; i < A[0].size(); ++i) {
                AtA[i].resize(A[0].size());
                for (size_t j = 0; j < A[0].size(); ++j) {
                    AtA[i][j] = 0;
                    for (size_t k = 0; k < m_strains.size()-1; ++k) {
                        AtA[i][j] += A[k][i] * A[k][j];
                    }
                }
                AtR[i] = 0;
                for (size_t k = 0; k < m_strains.size()-1; ++k) {
                    AtR[i] += A[k][i] * R[k];
                }
            }
            std::vector<double> res = gaussSolve1(AtA, AtR);

            new_omega0 += res[0];
            new_b += res[1];
            if (std::max(res[0]*res[0], res[1]*res[1]) < 1e-20) {
                break;
            }
        }
    }

    void prepLSM3()
    {
        new_epsilon_s = m_strains[0];
        new_G = m_stresses[0] / 2.0 / m_strains[0];
        new_b = 3;
        new_omega0 = 0.9;

        for (int cc = 0; cc < 100; ++cc) {
            std::vector<std::vector<double>> A(m_strains.size()-1);
            std::vector<double> R(m_strains.size()-1);
            for (size_t i = 0; i < m_strains.size()-1; ++i) {
                R[i] = m_stresses[i+1] - comp(m_strains[i+1]);
            }

            for (size_t i = 0; i < m_strains.size()-1; ++i) {
                A[i].push_back(dOmega0(m_strains[i+1]));
                A[i].push_back(dB(m_strains[i+1]));
                A[i].push_back(dEpsilonS(m_strains[i+1]));
            }

            std::vector<std::vector<double>> AtA(A[0].size());
            std::vector<double> AtR(A[0].size());
            for (size_t i = 0; i < A[0].size(); ++i) {
                AtA[i].resize(A[0].size());
                for (size_t j = 0; j < A[0].size(); ++j) {
                    AtA[i][j] = 0;
                    for (size_t k = 0; k < m_strains.size()-1; ++k) {
                        AtA[i][j] += A[k][i] * A[k][j];
                    }
                }
                AtR[i] = 0;
                for (size_t k = 0; k < m_strains.size()-1; ++k) {
                    AtR[i] += A[k][i] * R[k];
                }
            }
            std::vector<double> res = gaussSolve1(AtA, AtR);

            new_omega0 += res[0];
            new_b += res[1];
            new_epsilon_s += res[2];
            if (new_epsilon_s < 0.00001) {
                //                new_epsilon_s = 0.00001;
                new_epsilon_s = m_strains[0]/2.0;
                prepLSMEpsS(new_epsilon_s);
                break;
            }

            if (mat::max(res[0]*res[0], res[1]*res[1], res[2]*res[2]) < 1e-20) {
                break;
            }
        }
    }
    bool prepLSM3(double p_b)
    {
        new_epsilon_s = m_strains[0];
        new_G = m_stresses[0] / 2.0 / m_strains[0];
        new_b = p_b;
        new_omega0 = 0.9;

        for (int cc = 0; cc < 100; ++cc) {
            std::vector<std::vector<double>> A(m_strains.size()-1);
            std::vector<double> R(m_strains.size()-1);
            for (size_t i = 0; i < m_strains.size()-1; ++i) {
                R[i] = m_stresses[i+1] - comp(m_strains[i+1]);
            }

            for (size_t i = 0; i < m_strains.size()-1; ++i) {
                A[i].push_back(dOmega0(m_strains[i+1]));
                A[i].push_back(dB(m_strains[i+1]));
                A[i].push_back(dEpsilonS(m_strains[i+1]));
            }

            std::vector<std::vector<double>> AtA(A[0].size());
            std::vector<double> AtR(A[0].size());
            for (size_t i = 0; i < A[0].size(); ++i) {
                AtA[i].resize(A[0].size());
                for (size_t j = 0; j < A[0].size(); ++j) {
                    AtA[i][j] = 0;
                    for (size_t k = 0; k < m_strains.size()-1; ++k) {
                        AtA[i][j] += A[k][i] * A[k][j];
                    }
                }
                AtR[i] = 0;
                for (size_t k = 0; k < m_strains.size()-1; ++k) {
                    AtR[i] += A[k][i] * R[k];
                }
            }
            std::vector<double> res = gaussSolve1(AtA, AtR);

            new_omega0 += res[0];
            new_b += res[1];
            new_epsilon_s += res[2];
            if (new_epsilon_s < 0.00001) {
                //                LOG_MESSAGE("small_epsilon");
                return false;
            }

            if (mat::max(res[0]*res[0], res[1]*res[1], res[2]*res[2]) < 1e-20) {
                return true;
            }
        }
        return false;
    }
    bool prepLSMCycle()
    {
        int total = 1000;
        for (int i = 0; i < total; ++i) {
            double num = double(i+1) / total * 10 + 1;
            if (prepLSM3(num)) {
                //                LOG_MESSAGE("YES num = " << num);
                return true;
            } else {
                //                LOG_MESSAGE("NO num = " << num);
            }
        }
        prepLSM();
        return true;
    }

    bool prepLSMAQ(const std::vector<double> &p_strains1, double p_a)
    {
        new_a = p_a;
        //    new_a = 0.5;
        new_q = 1.5;

        for (int cc = 0; cc < 100; ++cc) {
            std::vector<std::vector<double>> A(m_strains.size());
            std::vector<double> R(m_strains.size());
            for (size_t i = 0; i < m_strains.size(); ++i) {
                R[i] = m_stresses[i] - comp(m_strains[i], p_strains1[i]);
            }

            for (size_t i = 0; i < m_strains.size(); ++i) {
                A[i].push_back(dA(m_strains[i], p_strains1[i]));
                A[i].push_back(dQ(m_strains[i], p_strains1[i]));
            }

            std::vector<std::vector<double>> AtA(A[0].size());
            std::vector<double> AtR(A[0].size());
            for (size_t i = 0; i < A[0].size(); ++i) {
                AtA[i].resize(A[0].size());
                for (size_t j = 0; j < A[0].size(); ++j) {
                    AtA[i][j] = 0;
                    for (size_t k = 0; k < m_strains.size(); ++k) {
                        AtA[i][j] += A[k][i] * A[k][j];
                    }
                }
                AtR[i] = 0;
                for (size_t k = 0; k < m_strains.size(); ++k) {
                    AtR[i] += A[k][i] * R[k];
                }
            }
            std::vector<double> res = gaussSolve1(AtA, AtR);

            new_a += res[0];
            new_q += res[1];
            if (std::max(res[0]*res[0], res[1]*res[1]) < 1e-20) {
                return true;
            }
        }
        return false;
    }

    bool prepLSMA(const std::vector<double> &p_strains1, double p_a)
    {
        new_a = p_a;
        new_q = 2;

        for (int cc = 0; cc < 100; ++cc) {
            std::vector<std::vector<double>> A(m_strains.size());
            std::vector<double> R(m_strains.size());
            for (size_t i = 0; i < m_strains.size(); ++i) {
                R[i] = m_stresses[i] - comp(m_strains[i], p_strains1[i]);
            }

            for (size_t i = 0; i < m_strains.size(); ++i) {
                A[i].push_back(dA(m_strains[i], p_strains1[i]));
            }

            std::vector<std::vector<double>> AtA(A[0].size());
            std::vector<double> AtR(A[0].size());
            for (size_t i = 0; i < A[0].size(); ++i) {
                AtA[i].resize(A[0].size());
                for (size_t j = 0; j < A[0].size(); ++j) {
                    AtA[i][j] = 0;
                    for (size_t k = 0; k < m_strains.size(); ++k) {
                        AtA[i][j] += A[k][i] * A[k][j];
                    }
                }
                AtR[i] = 0;
                for (size_t k = 0; k < m_strains.size(); ++k) {
                    AtR[i] += A[k][i] * R[k];
                }
            }
            std::vector<double> res = gaussSolve1(AtA, AtR);

            new_a += res[0];
            if (res[0]*res[0] < 1e-20) {
                return true;
            }
        }
        return false;
    }
    bool prepLSMAQCycle(const std::vector<double> &p_strains1)
    {
        int total = 1000;
        for (int i = 0; i < total; ++i) {
            double num = double(i+1) / total * 2;
            if (prepLSMAQ(p_strains1, num)) {
                //                    LOG_MESSAGE("YES num = " << num);
                return true;
            } else {
                //                    LOG_MESSAGE("NO num = " << num);
            }
        }

        return false;
    }

    double comp(double x)
    {
        double omega = 0;
        if (x > new_epsilon_s) {
            omega = new_omega0 * pow(1.0 - new_epsilon_s / x, new_b);
        }

        return 2 * new_G * (1 - omega) * x;
    }
    double comp(double x, double x1)
    {
        double omega = 0;
        double xx = sqrt(x*x + new_a*pow(x1,new_q));
        if (xx > new_epsilon_s) {
            omega = new_omega0 * pow(1.0 - new_epsilon_s / xx, new_b);
        }

        return 2 * new_G * (1 - omega) * x;
    }
};


#endif // DIALOG_H
