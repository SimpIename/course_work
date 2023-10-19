#include "PluginBaseElementsWidget.h"
#include "ui_PluginBaseElementsWidget.h"
#include "PluginAddDialog/pluginadddialog.h"
#include <QDebug>



PluginBaseElementsWidget::PluginBaseElementsWidget(QWidget *parent) :
    QWidget(parent),
    m_autoTableValuesChanged(false),
    ui(new Ui::PluginBaseElementsWidget)
{
    ui->setupUi(this);
    ui->propertyValuesWidget->setText("Характеристики материалов");
    ui->propertyValuesWidget->getPlusButton()->hide();//propertyValueChanged
    connect(ui->propertyValuesWidget->getView()->model(), SIGNAL(dataChanged(const QModelIndex& , const QModelIndex& )),
            this, SLOT(propertyValueChanged(const QModelIndex& , const QModelIndex& )));

    ui->scalarDataWidget->setText("Экспериментальные константы");
    connect(ui->scalarDataWidget->getPlusButton(), SIGNAL(clicked()), this, SLOT(addScalarValue()));
    connect(ui->scalarDataWidget->getView()->model(), SIGNAL(dataChanged(const QModelIndex& , const QModelIndex& )),
            this, SLOT(scalarValueChanged(const QModelIndex& , const QModelIndex& )));


    ui->dependencesWidget->setText("Экспериментальные таблицы");
    connect(ui->dependencesWidget->getPlusButton(), SIGNAL(clicked()), this, SLOT(addTable()));
    connect(ui->dependencesWidget->getView(), SIGNAL(clicked(QModelIndex)), this, SLOT(updateDependenceDataWidget()));
//    connect(ui->dependencesWidget->getView()->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
//            this, SLOT(updateDependenceDataWidget()));
    connect(ui->dependencesWidget->getView()->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &PluginBaseElementsWidget::updateDependenceDataWidget);

    ui->dependenceDataWidget->setText("Табличные значения");
    connect(ui->dependenceDataWidget->getPlusButton(), SIGNAL(clicked()), this, SLOT(addTableValue()));
    connect(ui->dependenceDataWidget->getColumnButton(), SIGNAL(clicked()), this, SLOT(addTableColumn()));
    connect(ui->dependenceDataWidget->getView()->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex& )), this, SLOT(tableValuesChanged()));

    ui->dependenceDataWidget->getPlusButton()->setIcon(QIcon(":/resources/rowPlus2.png"));
    ui->dependenceDataWidget->getPlusButton()->setIconSize(QSize(26,16));
    ui->dependenceDataWidget->getColumnButton()->setIcon(QIcon(":/resources/columnPlus2.png"));
    ui->dependenceDataWidget->getColumnButton()->setIconSize(QSize(26,16));
    ui->dependenceDataWidget->getColumnButton()->show();
//    ui->dependencesWidget->getView()->horizontalHeader()->customContextMenuRequested();

    m_chart_window = new ChartWindow(windowHandle());
    QWidget *chart_container = QWidget::createWindowContainer(m_chart_window, new QWidget);
    chart_container->installEventFilter(this);
    chart_container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_chart_window->setLineWidth(3);
    m_chart_window->setPointSize(3);
    QHBoxLayout *layout = new QHBoxLayout(ui->chartWidget);
    layout->addWidget(chart_container);
}

PluginBaseElementsWidget::~PluginBaseElementsWidget()
{
    delete ui;
}


void PluginBaseElementsWidget::addScalarValue()
{
    qDebug() << "addScalarValue";
    pluginAddDialog dlg(this);
    dlg.setWindowTitle("Название новой константы");
    if (dlg.exec()==QDialog::Accepted)
    {
        QString new_scalar_name = dlg.getLineEdit()->text();
        qDebug() << "QDialog::Accepted new scalar name is " << new_scalar_name;
        if ((!new_scalar_name.isEmpty())&&(!m_interface_dialog->m_experimental_scalars.contains(new_scalar_name)))
        {
            m_interface_dialog->m_experimental_scalars[new_scalar_name]=0;
            updateScalarDataWidget();
        }
    }
}

