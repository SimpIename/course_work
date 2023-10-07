#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "interfaceLoader.h"
#include <QMainWindow>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QMap<QString,interfaceDialog*> plugins;

};
#endif // MAINWINDOW_H
