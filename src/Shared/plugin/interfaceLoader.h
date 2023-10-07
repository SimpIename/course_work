#ifndef INTERFACELOADER_H
#define INTERFACELOADER_H

#include "interfaceTypes.h"


#include <QObject>
#include <QDialog>
#include <QMap>

class interfaceDialog : public QDialog
{
    Q_OBJECT
public:
    interfaceDialog(QWidget *parent = nullptr)
        :QDialog(parent)
    {

    }
    virtual ~interfaceDialog() = default;
    virtual QStringList getUsingProperties() = 0;
    virtual void updatePropertyValuesWidget() = 0;
    virtual void updateScalarDataWidget() = 0;
    virtual void updateDependencesWidget() = 0;
    virtual void updateDependenceDataWidget() = 0;

    virtual int exec(QMap<QString, MaterialProperty> &o_properties)
    {
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

//    int exec(QMap<QString, MaterialProperty> &p_properties)
//        {
//            m_material_properties = p_properties;
//            updatePropertyValuesWidget();
//            int result = QDialog::exec();
//            p_properties = m_material_properties;
//            return result;
//        }

protected:
    QMap<QString, MaterialProperty> m_material_properties;
    /// \brief экспериментальные данные
    QMap<QString, double> m_experimental_scalars;
    QMap<QString, TableExperimental > m_experimental_tables;
};

class Interface :public QObject
{
    Q_OBJECT
public:
    virtual ~Interface() = default;
    virtual QString getPluginName() = 0;
    virtual interfaceDialog *getPluginDialog() = 0;

    interfaceDialog* dialog;
};



Q_DECLARE_INTERFACE(Interface, "SMCM_MATERIAL_MANAGER_PLUGIN")


#endif // INTERFACELOADER_H
