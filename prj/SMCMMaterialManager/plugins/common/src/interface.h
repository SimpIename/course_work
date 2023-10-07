#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>
#include <QDialog>
#include <QMap>
#include <QDebug>
#include <QFile>

//// [0] include QXlsx headers
//#include "xlsxdocument.h"
//#include "xlsxchartsheet.h"
//#include "xlsxcellrange.h"
//#include "xlsxchart.h"
//#include "xlsxrichstring.h"
//#include "xlsxworkbook.h"


/// \brief экспериментальные данные для 1 таблицы
struct TableExperimental
{
    /// \brief массив заголовков
    QVector<QString> names;
    /// \brief построчный массив данных таблицы [строка][столбец]
    QVector<QVector<double> > values;
};

struct MaterialProperty
{
    QString description;
    double min;
    double max;
    int steps = 1;
};

/// \brief Составление ключа для контейнера с таблицами
inline QString setTableName(QVector<QString> names)
{
    QString name = "";
    for (int j = 0; j < names.size(); j++)
        name += (name.isEmpty()?"":",") + names[j];
    return name;
}

class AbstractInterfaceDialog : public QDialog
{
    Q_OBJECT
public:
    AbstractInterfaceDialog(QWidget *parent = nullptr)
        :QDialog(parent)
    {

    }
    virtual ~AbstractInterfaceDialog() = default;
    virtual QStringList getUsingProperties() = 0;
    virtual void updatePropertyValuesWidget() = 0;
    int exec(QMap<QString, MaterialProperty> &o_properties)
    {
        m_material_properties = o_properties;
        updatePropertyValuesWidget();
        int result = QDialog::exec();
        o_properties = m_material_properties;
        return result;
    }

public:
    QMap<QString, MaterialProperty> m_material_properties;
    /// \brief экспериментальные данные
    QMap<QString, double> m_experimental_scalars;
    QMap<QString, TableExperimental > m_experimental_tables;

};

class AbstractInterface :public QObject
{
    Q_OBJECT
public:
    virtual ~AbstractInterface() = default;
    virtual QString getPluginName() = 0;
    virtual AbstractInterfaceDialog *getPluginDialog() = 0;

    AbstractInterfaceDialog* m_dialog;
};



Q_DECLARE_INTERFACE(AbstractInterface, "SMCM_MATERIAL_MANAGER_PLUGIN")

#endif // INTERFACE_H