void PluginBaseElementsWidget::addTable()
{
    qDebug() << "addTable";
    TableExperimental table;
    QString name = setTableName(table.names);
    if (m_interface_dialog->m_experimental_tables.find(name)==m_interface_dialog->m_experimental_tables.end())
        m_interface_dialog->m_experimental_tables[name]=table;
    updateDependencesWidget();
}

void PluginBaseElementsWidget::addTableColumn()
{
    qDebug() << "addTableColumn";
    QTableView *parent_table_view = ui->dependencesWidget->getView();
    QString name_table = parent_table_view->model()->data(parent_table_view->model()->index(parent_table_view->currentIndex().row(), 0)).toString();
    if (m_interface_dialog->m_experimental_tables.contains(name_table))
    {
        TableExperimental &table = m_interface_dialog->m_experimental_tables[name_table];
        pluginAddDialog dlg(this);
        dlg.setWindowTitle("Название нового столбца");
        if (dlg.exec()==QDialog::Accepted)
        {
            QString new_column_name = dlg.getLineEdit()->text();
            qDebug() << "QDialog::Accepted for table " << name_table << " new column name is " << new_column_name;
            if (!new_column_name.isEmpty())
            {
                table.names.push_back(new_column_name);
                for (auto &row : table.values)
                    row.push_back(0);
                TableExperimental buffer = table;
                m_interface_dialog->m_experimental_tables.remove(name_table);
                m_interface_dialog->m_experimental_tables[setTableName(buffer.names)]=buffer;
                updateDependencesWidget();
                updateDependenceDataWidget();
            }
        }
    }
}

void PluginBaseElementsWidget::addTableValue()
{
    qDebug() << "addTableValue";
    QTableView *parent_table_view = ui->dependencesWidget->getView();
    QString name_table = parent_table_view->model()->data(parent_table_view->model()->index(parent_table_view->currentIndex().row(), 0)).toString();
    if (m_interface_dialog->m_experimental_tables.contains(name_table))
    {
          TableExperimental &table = m_interface_dialog->m_experimental_tables[name_table];
          table.values.push_back(QVector<double>(table.names.size(),0));
          updateDependencesWidget();
    }
}

void PluginBaseElementsWidget::updatePropertyValuesWidget()
{
    qDebug() << "updateScalarDataWidget";
    m_autoTableValuesChanged = true;
    QTableView *table_view = ui->propertyValuesWidget->getView();
    QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(table_view->model());
    model->clear();
    model->setColumnCount(3);
    model->setRowCount(m_interface_dialog->m_material_properties.size());

    model->setHeaderData(0,Qt::Horizontal,QVariant("Название"));
    model->setHeaderData(1,Qt::Horizontal,QVariant("Значение"));
    model->setHeaderData(2,Qt::Horizontal,QVariant("Размерность"));
    int row_id = 0;
    for(QMap<QString, MaterialProperty>::iterator it = m_interface_dialog->m_material_properties.begin();
        it != m_interface_dialog->m_material_properties.end();
        it++, row_id++){
//        row_id = std::distance(m_interface_dialog->m_material_properties.begin(), it);
        QStandardItem *key = new QStandardItem(it.key());
        key->setEditable(false);
        model->setItem(row_id,0,key);
        MaterialProperty &property = it.value();
//        if (property.)
        model->setItem(row_id,1,new QStandardItem(QString::number(property.min)));
        model->setItem(row_id,2,new QStandardItem(QString::number(property.max)));
        model->setItem(row_id,3,new QStandardItem(QString::number(property.steps)));
        key = new QStandardItem(property.description);
        key->setEditable(false);
        model->setItem(row_id,4,key);
    }
    m_autoTableValuesChanged = false;
}

void PluginBaseElementsWidget::set_interface_dialog(AbstractInterfaceDialog *p_interface_dialog)
{
    m_interface_dialog = p_interface_dialog;
}

ChartWindow *PluginBaseElementsWidget::get_chart_window()
{
    return m_chart_window;
}

