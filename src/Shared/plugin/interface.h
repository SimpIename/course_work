#ifndef INTERFACE_H
#define INTERFACE_H

#include <QDialog>
#include <QMap>
#include <QDebug>
#include <QFile>
#include "interfaceTypes.h"

//// [0] include QXlsx headers
//#include "xlsxdocument.h"
//#include "xlsxchartsheet.h"
//#include "xlsxcellrange.h"
//#include "xlsxchart.h"
//#include "xlsxrichstring.h"
//#include "xlsxworkbook.h"


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
    virtual void updateScalarDataWidget() = 0;
    virtual void updateDependencesWidget() = 0;
    virtual void updateDependenceDataWidget() = 0;

    virtual int exec(QMap<QString, MaterialProperty> &o_properties)
    {
        qDebug() << "AbstractInterfaceDialog exec";
        m_material_properties = o_properties;
        m_experimental_scalars.clear();
        m_experimental_tables.clear();
        updatePropertyValuesWidget();
        updateScalarDataWidget();
        updateDependencesWidget();
        updateDependenceDataWidget();
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
