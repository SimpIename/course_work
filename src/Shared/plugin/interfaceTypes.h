#ifndef INTERFACETYPES_H
#define INTERFACETYPES_H

#include <QObject>

/// \brief Составление ключа для контейнера с таблицами
inline QString setTableName(QVector<QString> names)
{
    QString name = "";
    for (int j = 0; j < names.size(); j++)
        name += (name.isEmpty()?"":",") + names[j];
    return name;
}

/// \brief экспериментальные данные для 1 таблицы
struct TableExperimental
{
    /// \brief массив заголовков
    QVector<QString> names;
    /// \brief построчный массив данных таблицы [строка][столбец]
    QVector<QVector<double> > values;
};

struct direction
{
    int     arg_childRole_id;
    QString argName = "неизвестно";
    int     steps           ;
    int     degree          ;
    float   minValue        ;
    float   maxValue        ;
};

struct coefficient
{
    float value       ;
    float weight = 1.0;
};

struct MaterialProperty
{
    QString              description          ; //описание
    QStringList          approximations       ; //доступные аппроксимации
    QString              approximation        ; //функциональные: выбранная аппроксимация
    QVector<direction>   directions           ; //функциональные: аргументы
    QVector<float>       nodes                ; //функциональные: узлы
    QVector<coefficient> coefficients         ; //функциональные: коэффициенты
    double               min                  ; //интервальные: минимум  скалярные: значение
    double               max                  ; //интервальные: максимум
    int                  steps = 1            ; //интервальные: шаги
};


#endif // INTERFACETYPES_H