void PluginBaseElementsWidget::updateScalarDataWidget()
{
    qDebug() << "updateScalarDataWidget";
    m_autoTableValuesChanged = true;
    QTableView *table_view = ui->scalarDataWidget->getView();
    QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(table_view->model());
    model->clear();
    model->setColumnCount(2);
    model->setRowCount(m_interface_dialog->m_experimental_scalars.size());
    model->setHeaderData(0,Qt::Horizontal,QVariant("Название"));
    model->setHeaderData(1,Qt::Horizontal,QVariant("Значение"));

    for(QMap<QString,double>::iterator it = m_interface_dialog->m_experimental_scalars.begin(); it != m_interface_dialog->m_experimental_scalars.end(); it++)
    {
        int row_id = std::distance(m_interface_dialog->m_experimental_scalars.begin(), it);
        QStandardItem *key = new QStandardItem(it.key());
        key->setEditable(false);
        model->setItem(row_id,0,key);
        model->setItem(row_id,1,new QStandardItem(QString::number(it.value())));
    }
    m_autoTableValuesChanged = false;
}

void PluginBaseElementsWidget::updateDependencesWidget()
{
    qDebug() << "updateDependencesWidget";
    QTableView *table_view = ui->dependencesWidget->getView();
    QString current_table_name = table_view->model()->data(table_view->model()->index(table_view->currentIndex().row(), 0)).toString();
    QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(table_view->model());
    model->clear();
    model->setColumnCount(2);
    model->setRowCount(m_interface_dialog->m_experimental_tables.size());
    model->setHeaderData(0,Qt::Horizontal,QVariant("Зависимости"));
    model->setHeaderData(1,Qt::Horizontal,QVariant("Строки"));
    for(QMap<QString, TableExperimental>::iterator it = m_interface_dialog->m_experimental_tables.begin(); it != m_interface_dialog->m_experimental_tables.end(); it++)
    {
        int row_id = std::distance(m_interface_dialog->m_experimental_tables.begin(), it);
        QStandardItem *key = new QStandardItem(it.key());
        key->setEditable(false);
        model->setItem(row_id,0,key);
        QStandardItem *rows = new QStandardItem(QString::number(it.value().values.size()));
        rows->setEditable(false);
        model->setItem(row_id,1,rows);
    }
    QList<QStandardItem*> items = model->findItems(current_table_name,Qt::MatchExactly,0);
    if (!items.isEmpty())
        table_view->setCurrentIndex(items.first()->index());
}

void PluginBaseElementsWidget::updateDependenceDataWidget()
{
    qDebug() << "updateDependenceDataWidget";
    m_autoTableValuesChanged = true;
    QTableView *parent_table_view = ui->dependencesWidget->getView();
    QTableView *table_view = ui->dependenceDataWidget->getView();
    QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(table_view->model());
    model->clear();
    QString name_table = parent_table_view->model()->data(parent_table_view->model()->index(parent_table_view->currentIndex().row(), 0)).toString();
    if (m_interface_dialog->m_experimental_tables.contains(name_table)) {
        TableExperimental &current_table = m_interface_dialog->m_experimental_tables[name_table];
        std::sort(current_table.values.begin(), current_table.values.end(), [](QVector<double> a, QVector<double> b)->bool{
            if (a.isEmpty()) return true;
            if (b.isEmpty()) return false;
            return (a[0]<b[0]);
        });
        model->setColumnCount(current_table.names.size());
        for (int col_id = 0; col_id < current_table.names.size(); col_id++)
            model->setHeaderData(col_id,Qt::Horizontal,QVariant(current_table.names[col_id]));
        model->setRowCount(current_table.values.size());
        for (int row_id = 0; row_id < current_table.values.size(); row_id++)
            for (int col_id = 0; col_id < current_table.values[row_id].size(); col_id++)
                model->setItem(row_id,col_id,new QStandardItem(QString::number(current_table.values[row_id][col_id])));

        m_chart_window->clear();


        if (!current_table.values.isEmpty())
            if (!current_table.values[0].isEmpty()){
            {
                charts::Table o_table;
                charts::Column x_column (current_table.names[0], QList<Real>(current_table.values.size()));
                charts::Column y_column (current_table.names[1], QList<Real>(current_table.values.size()));
                for (unsigned int i = 0; i < current_table.values.size(); ++i) {
                    x_column.rows[i] = current_table.values[i][0];
                    y_column.rows[i] = current_table.values[i][1];
                }

                o_table.columns.push_back(x_column);
                o_table.columns.push_back(y_column);

                m_chart_window->addData(o_table);
                m_chart_window->addCoordinateColumns(0, 1);
            }

//            {
//                charts::Table o_table;

//                charts::Column strains_column("Strain", QList<Real>(arg_x.size()+1));
//                charts::Column stresses_column("Stress", QList<Real>(arg_x.size()+1));
//                strains_column.rows[0] = 0;
//                stresses_column.rows[0] = 0;
//                for (unsigned int i = 0; i < arg_x.size(); ++i) {
//                    strains_column.rows[i+1] = arg_x[i];
//                    stresses_column.rows[i+1] = func.comp(arg_x[i]);
//                }

//                o_table.columns.push_back(strains_column);
//                o_table.columns.push_back(stresses_column);

//                m_chart_window->addData(o_table);
//                m_chart_window->addCoordinateColumns(0, 1);
//            }
        }



    }
    m_autoTableValuesChanged = false;



}

