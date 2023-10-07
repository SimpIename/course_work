#ifndef CHART_DATA_H
#define CHART_DATA_H

#include <QString>
#include <QList>

#include <Defs>

namespace charts
{
/// Столбец таблицы

    struct Column
    {
        /// Нзавание столбца
        QString id;

        /// Строки столбца
        QList<Real> rows;

        Column() = default;

        inline Column(const QString     &p_id,
                      const QList<Real> &p_rows = {}) noexcept:
            id(p_id),
            rows(p_rows)
        {}

        inline bool operator ==(const Column &p_column) const noexcept
        {
            return p_column.id == id && p_column.rows == rows;
        }
    };

/// Столбцы таблицы

    using Columns = QList<Column>;

/// Таблица значений

    struct Table
    {
        /// Название таблицы значений
        QString id;

        /// Cтолбцы таблицы
        Columns columns;

        Table() = default;

        inline Table(const QString &p_id,
                     const Columns &p_columns = {}) noexcept:
            id(p_id),
            columns(p_columns)
        {}
    };

/// Тип данных графика

    struct Chart
    {
        /// Номер таблицы
        int table_id;

        /// Номер столбца таблицы для оси X
        int column_x;

        /// Номер столбца таблицы для оси Y
        int column_y;

        inline bool operator==(const Chart &p_chart) const noexcept
        {
            return (table_id == p_chart.table_id) &&
                   (column_x == p_chart.column_x) &&
                   (column_y == p_chart.column_y);
        }
    };
}

#endif // CHART_DATA_H
