#ifndef PLUGINBASEELEMENTSWIDGET_H
#define PLUGINBASEELEMENTSWIDGET_H

#include <QWidget>
#include "../../../../../src/Shared/plugin/interface.h"
#include <Defs>
#include <ChartWindow>
#include <QDialog>
#include <QVector>
#include <QMap>


namespace Ui {
class PluginBaseElementsWidget;
}

class PluginBaseElementsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PluginBaseElementsWidget(QWidget *parent = nullptr);
    ~PluginBaseElementsWidget();
    void updatePropertyValuesWidget();
    void updateScalarDataWidget();
    void updateDependencesWidget();
    void updateDependenceDataWidget();
    void set_interface_dialog(AbstractInterfaceDialog * p_interface_dialog);
    ChartWindow *get_chart_window();

private slots:
    void addScalarValue();
    void addTable();
    void addTableColumn();
    void addTableValue();

    void tableValuesChanged();
    void scalarValueChanged(const QModelIndex &p_topLeft, const QModelIndex &p_botomRight);
    void propertyValueChanged(const QModelIndex &p_topLeft, const QModelIndex &p_botomRight);

    void on_acceptButton_clicked();

    void on_loadTextButton_clicked();

    void on_saveTextButton_clicked();

private:
    Ui::PluginBaseElementsWidget *ui;
    AbstractInterfaceDialog * m_interface_dialog;
    ChartWindow *m_chart_window;
    bool m_autoTableValuesChanged;

    /// \brief загрузка из текстового файла
    void loadDataFromText(QString p_filename);
    /// \brief загрузка из Json файла
    void loadDataFromJson(QString p_filename);
    /// \brief загрузка из XML файла
    void loadDataFromXML(QString p_filename);
    /// \brief загрузка из XLSX файла
    void loadDataFromXLSX(QString p_filename);

    /// \brief Запись в текстовый файл
    void saveDataToText(QString p_filename);
    /// \brief Запись в Json файл
    void saveDataToJson(QString p_filename);
    /// \brief Запись в XML файл
    void saveDataToXML(QString p_filename);
    /// \brief Запись в XLSX файл
    void saveDataToXLSX(QString p_filename);

signals:
    void accept();

};

#endif // PLUGINBASEELEMENTSWIDGET_H
