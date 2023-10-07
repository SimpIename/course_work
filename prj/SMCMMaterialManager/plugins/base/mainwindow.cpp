#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDir"
#include <QPluginLoader>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    qDebug() << "on_pushButton_clicked";
    QStringList list_files;
    QDir dir (QDir::currentPath()  + PLUGIN_DIR);
    if (dir.exists()) list_files = dir.entryList((QStringList("*"),QDir::Files));
    else qDebug() << "dir error, name = " << dir.path();
    for (QString str: list_files)
    {
        Interface *plugin;
        QPluginLoader loader(dir.absolutePath() + "/" + str);
        QObject *p_obj = 0;
        p_obj = qobject_cast<QObject*>(loader.instance());
        if (!p_obj)
        {
            qDebug() << "object pointer error, name = " << str;
            continue;
        }
        plugin = qobject_cast<Interface*>(p_obj);
        if (plugin)
        {
            plugins.insert(plugin->getPluginName(),plugin->dialog);
            qDebug() << "plugin loaded, file_name = " << str << " name = " << plugin->getPluginName();
        }
        else
            qDebug() << "plugin pointer error, name = " << str;
    }
    qDebug() << "Загружены плагины:";
    foreach (QString str, plugins.keys()) qDebug() << str;
    foreach (interfaceDialog* dialog, plugins)
    {
        QMap<QString, MaterialProperty> properties;
        QStringList property_names = dialog->getUsingProperties();
        for (QString property_name:property_names)
        {
            MaterialProperty property;
            property.min = 1;
            property.description = "test plugin";
            property.max = 2;
            property.steps = 1;
            properties[property_name] = property;
        }
        dialog->exec(properties);
        for (QMap<QString, MaterialProperty>::iterator it = properties.begin(); it != properties.end(); it++)
        {
            qDebug() << it.key() << " " << it.value().min << " " << it.value().max << " " << it.value().steps << " " << it.value().description;
        }
    }

}