void PluginBaseElementsWidget::tableValuesChanged()
{
    if (!m_autoTableValuesChanged)
    {
        qDebug() << "applyTableValues";
        QTableView *parent_table_view = ui->dependencesWidget->getView();
        QTableView *table_view = ui->dependenceDataWidget->getView();
        QString name_table = parent_table_view->model()->data(parent_table_view->model()->index(parent_table_view->currentIndex().row(), 0)).toString();
        if (m_interface_dialog->m_experimental_tables.contains(name_table))
        {
            TableExperimental &current_table = m_interface_dialog->m_experimental_tables[name_table];
            QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(table_view->model());
            for (int row_id = 0; row_id < model->rowCount(); row_id++)
                for (int col_id = 0; col_id < model->columnCount(); col_id++)
                    current_table.values[row_id][col_id] = model->data(model->index(row_id,col_id)).toDouble();
        }
//        updateDependenceDataWidget();

    }
}

void PluginBaseElementsWidget::scalarValueChanged(const QModelIndex &p_topLeft, const QModelIndex &p_botomRight)
{
    if ((!m_autoTableValuesChanged)&&(p_topLeft.column()==1))
    {
        qDebug() << "scalarValueChanged";
        QTableView *table_view = ui->scalarDataWidget->getView();
        QString name_scalar = table_view->model()->data(table_view->model()->index(p_topLeft.row(), 0)).toString();
        if (m_interface_dialog->m_experimental_scalars.contains(name_scalar))
        {
            double &scalar_value = m_interface_dialog->m_experimental_scalars[name_scalar];
            scalar_value = table_view->model()->data(p_topLeft).toDouble();
            updateScalarDataWidget();
        }
    }
}

void PluginBaseElementsWidget::propertyValueChanged(const QModelIndex &p_topLeft, const QModelIndex &p_botomRight)
{
    if ((!m_autoTableValuesChanged))
    {
        qDebug() << "propertyValueChanged";
        QTableView *table_view = ui->propertyValuesWidget->getView();
        QString name_property = table_view->model()->data(table_view->model()->index(p_topLeft.row(), 0)).toString();
        if (m_interface_dialog->m_material_properties.contains(name_property))
        {
            MaterialProperty &property = m_interface_dialog->m_material_properties[name_property];
            if (p_topLeft.column()==1)
                property.min = table_view->model()->data(p_topLeft).toDouble();
            else if (p_topLeft.column()==2)
                property.max = table_view->model()->data(p_topLeft).toDouble();
            if (p_topLeft.column()==3)
                property.steps = table_view->model()->data(p_topLeft).toDouble();
        }
        updatePropertyValuesWidget();
    }

}

void PluginBaseElementsWidget::on_acceptButton_clicked()
{
    emit accept();
}


void PluginBaseElementsWidget::on_loadTextButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Открыть экспериментальные данные",
                                                    QDir::current().path(),
                                                    "*.txt *.xlsx *.xml *.json");
    QString suffix = QFileInfo(filename).suffix();
    if      (suffix == "txt" ) loadDataFromText(filename);
    else if (suffix == "json") loadDataFromJson(filename);
    else if (suffix == "xml" ) loadDataFromXML (filename);
    else if (suffix == "xlsx") loadDataFromXLSX(filename);
}


void PluginBaseElementsWidget::on_saveTextButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    "Открыть экспериментальные данные",
                                                    QDir::current().path(),
                                                    "*.txt *.xlsx *.xml *.json");
    QString suffix = QFileInfo(filename).suffix();
    if      (suffix == "txt" ) saveDataToText(filename);
    else if (suffix == "json") saveDataToJson(filename);
    else if (suffix == "xml" ) saveDataToXML (filename);
    else if (suffix == "xlsx") saveDataToXLSX(filename);
}

void PluginBaseElementsWidget::loadDataFromText(QString p_filename)
{
    qDebug() << "loadDataFromText";
    m_interface_dialog->m_experimental_scalars.clear();
    m_interface_dialog->m_experimental_tables.clear();
    QFile input_file(p_filename);
    input_file.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream text(&input_file);
    QString line = text.readLine();
    while (!text.atEnd())
    {
        /// \brief Пропуск комментария
        if (line.contains("#"))
        {

        }
        /// \brief Чтение скаляров
        else if (line.contains("[scalar]"))
        {
            QString name;
            double value;
            text >> name;
            text >> value;
            if (!name.isEmpty()) m_interface_dialog->m_experimental_scalars[name] = value;
        }
        /// \brief Чтение таблиц
        else if (line.contains("[table]"))
        {
            int rows_count = 0;
            int colums_count = 0;
            TableExperimental table;
            QString name;
            double value;
            /// \brief Чтение параметров таблицы
            text >> rows_count;
            text >> colums_count;
            /// \brief Чтение заголовков столбцов
            for (int j = 0; j < colums_count; j++)
            {
                text >> name;
                table.names.push_back(name);
            }
            /// \brief Чтение строк таблицы
            for (int i = 0; i < rows_count; i++)
            {
                QVector<double> row;
                /// \brief Чтение строки таблицы по столбцам
                for (int j = 0; j < colums_count; j++)
                {
                    text >> value;
                    row.push_back(value);
                }
                table.values.push_back(row);
            }
            /// \brief Составление ключа для контейнера с таблицами
            m_interface_dialog->m_experimental_tables[setTableName(table.names)] = table;
        };
        line = text.readLine();
    }
    input_file.close();
    updateScalarDataWidget();
    updateDependencesWidget();

}

void PluginBaseElementsWidget::loadDataFromJson(QString p_filename)
{

}

void PluginBaseElementsWidget::loadDataFromXML(QString p_filename)
{

}

void PluginBaseElementsWidget::loadDataFromXLSX(QString p_filename)
{
    qDebug() << "loadDataFromExcel";
    QXlsx::Document xlsxDoc(p_filename);
    int sheetIndexNumber = 0;
    foreach( QString currentSheetName, xlsxDoc.sheetNames() )
    {
        // get current sheet
        QXlsx::AbstractSheet* currentSheet = xlsxDoc.sheet( currentSheetName );
        if ( NULL == currentSheet )
            continue;

        // get full cells of current sheet
        int maxRow = -1;
        int maxCol = -1;
        currentSheet->workbook()->setActiveSheet( sheetIndexNumber );
        QXlsx::Worksheet* wsheet = (QXlsx::Worksheet*) currentSheet->workbook()->activeSheet();
        if ( NULL == wsheet )
            continue;

        QString strSheetName = wsheet->sheetName(); // sheet name

        QVector<QXlsx::CellLocation> clList = wsheet->getFullCells( &maxRow, &maxCol );

        QVector< QVector<QString> > cellValues;
        for (int rc = 0; rc < maxRow; rc++)
        {
            QVector<QString> tempValue;
            for (int cc = 0; cc < maxCol; cc++)
            {
                tempValue.push_back(QString(""));
            }
            cellValues.push_back(tempValue);
        }
        for ( int ic = 0; ic < clList.size(); ++ic )
        {
            QXlsx::CellLocation cl = clList.at(ic); // cell location

            int row = cl.row - 1;
            int col = cl.col - 1;

            QSharedPointer<QXlsx::Cell> ptrCell = cl.cell; // cell pointer

            // value of cell
            QVariant var = cl.cell.data()->value();
            QString str = var.toString();

            cellValues[row][col] = str;
        }
        for (int rc = 0; rc < maxRow; rc++)
        {
            for (int cc = 0; cc < maxCol; cc++)
            {
                if (cellValues[rc][cc].contains("#"))
                {

                }
                /// \brief Чтение скаляров
                else if (cellValues[rc][cc].contains("[scalar]"))
                {
                    QString name;
                    double value;
                    name = cellValues[rc + 1][cc];
                    rc++;
                    value = cellValues[rc + 1][cc].toDouble();
                    rc++;
                    //text >> name;
                    //text >> value;
                    if (!name.isEmpty()) m_interface_dialog->m_experimental_scalars[name] = value;
                }
                /// \brief Чтение таблиц
                else if (cellValues[rc][cc].contains("[table]"))
                {
                    int rows_count = 0;
                    int colums_count = 0;
                    TableExperimental table;
                    QString name;
                    double value;
                    /// \brief Чтение параметров таблицы
                    QString s = cellValues[rc + 1][cc];
                    rc++;
                    char delim = ' ';
                    int pos = 0;
                    //std::string token;
                    pos = s.indexOf(delim);
                    rows_count = s.first(pos).toDouble();
                    for (int t = 0; t < pos + 1; t++){
                    s.remove(0, 1);
                    }
                    pos = s.indexOf(delim);
                    colums_count = s.first(pos).toDouble();
                    for (int t = 0; t < pos + 1; t++){
                    s.remove(0, 1);
                    }

                    //text >> rows_count;
                    //text >> colums_count;
                    /// \brief Чтение заголовков столбцов
                    for (int j = 0; j < colums_count; j++)
                    {
                    if (j != colums_count - 1){
                        pos = s.indexOf(delim);
                        name = s.first(pos);
                        table.names.push_back(name);
                        qDebug() << name;
                        for (int t = 0; t < pos + 1; t++){
                            s.remove(0, 1);
                        }
                        //text >> name;
                        qDebug() << "loadprocess13";
                    }
                    else{
                        name = s;
                        table.names.push_back(name);
                    }
                    }

                    qDebug() << "loadprocess131";
                    /// \brief Чтение строк таблицы
                    for (int i = 0; i < rows_count; i++)
                    {
                    rc++;
                    s = cellValues[rc][cc];
                    QVector<double> row;
                    /// \brief Чтение строки таблицы по столбцам
                    for (int j = 0; j < colums_count; j++)
                    {
                        //text >> value;
                        if (j != colums_count - 1){
                        pos = s.indexOf(delim);
                        value = s.first(pos).toDouble();
                        for (int t = 0; t < pos + 1; t++){
                            s.remove(0, 1);
                        }
                        row.push_back(value);
                        }
                        else{
                        value = s.toDouble();
                        row.push_back(value);
                        }
                    }
                    table.values.push_back(row);
                    }
                    /// \brief Составление ключа для контейнера с таблицами
                    m_interface_dialog->m_experimental_tables[setTableName(table.names)] = table;
                };
                //line = text.readLine();
            }
            updateScalarDataWidget();
            updateDependencesWidget();
        }
    }

    sheetIndexNumber++;
}




void PluginBaseElementsWidget::saveDataToText(QString p_filename)
{
    qDebug() << "saveDataToText";
    QFile output_file(p_filename);
    output_file.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream text(&output_file);
    /// \brief Запись скаляров
    for (QMap<QString, double>::iterator it = m_interface_dialog->m_experimental_scalars.begin(); it != m_interface_dialog->m_experimental_scalars.end(); it++)
    {
        text << "[scalar]" << Qt::endl;
        text << it.key() << Qt::endl;
        text << it.value() << Qt::endl;
        text << Qt::endl;
    }
    /// \brief Запись таблиц
    for (QMap<QString, TableExperimental >::iterator it = m_interface_dialog->m_experimental_tables.begin(); it != m_interface_dialog->m_experimental_tables.end(); it++)
    {
        text << "[table]" << Qt::endl;
        TableExperimental &table = *it;
        text << table.values.size() << " " << table.names.size();
        /// \brief Запись заголовков
        for (QVector<QString>::iterator name_it = table.names.begin(); name_it != table.names.end(); name_it++)
            text << " " << *name_it;
        text << Qt::endl;
        /// \brief Запись строк
        for (QVector<QVector<double> >::iterator row_it = table.values.begin(); row_it != table.values.end(); row_it++)
        {
            QVector<double> &row = *row_it;
            /// \brief Запись строки по столбцам
            for(QVector<double>::iterator col_it = row.begin(); col_it != row.end(); col_it++)
                text << (col_it == row.begin()?"":" ") << *col_it;
            text << Qt::endl;
        }
        text << Qt::endl;
    }
    output_file.close();
}

void PluginBaseElementsWidget::saveDataToJson(QString p_filename)
{

}

void PluginBaseElementsWidget::saveDataToXML(QString p_filename)
{

}

void PluginBaseElementsWidget::saveDataToXLSX(QString p_filename)
{
    qDebug() << "saveDataToXlsx";
    QXlsx::Document xlsxDoc(p_filename);
//    output_file.open(QIODevice::WriteOnly|QIODevice::Text);
//    QTextStream text(&output_file);
    /// \brief Запись скаляров
    int k = 1;
    for (QMap<QString, double>::iterator it = m_interface_dialog->m_experimental_scalars.begin(); it != m_interface_dialog->m_experimental_scalars.end(); it++)
    {
        xlsxDoc.write(k, 1, "[scalar]");
        k++;
        xlsxDoc.write(k, 1, it.key());
        k++;
        xlsxDoc.write(k, 1, it.value());
//        text << "[scalar]" << Qt::endl;
//        text << it.key() << Qt::endl;
//        text << it.value() << Qt::endl;
//        text << Qt::endl;
    }
    /// \brief Запись таблиц
    for (QMap<QString, TableExperimental >::iterator it = m_interface_dialog->m_experimental_tables.begin(); it != m_interface_dialog->m_experimental_tables.end(); it++)
    {
        xlsxDoc.write(k, 1, "[table]");
        k++;
//        text << "[table]" << Qt::endl;
        TableExperimental &table = *it;
        xlsxDoc.write(k, 1, table.values.size());
        xlsxDoc.write(k, 1, table.names.size());
        k++;
//        text << table.values.size() << " " << table.names.size();
        /// \brief Запись заголовков
        int i = 1;
        for (QVector<QString>::iterator name_it = table.names.begin(); name_it != table.names.end(); name_it++){
            xlsxDoc.write(k, i, *name_it);
            i++;
        }
        k++;
//            text << " " << *name_it;
//        text << Qt::endl;
        /// \brief Запись строк
        for (QVector<QVector<double> >::iterator row_it = table.values.begin(); row_it != table.values.end(); row_it++)
        {
            QVector<double> &row = *row_it;
            i = 1;
            /// \brief Запись строки по столбцам
            for(QVector<double>::iterator col_it = row.begin(); col_it != row.end(); col_it++){
                xlsxDoc.write(k, i, *col_it);
                i++;
            }


//                text << (col_it == row.begin()?"":" ") << *col_it;
            k++;
//            text << Qt::endl;
        }
        k++;
//        text << Qt::endl;
    }
    xlsxDoc.saveAs("my_doc.xlsx");
//    output_file.close();
}

