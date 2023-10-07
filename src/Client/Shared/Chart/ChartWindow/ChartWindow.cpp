// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "ChartWindow.h"
#include <QGraphicsItem>
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <cmath>

#include <QDebug>
#include <ChangePointDialog>

unsigned int ChartWindow::m_id_counter = 0;

ChartWindow::ChartWindow(QWindow *p_parent):
QOpenGLWindow(NoPartialUpdate, p_parent),
m_range_x{-1.0, 1.0, {}},
m_range_y{-1.0, 1.0, {}},
//      m_frame_margin_left(30),
//      m_frame_margin_right(10),
//      m_frame_margin_top(10),
//      m_frame_margin_bottom(30),
m_frame_margin_left(0),
m_frame_margin_right(0),
m_frame_margin_top(0),
m_frame_margin_bottom(0),
m_chosen_point(-1),
m_chosen_chart(-1),
m_is_mouse_pressed(false),
m_is_zoom_frame(false),
m_is_initialized(false),
m_line_width(4),
m_point_size(8),
m_is_fixed(true),
m_is_show_points(true),
m_label_style(top),
m_log_tick_mark_style(power_of_ten),
m_is_dynamic_scale(true),
m_is_show_chart_id(false),
m_is_use_cells(false),
m_cells_count(100)
{
    QSurfaceFormat format;
    format.setVersion(3, 2);
    setFormat(format);
    m_id_counter++;
    m_id = m_id_counter;
}


void ChartWindow::findValueRange(const QList<Real> &p_values,
                                 ValueRange        &o_range) const
{
    o_range.min = std::numeric_limits<Real>::max();
    o_range.max = std::numeric_limits<Real>::lowest();

    for (const auto &value : p_values) {
        if (o_range.min > value) {
            o_range.min = value;
        }
        if (o_range.max < value) {
            o_range.max = value;
        }
    }

    if (m_is_use_cells) {
        o_range.cells.resize(m_cells_count);
        for (auto &cell : o_range.cells) {
            cell.clear();
        }

        for (int i = 0; i < p_values.size(); ++i) {
            o_range.cells[o_range.getCellIndex(p_values[i], m_cells_count)].append(i);
        }
    }
}


inline void ChartWindow::updateRange(int p_table_index, int p_column_index)
{
    findValueRange(m_tables[p_table_index].columns[p_column_index].rows,
                   m_ranges[p_table_index][p_column_index]);
}


void ChartWindow::findMinMax()
{
    m_range_x.min = std::numeric_limits<Real>::max();
    m_range_y.min = std::numeric_limits<Real>::max();
    m_range_x.max = std::numeric_limits<Real>::lowest();
    m_range_y.max = std::numeric_limits<Real>::lowest();

    for (const auto &chart : m_charts) {
        const auto& local_range_x = m_ranges[chart.table_id][chart.column_x];
        const auto& local_range_y = m_ranges[chart.table_id][chart.column_y];

        if (m_log_scale_x) {
            if (local_range_x.min > 0) {
                if (m_range_x.min > local_range_x.min) {
                    m_range_x.min = local_range_x.min;
                }
                if (m_range_x.max < local_range_x.max) {
                    m_range_x.max = local_range_x.max;
                }
            } else if (local_range_x.max > 0) {
                for (const auto &value : m_tables[chart.table_id].columns[chart.column_x].rows) {
                    if (value <= 0)
                        continue;

                    if (m_range_x.min > value) {
                        m_range_x.min = value;
                    }
                    if (m_range_x.max < value) {
                        m_range_x.max = value;
                    }
                }
            }
        } else {
            if (m_range_x.min > local_range_x.min) {
                m_range_x.min = local_range_x.min;
            }
            if (m_range_x.max < local_range_x.max) {
                m_range_x.max = local_range_x.max;
            }
        }
        if (m_log_scale_y) {
            if (local_range_y.min > 0) {
                if (m_range_y.min > local_range_y.min) {
                    m_range_y.min = local_range_y.min;
                }
                if (m_range_y.max < local_range_y.max) {
                    m_range_y.max = local_range_y.max;
                }
            } else if (local_range_y.max > 0) {
                for (const auto &value : m_tables[chart.table_id].columns[chart.column_y].rows) {
                    if (value <= 0)
                        continue;

                    if (m_range_y.min > value) {
                        m_range_y.min = value;
                    }
                    if (m_range_y.max < value) {
                        m_range_y.max = value;
                    }
                }
            }
        } else {
            if (m_range_y.min > local_range_y.min) {
                m_range_y.min = local_range_y.min;
            }
            if (m_range_y.max < local_range_y.max) {
                m_range_y.max = local_range_y.max;
            }
        }
    }

    if (m_log_scale_x) {
        if (m_range_x.min == std::numeric_limits<Real>::max())
            m_range_x.min = 1;
        if (m_range_x.max == std::numeric_limits<Real>::lowest())
            m_range_x.max = 10;
        m_range_x.min = log10(m_range_x.min);
        m_range_x.max = log10(m_range_x.max);
    }

    if (qFuzzyCompare(float(m_range_x.min), float(m_range_x.max))) {
        if (qFuzzyIsNull(m_range_x.min)) {
            m_range_x.min -= 1;
            m_range_x.max += 1;
        } else {
            m_range_x.min -= fabs(m_range_x.min);
            m_range_x.max += fabs(m_range_x.max);
        }
    }

    if (m_log_scale_y) {
        if (m_range_y.min == std::numeric_limits<Real>::max())
            m_range_y.min = 1;
        if (m_range_y.max == std::numeric_limits<Real>::lowest())
            m_range_y.max = 10;
        m_range_y.min = log10(m_range_y.min);
        m_range_y.max = log10(m_range_y.max);
    }

    if (qFuzzyCompare(float(m_range_y.min), float(m_range_y.max))) {
        if (qFuzzyIsNull(m_range_y.min)) {
            m_range_y.min -= 1;
            m_range_y.max += 1;
        } else {
            m_range_y.min -= fabs(m_range_y.min);
            m_range_y.max += fabs(m_range_y.max);
        }
    }

    Real xc = (m_range_x.min+m_range_x.max)/2.0;
    m_range_x.min = xc + (m_range_x.min-xc)*1.1;
    m_range_x.max = xc + (m_range_x.max-xc)*1.1;

    Real yc = (m_range_y.min+m_range_y.max)/2.0;
    m_range_y.min = yc + (m_range_y.min-yc)*1.1;
    m_range_y.max = yc + (m_range_y.max-yc)*1.1;
}


void ChartWindow::clear()
{
    makeCurrent();
    m_chosen_point = -1;
    m_chosen_chart = -1;

    m_tables.clear();
    m_charts.clear();
    m_ranges.clear();

    if (m_is_initialized)
        m_lines.clear();

    update();
}


void ChartWindow::setData(const QList<charts::Table> &p_tables)
{
    m_tables.clear();
    m_charts.clear();
    m_ranges.clear();
    if (m_is_initialized) {
        clearLineBuffers();
    }
    addData(p_tables);
    update();
}


void ChartWindow::setData(const charts::Table &p_table)
{
    m_tables.clear();
    m_charts.clear();
    m_ranges.clear();
    if (m_is_initialized) {
        clearLineBuffers();
    }
    addData(p_table);
    update();
}


void ChartWindow::setData(int                       p_table_index,
                          const QList<QList<Real>> &p_columns_data,
                          bool                      p_update_ranges)
{
    if (p_table_index < 0 || p_table_index >= m_tables.size())
        return;

    const int columns_count = m_tables[p_table_index].columns.size();

    if (p_columns_data.size() != columns_count)
        return;

    auto &columns_data = m_tables[p_table_index].columns;

    for (int i = 0; i < columns_count; ++i) {
        columns_data[i].rows = p_columns_data[i];
        updateRange(p_table_index, i);
    }

    bool updated{ false };

    for (int i = 0; i < m_charts.size(); ++i) {
        if (m_charts[i].table_id == p_table_index) {
            if (m_is_initialized)
                resizeAndUpdateBufferData(i);

            updated = true;
        }
    }

    if (updated) {
        if (p_update_ranges && m_is_dynamic_scale)
            findMinMax();

        update();
    }
}


void ChartWindow::setData(const charts::Chart &p_chart,
                          const QList<Real>   &p_column_x_rows,
                          const QList<Real>   &p_column_y_rows,
                          bool                 p_update_ranges)
{
    if (!valid(p_chart))
        return;

    const auto [table_id, column_x, column_y] = p_chart;
    auto &columns = m_tables[table_id].columns;

    columns[column_x].rows = p_column_x_rows;
    updateRange(table_id, column_x);

    columns[column_y].rows = p_column_y_rows;
    updateRange(table_id, column_y);

    bool updated{ false };

    for (int i = 0; i < m_charts.size(); ++i) {
        if (m_charts[i].table_id == table_id) {
            if (m_is_initialized)
                resizeAndUpdateBufferData(i);

            updated = true;
        }
    }

    if (updated) {
        if (p_update_ranges && m_is_dynamic_scale)
            findMinMax();

        update();
    }
}


void ChartWindow::setData(int                          p_table_index,
                          const QList<charts::Column> &p_columns,
                          bool                         p_update_ranges)
{
    if (p_table_index < 0 || p_table_index >= m_tables.size())
        return;

    m_tables[p_table_index].columns = p_columns;
    const int columns_count = m_tables[p_table_index].columns.size();

    QList<ValueRange> ranges(p_columns.size());

    for (int i = 0; i < columns_count; ++i)
        findValueRange(p_columns[i].rows, ranges[i]);

    m_ranges[p_table_index] = ranges;

    bool updated{ false };

    for (int i = 0; i < m_charts.size(); ++i) {
        if (m_charts[i].table_id == p_table_index) {
            if (m_is_initialized)
                resizeAndUpdateBufferData(i);

            updated = true;
        }
    }

    if (updated) {
        if (p_update_ranges && m_is_dynamic_scale)
            findMinMax();

        update();
    }
}


void ChartWindow::addData(const charts::Table &p_table)
{
    m_tables.append(p_table);
    QList<ValueRange> new_ranges(p_table.columns.size());
    for (int i = 0; i < p_table.columns.size(); ++i)
        findValueRange(p_table.columns[i].rows, new_ranges[i]);

    m_ranges.append(new_ranges);
}


void ChartWindow::addData(const QList<charts::Table> &p_tables)
{
    m_tables.append(p_tables);
    for (const auto &table : p_tables) {
        QList<ValueRange> new_ranges(table.columns.size());
        for (int i = 0; i < table.columns.size(); ++i) {
            findValueRange(table.columns[i].rows, new_ranges[i]);
        }
        m_ranges.append(new_ranges);
    }
}


void ChartWindow::addRow(int p_table_index, int p_next_row_index, const QList<Real> &p_row)
{
    if (p_table_index < 0 || p_table_index >= m_tables.size())
        return;

    if (p_next_row_index < 0 ||
        p_next_row_index > m_tables[p_table_index].columns.first().rows.size())
        return;

    if (p_row.empty()) {
        for (int i = 0; i < m_tables[p_table_index].columns.size(); ++i) {
            auto &column_rows = m_tables[p_table_index].columns[i].rows;
            if (p_next_row_index == 0) {
                Real new_value
                    = 2.0 * column_rows[p_next_row_index] - column_rows[p_next_row_index + 1];
                column_rows.insert(p_next_row_index,
                                   new_value);
            } else if (p_next_row_index == column_rows.size()) {
                Real new_value
                    = 2.0 * column_rows[p_next_row_index - 1] - column_rows[p_next_row_index - 2];
                column_rows.insert(p_next_row_index, new_value);
            } else {
                column_rows.insert(p_next_row_index,
                                   (column_rows[p_next_row_index - 1] +
                                    column_rows[p_next_row_index]) / 2.0);
            }
            updateRange(p_table_index, i);
        }
    } else {
        if (p_row.size() != m_tables[p_table_index].columns.size())
            return;

        if (p_row.size() != m_tables[p_table_index].columns.size())
            return;
        for (int i = 0; i < m_tables[p_table_index].columns.size(); ++i) {
            m_tables[p_table_index].columns[i].rows.insert(p_next_row_index, p_row[i]);
            updateRange(p_table_index, i);
        }
    }

    for (int i = 0; i < m_charts.size(); ++i) {
        if (m_charts[i].table_id == p_table_index) {
            if (m_is_initialized) {
                resizeAndUpdateBufferData(i);
            }
        }
    }
//    findMinMax();
    update();
}


void ChartWindow::removeRow(int p_table_index, int p_row_index)
{
    if (p_table_index < 0 || p_table_index >= m_tables.size())
        return;

    if (m_tables[p_table_index].columns.empty())
        return;

    int row_count = m_tables[p_table_index].columns.first().rows.size();
    if (row_count <= 2)
        return;

    if (p_row_index < 0 || p_row_index >= row_count)
        return;

    for (int i = 0; i < m_tables[p_table_index].columns.size(); ++i) {
        m_tables[p_table_index].columns[i].rows.remove(p_row_index);
        updateRange(p_table_index, i);
    }

    for (int i = 0; i < m_charts.size(); ++i) {
        if (m_charts[i].table_id == p_table_index) {
            if (m_is_initialized) {
                resizeAndUpdateBufferData(i);
            }
        }
    }
//    findMinMax();
    update();
}


void ChartWindow::setLabels(int p_table_index, const QStringList &p_labels)
{
    if (p_table_index < 0 || p_table_index >= m_tables.size())
        return;

    if (p_labels.size() != m_tables[p_table_index].columns.size())
        return;

    for (int i = 0; i < p_labels.size(); ++i) {
        m_tables[p_table_index].columns[i].id = p_labels[i];
    }
}


void ChartWindow::setLabel(int p_table_index, int p_column_index, const QString &p_label)
{
    if (p_table_index < 0 || p_table_index >= m_tables.size())
        return;
    if (p_column_index < 0 || p_column_index >= m_tables[p_table_index].columns.size())
        return;

    m_tables[p_table_index].columns[p_column_index].id = p_label;
}


bool ChartWindow::valid(const charts::Chart &p_chart) const noexcept
{
    if (p_chart.table_id < 0 || p_chart.table_id >= m_tables.size())
        return false;

    const int columns_count = m_tables[p_chart.table_id].columns.size();

    if (p_chart.column_x < 0 || p_chart.column_x >= columns_count ||
        p_chart.column_y < 0 || p_chart.column_y >= columns_count)
        return false;

    return true;
}


void ChartWindow::setChart(const charts::Chart &p_chart)
{
    if (!valid(p_chart))
        return;

    if (!m_charts.empty()) {
        m_charts.clear();
        if (m_is_initialized) {
            clearLineBuffers();
        }
    }

    m_charts.append(p_chart);
    findMinMax();

    if (m_is_initialized) {
        addLineBuffer(p_chart);
        update();
    }
}


void ChartWindow::setCharts(const QList<charts::Chart> &p_charts)
{
    if (!m_charts.empty()) {
        m_charts.clear();
        if (m_is_initialized) {
            clearLineBuffers();
        }
    }

    m_charts = p_charts;
    findMinMax();

    if (m_is_initialized) {
        for (const auto& chart : m_charts) {
            addLineBuffer(chart);
        }
        update();
    }
}


void ChartWindow::addChart(const charts::Chart &p_chart)
{
    if (!valid(p_chart))
        return;

    if (m_charts.contains(p_chart))
        return;

    m_charts.append(p_chart);
    findMinMax();

    if (m_is_initialized) {
        addLineBuffer(p_chart);
        update();
    }
}


void ChartWindow::setRangeX(Real p_minimum, Real p_maximum)
{
    if (p_minimum <= p_maximum) {
        m_minimum_x = p_minimum;
        m_maximum_x = p_maximum;
    }
}


void ChartWindow::setRangeY(Real p_minimum, Real p_maximum)
{
    if (p_minimum <= p_maximum) {
        m_minimum_y = p_minimum;
        m_maximum_y = p_maximum;
    }
}


//void ChartWindow::removeCoordinateColumns(int p_table, int p_coord1, int p_coord2)
//{
//    Chart chart{p_table, p_coord1, p_coord2};

//    int index = m_charts.indexOf(chart);

//    if (index == -1)
//        return;

//    m_charts.remove(index);
//}

void ChartWindow::setFixed(bool p_is_fixed)
{
    m_is_fixed = p_is_fixed;
    update();
}


void ChartWindow::setUseSelectionCells(bool p_use)
{
    bool use_cells_changed = m_is_use_cells != p_use;
    m_is_use_cells = p_use;

    if (m_is_use_cells && use_cells_changed) {
        const int tables_count = m_tables.size();
        int columns_count;

        for (int table_index = 0; table_index < tables_count; ++table_index) {
            columns_count = m_tables[table_index].columns.size();

            for (int column_index = 0; column_index < columns_count; ++column_index)
                updateRange(table_index, column_index);
        }
    }
}


void ChartWindow::setShowPoints(bool p_is_show)
{
    m_is_show_points = p_is_show;
    update();
}


void ChartWindow::setLabelStyle(ChartWindow::LabelStyle p_style)
{
    m_label_style = p_style;
    update();
}


void ChartWindow::setLogTickMarkStyle(LogTickMarkStyle p_style)
{
    m_log_tick_mark_style = p_style;
    update();
}


void ChartWindow::setLineWidth(float p_line_width)
{
    m_line_width = p_line_width;
    update();
}


void ChartWindow::setPointSize(float p_point_size)
{
    m_point_size = p_point_size;
    update();
}


void ChartWindow::setShowChartId(bool p_is_show)
{
    m_is_show_chart_id = p_is_show;
    update();
}


QColor ChartWindow::getChartColor(int p_chart_index) const
{
    return m_colors[p_chart_index % m_colors.size()];
}


inline Real ChartWindow::projectX(Real p_obj_x) const
{
//    return (p_obj_x-m_range_x.min)/(m_range_x.max-m_range_x.min)*width();
    return (p_obj_x-m_range_x.min)/(m_range_x.max-m_range_x.min)
            * (width()-m_frame_margin_left-m_frame_margin_right)+m_frame_margin_left;
}


inline Real ChartWindow::projectY(Real p_obj_y) const
{
//    return (1-(p_obj_y-m_range_y.min)/(m_range_y.max-m_range_y.min))*height();
    return (1-(p_obj_y-m_range_y.min)/(m_range_y.max-m_range_y.min))
            * (height()-m_frame_margin_top-m_frame_margin_bottom)+m_frame_margin_top;
}


inline Real ChartWindow::unProjectX(Real p_win_x) const
{
//    return p_win_x * (m_xMax - m_xMin);

//    return p_win_x/width()*(m_range_x.max-m_range_x.min) + m_range_x.min;
    return (p_win_x-m_frame_margin_left)
            /(width()-m_frame_margin_left-m_frame_margin_right)*(m_range_x.max-m_range_x.min)
            + m_range_x.min;
}


inline Real ChartWindow::unProjectY(Real p_win_y) const
{
//    return (height() - p_win_y) * (m_yMin - m_yMax);
//    return (1-p_win_y/height())*(m_range_y.max-m_range_y.min) + m_range_y.min;
    return (1-(p_win_y-m_frame_margin_top)
            /(height()-m_frame_margin_top-m_frame_margin_bottom))*(m_range_y.max-m_range_y.min)
            +  m_range_y.min;
}


void ChartWindow::addLineBuffer(const charts::Chart &p_chart)
{
    makeCurrent();

    QList<GLfloat> line_vertices;

    const auto &table_columns = m_tables[p_chart.table_id].columns;
    const int points_count = table_columns[0].rows.size();

    const auto &column_x_rows = table_columns[p_chart.column_x].rows;
    const auto &column_y_rows = table_columns[p_chart.column_y].rows;

    line_vertices.reserve(points_count * 2);
    for (int i = 0; i < points_count; ++i) {
        if (m_log_scale_x) {
            line_vertices.append(static_cast<float>(log10(column_x_rows[i])));
        } else {
            line_vertices.append(static_cast<float>(column_x_rows[i]));
        }
        if (m_log_scale_y) {
            line_vertices.append(static_cast<float>(log10(column_y_rows[i])));
        } else {
            line_vertices.append(static_cast<float>(column_y_rows[i]));
        }
    }

    m_lines.append({ std::make_shared<QOpenGLVertexArrayObject>(),
                     std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer),
                     points_count });

    auto &lines = m_lines.last();

    lines.vao->create();

    if (lines.vao->isCreated())
        lines.vao->bind();

    lines.vbo_vertices->create();
    lines.vbo_vertices->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    lines.vbo_vertices->bind();
    lines.vbo_vertices->allocate(line_vertices.constData(),
                                 line_vertices.size() * static_cast<int>(sizeof(GLfloat)));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    lines.vao->release(); // error if VAO released after VBO
    lines.vbo_vertices->release();
}


void ChartWindow::clearLineBuffers()
{
    makeCurrent();
    m_lines.clear();
}


void ChartWindow::createPalette()
{
    QColor cl;

    cl.setHsl(0, 255, 127); //красный
    m_colors.append(cl);

    cl.setHsl(120, 255, 115); //зелёный
    m_colors.append(cl);

    cl.setHsl(240, 255, 127); //синий
    m_colors.append(cl);

    cl.setHsl(60, 255, 102); //жёлтый
    m_colors.append(cl);

    cl.setHsl(180, 255, 153); //морской волны
    m_colors.append(cl);

    cl.setHsl(300, 255, 127); //малиновый
    m_colors.append(cl);

    cl.setHsl(30, 255, 153); //оранжевый
    m_colors.append(cl);

    cl.setHsl(80, 230, 115); //салатовый
    m_colors.append(cl);

    cl.setHsl(210, 255, 127); //голубой
    m_colors.append(cl);

    cl.setHsl(270, 255, 127); //фиолетовый
    m_colors.append(cl);
}


void ChartWindow::createShaders()
{
    m_point_program.createProgramFromSourceFiles(":/Shaders/point.vert",
                                                 ":/Shaders/point.frag");

    m_line_program.createProgramFromSourceFiles(":/Shaders/line.vert",
                                                ":/Shaders/line.geom",
                                                ":/Shaders/line.frag");

    m_grid_program.createProgramFromSourceFiles(":/Shaders/grid.vert",
                                                ":/Shaders/grid.frag");

    m_grid_program_log_X.createProgramFromSourceFiles(":/Shaders/grid.vert",
                                                      ":/Shaders/grid_log_X.frag");

    m_grid_program_log_Y.createProgramFromSourceFiles(":/Shaders/grid.vert",
                                                      ":/Shaders/grid_log_Y.frag");

    m_grid_program_log_XY.createProgramFromSourceFiles(":/Shaders/grid.vert",
                                                       ":/Shaders/grid_log_XY.frag");
}


void ChartWindow::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

//    glEnable(GL_MULTISAMPLE);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SPRITE); // Обычно включена по умолчанию, но в линуксе не была

    m_lines.clear();
    for (const auto& chart : m_charts) {
        addLineBuffer(chart);
    }

//    QList<GLfloat> grid_vertices;
//    grid_vertices.append({-1,-1});
//    grid_vertices.append({1,-1});
//    grid_vertices.append({-1,1});
//    grid_vertices.append({1,1});

    m_grid.vao = std::make_shared<QOpenGLVertexArrayObject>();

    m_grid.vao->create();
    if (m_grid.vao->isCreated())
        m_grid.vao->bind();

    m_grid.vbo_vertices = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
    m_grid.vbo_vertices->create();
    m_grid.vbo_vertices->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_grid.vbo_vertices->bind();
    m_grid.vbo_vertices->allocate(8 * static_cast<int>(sizeof(GLfloat)));
//    m_grid.vbo_vertices->allocate(grid_vertices.constData(),
//                                  grid_vertices.size() * static_cast<int>(sizeof(GLfloat)));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    m_grid.vao->release(); // error if VAO released after VBO
    m_grid.vbo_vertices->release();


    m_zoom_frame.vao = std::make_shared<QOpenGLVertexArrayObject>();

    m_zoom_frame.vao->create();
    if (m_zoom_frame.vao->isCreated())
        m_zoom_frame.vao->bind();

    m_zoom_frame.vbo_vertices = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
    m_zoom_frame.vbo_vertices->create();
    m_zoom_frame.vbo_vertices->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_zoom_frame.vbo_vertices->bind();
    m_zoom_frame.vbo_vertices->allocate(8 * static_cast<int>(sizeof(GLfloat)));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    m_zoom_frame.vao->release(); // error if VAO released after VBO
    m_zoom_frame.vbo_vertices->release();

    createShaders();
    createPalette();

    m_is_initialized = true;
    emit contextInitialized();
}


void ChartWindow::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
    m_projection.setToIdentity();
    m_projection.ortho(0, width(), height(), 0, -1, 1);

    m_device_pixel_ratio = devicePixelRatioF();
}


void ChartWindow::paintGL()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT);

    m_mv_matrix.setToIdentity();
//    m_mv_matrix.translate(0.0, height(), 0.0);
    m_mv_matrix.translate(m_frame_margin_left,
                          height()-m_frame_margin_bottom,
                          0.0);
//    m_mv_matrix.scale(width()/static_cast<float>(m_range_x.max - m_range_x.min),
//                      height()/static_cast<float>(m_range_y.min - m_range_y.max),
//                      1);
    m_mv_matrix.scale((width()-m_frame_margin_left-m_frame_margin_right)
                      / static_cast<float>(m_range_x.max - m_range_x.min),
                      (height()-m_frame_margin_top-m_frame_margin_bottom)
                      / static_cast<float>(m_range_y.min - m_range_y.max),
                      1);

    m_mv_matrix.translate(static_cast<float>(-m_range_x.min),
                          static_cast<float>(-m_range_y.min),
                          0.0);

    auto mvp = m_projection * m_mv_matrix;

    // Сетка
    updateGridBufferData();
    Real pixel_size[]{ (m_range_x.max-m_range_x.min)/width(),
                         (m_range_y.max-m_range_y.min)/height() };
    Real grid_step[2];
//    Real average_grid_step_in_pixels[]{200, 200};
    Real average_grid_step_in_pixels[]{width()/ 6.0, height()/ 6.0};

    for (int i_c = 0; i_c < 2; ++i_c) {
        Real average_grid_step = pixel_size[i_c] * average_grid_step_in_pixels[i_c];
        Real step_order = floor( log10(average_grid_step) );
        Real step = pow(10, step_order);

        Real step_factor_candidates[]{2.0, 5.0, 10.0};

        grid_step[i_c] = step;
        for (int i_factor = 0; i_factor < 3; ++i_factor) {
            if (fabs(average_grid_step - step * step_factor_candidates[i_factor])
                    < fabs(average_grid_step - grid_step[i_c])) {
                grid_step[i_c] = step * step_factor_candidates[i_factor];
            }
        }
    }
//    if (m_log_scale_x)
//        grid_step[0] = 1;

    GridShader *grid_program;

    bool log_x = false;
    bool log_y = false;

    if (m_log_scale_x && grid_step[0] <= 1) {
        grid_step[0] = 1;
        log_x = true;
    }
    if (m_log_scale_y && grid_step[1] <= 1) {
        grid_step[1] = 1;
        log_y = true;
    }

    if (log_x && log_y) {
        grid_program = &m_grid_program_log_XY;
    } else if (log_x) {
        grid_program = &m_grid_program_log_X;
    } else if (log_y) {
        grid_program = &m_grid_program_log_Y;
    } else {
        grid_program = &m_grid_program;
    }

    grid_program->bind();

    Real grid_min[]{ m_range_x.min, m_range_y.min };
    Real grid_max[]{ m_range_x.max, m_range_y.max };
    Real disp[]{ 0.0, 0.0 };

    for (int i_c = 0; i_c < 2; ++i_c) {
        if (grid_min[i_c] > 0 && grid_max[i_c] > 0) {
            disp[i_c] = trunc(grid_min[i_c] / grid_step[i_c]) * grid_step[i_c];
            grid_min[i_c] -= disp[i_c];
            grid_max[i_c] -= disp[i_c];
        } else if (grid_min[i_c] < 0 && grid_max[i_c] < 0) {
            disp[i_c] = trunc(grid_min[i_c] / grid_step[i_c] - 1) * grid_step[i_c];
            grid_min[i_c] -= disp[i_c];
            grid_max[i_c] -= disp[i_c];
        }
    }

//    m_grid_program.setGridRect(static_cast<float>(grid_min[0] / grid_step[0]),
//                               static_cast<float>(grid_min[1] / grid_step[1]),
//                               static_cast<float>((grid_max[0]-grid_min[0]) / grid_step[0] / width()),
//                               static_cast<float>((grid_max[1]-grid_min[1]) / grid_step[1] / height()));
    grid_program->setGridRect(static_cast<float>(grid_min[0] / grid_step[0]),
                              static_cast<float>(grid_min[1] / grid_step[1]),
                              static_cast<float>((grid_max[0]-grid_min[0]) / grid_step[0]),
                              static_cast<float>((grid_max[1]-grid_min[1]) / grid_step[1]));
    grid_program->setViewRect(m_frame_margin_left,
                              m_frame_margin_bottom,
                              width()*m_device_pixel_ratio - m_frame_margin_right - m_frame_margin_left,
                              height()*m_device_pixel_ratio - m_frame_margin_top - m_frame_margin_bottom);

    m_grid.vao->bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    m_grid.vao->release();

    if (m_charts.isEmpty())
        return;

    // Невыделенные линии
    m_line_program.bind();
    m_line_program.setMVPMatrix(mvp);
    m_line_program.setPixels(width(), height());
    m_line_program.setViewRect(m_frame_margin_left,
                               m_frame_margin_bottom,
                               width()*m_device_pixel_ratio - m_frame_margin_right,
                               height()*m_device_pixel_ratio - m_frame_margin_top);
    m_line_program.setLineWidth(m_line_width);

    for (int i = 0; i < m_lines.size(); ++i) {
        if (i == m_chosen_chart) {
            continue;
        }
        m_line_program.setColor(getChartColor(i));
        m_lines[i].vao->bind();

        glDrawArrays(GL_LINE_STRIP, 0, m_lines[i].point_count);
        m_lines[i].vao->release();
    }

    if (m_is_show_points) {
        m_point_program.bind();
        m_point_program.setMVPMatrix(mvp);
        m_point_program.setPointSize(m_point_size);
        m_point_program.setViewRect(m_frame_margin_left,
                                    m_frame_margin_bottom,
                                    width()*m_device_pixel_ratio - m_frame_margin_right,
                                    height()*m_device_pixel_ratio - m_frame_margin_top);

        if (m_is_fixed) {
            m_point_program.setPointStyle(0);
        } else {
            m_point_program.setPointStyle(1);
        }
        for (int i = 0; i < m_lines.size(); ++i) {
            if (i == m_chosen_chart) {
                continue;
            }
            m_point_program.setColor(getChartColor(i));
            m_lines[i].vao->bind();
            glDrawArrays(GL_POINTS, 0, m_lines[i].point_count);
            m_lines[i].vao->release();
        }
    }

    // Выделенная линия
    if (m_chosen_chart != -1) {
        m_line_program.bind();
        m_line_program.setMVPMatrix(mvp);
        m_line_program.setPixels(width(), height());
        m_line_program.setViewRect(m_frame_margin_left,
                                   m_frame_margin_bottom,
                                   width()*m_device_pixel_ratio - m_frame_margin_right,
                                   height()*m_device_pixel_ratio - m_frame_margin_top);
        m_line_program.setLineWidth(m_line_width);

        m_line_program.setColor(getChartColor(m_chosen_chart));
        m_lines[m_chosen_chart].vao->bind();
        glDrawArrays(GL_LINE_STRIP, 0, m_lines[m_chosen_chart].point_count);
        m_lines[m_chosen_chart].vao->release();


        m_point_program.bind();
        m_point_program.setMVPMatrix(mvp);
        m_point_program.setPointSize(m_point_size);
        m_point_program.setViewRect(m_frame_margin_left,
                                    m_frame_margin_bottom,
                                    width()*m_device_pixel_ratio - m_frame_margin_right,
                                    height()*m_device_pixel_ratio - m_frame_margin_top);
        m_point_program.setColor(getChartColor(m_chosen_chart));

        m_lines[m_chosen_chart].vao->bind();
        if (m_is_show_points) {
            if (m_is_fixed) {
                m_point_program.setPointStyle(0);
            } else {
                m_point_program.setPointStyle(1);
            }
            glDrawArrays(GL_POINTS, 0, m_lines[m_chosen_chart].point_count);
        }

        if (m_chosen_point != -1) {
            m_point_program.setPointStyle(2);
            m_point_program.setPointSize(m_point_size + chosen_point_expansion);
            glDrawArrays(GL_POINTS, m_chosen_point, 1);
        }
        m_lines[m_chosen_chart].vao->release();
    }

    // Рамка масштабирования
    if (m_is_zoom_frame) {
        updateZoomFrameBufferData();

        m_line_program.bind();
        m_line_program.setMVPMatrix(mvp);
        m_line_program.setPixels(width(), height());
        m_line_program.setViewRect(0,
                                   0,
                                   width()*m_device_pixel_ratio,
                                   height()*m_device_pixel_ratio);
        m_line_program.setLineWidth(3);

        m_line_program.setColor(Qt::darkBlue);
        m_zoom_frame.vao->bind();
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        m_zoom_frame.vao->release();
    }

    QPainter painter(this);

    if (m_label_style == sides) {
        painter.drawRect(m_frame_margin_left,
                         m_frame_margin_top,
                         width() - m_frame_margin_right - m_frame_margin_left,
                         height() - m_frame_margin_bottom - m_frame_margin_top);
    }

    painter.setBrush(Qt::white);

    //painter.drawLine(projectX(9999.995),projectY(0.9999995), projectX(10000.005),projectY(1.0000005));

    // Названия используемых столбцов
    if (m_label_style != none) {
        QFont font("Arial", 16);//, QFont::Bold
        painter.setFont(font);
        if (m_label_style == top) {
            QString text = QString("%1 - %2")
                    .arg(m_tables[m_charts[0].table_id].columns[m_charts[0].column_y].id,
                         m_tables[m_charts[0].table_id].columns[m_charts[0].column_x].id);
            painter.drawText(0, 2, width(), height()-2,
                             Qt::AlignTop|Qt::AlignHCenter,
                             text);
        } else {
            painter.save();
            painter.translate(0, height());
            painter.rotate(-90);
            QString text =
                QString("%1").arg(m_tables[m_charts[0].table_id].columns[m_charts[0].column_y].id);
            painter.drawText(m_frame_margin_bottom,
                             0,
                             height()-m_frame_margin_bottom-m_frame_margin_top,
                             m_frame_margin_left,
                             Qt::AlignVCenter|Qt::AlignHCenter,
                             text);
            painter.restore();

            text =
                QString("%1").arg(m_tables[m_charts[0].table_id].columns[m_charts[0].column_x].id);
            painter.drawText(m_frame_margin_left,
                             height()-m_frame_margin_bottom,
                             width()-m_frame_margin_left-m_frame_margin_right,
                             m_frame_margin_bottom,
                             Qt::AlignVCenter|Qt::AlignHCenter,
                             text);
        }
    }

    // Значения осей
    {
        QFont font("Arial", 11, QFont::Bold);
        QFont font1("Arial", 9, QFont::Bold);

        painter.setFont(font);

        {
//            bool is_mark_under = m_frame_margin_bottom >= 20;
            bool is_mark_under = false;

            int text_win_y = static_cast<int>(projectY(m_range_y.min));
            int flags;
            if (is_mark_under) {
                text_win_y += 4;
                flags = Qt::AlignTop | Qt::AlignHCenter;
            } else {
                text_win_y -= 2;
                flags = Qt::AlignBottom | Qt::AlignHCenter;
            }

            for (int obj_x = ceil(m_range_x.min/grid_step[0]);
                 obj_x <= floor(m_range_x.max/grid_step[0]);
                 obj_x += 1)
            {
                int win_x = static_cast<int>(projectX(obj_x*grid_step[0]));

                if (win_x < m_frame_margin_left || win_x > width()-m_frame_margin_right) {
                    continue;
                }

                if (m_log_scale_x && m_log_tick_mark_style == LogTickMarkStyle::power_of_ten) {
                    Real tick_mark_value = obj_x*grid_step[0];
                    QString tick_mark_label = QString::number(tick_mark_value, 'g', 12);
                    QRect text_rect(win_x-1,
                                    text_win_y,
                                    2,
                                    1);

                    QRect bounding_rect_10 = painter.boundingRect(text_rect, flags, "10");
                    QRect bounding_rect = painter.boundingRect(QRect(bounding_rect_10.right()+1,
                                                                     bounding_rect_10.top(),
                                                                     1,
                                                                     bounding_rect_10.height()/2.0),
                                                               Qt::AlignBottom | Qt::AlignLeft, tick_mark_label);

                    if (!is_mark_under) {
                        painter.setPen(Qt::white);
                        painter.drawRect(bounding_rect_10);
                        painter.drawRect(bounding_rect);
                    }
                    painter.setPen(Qt::black);
                    painter.drawText(bounding_rect_10, flags, "10");
                    painter.setFont(font1);
                    painter.drawText(bounding_rect, Qt::AlignBottom | Qt::AlignLeft, tick_mark_label);
                    painter.setFont(font);
                } else {
                    Real tick_mark_value = obj_x*grid_step[0];
                    if (m_log_scale_x && m_log_tick_mark_style == LogTickMarkStyle::simple_value) {
                        tick_mark_value = pow(10, tick_mark_value);
                    }
                    QString tick_mark_label = QString::number(tick_mark_value, 'g', 12);
                    QRect text_rect(win_x-1,
                                    text_win_y,
                                    2,
                                    1);
                    QRect bounding_rect = painter.boundingRect(text_rect, flags, tick_mark_label);
                    if (!is_mark_under) {
                        painter.setPen(Qt::white);
                        painter.drawRect(bounding_rect);
                    }
                    painter.setPen(Qt::black);
                    painter.drawText(bounding_rect, flags, tick_mark_label);
                }
            }
        }

        {
            bool is_mark_left = false;
//            bool is_mark_left = m_frame_margin_bottom >= 20;

            int text_win_x = static_cast<int>(projectX(m_range_x.min));
            int flags;
            if (is_mark_left) {
                text_win_x -= 6;
                flags = Qt::AlignVCenter | Qt::AlignRight;
            } else {
                text_win_x += 3;
                flags = Qt::AlignVCenter | Qt::AlignLeft;
            }

            for (int obj_y = ceil(m_range_y.min/grid_step[1]);
                 obj_y <= floor(m_range_y.max/grid_step[1]);
                 obj_y += 1)
            {
                int win_y = static_cast<int>(projectY(obj_y*grid_step[1]));

                if (win_y < m_frame_margin_top || win_y > height()-m_frame_margin_bottom) {
                    continue;
                }

                if (m_log_scale_y && m_log_tick_mark_style == LogTickMarkStyle::power_of_ten) {
                    Real tick_mark_value = obj_y*grid_step[1];
                    QString tick_mark_label = QString::number(tick_mark_value, 'g', 12);
                    QRect text_rect(text_win_x,
                                    win_y-1,
                                    1,
                                    2);

                    QRect bounding_rect_10 = painter.boundingRect(text_rect, flags, "10");
                    QRect bounding_rect = painter.boundingRect(QRect(bounding_rect_10.right()+1,
                                                                     bounding_rect_10.top(),
                                                                     1,
                                                                     bounding_rect_10.height()/2.0),
                                                               Qt::AlignBottom | Qt::AlignLeft, tick_mark_label);

                    if (!is_mark_left) {
                        painter.setPen(Qt::white);
                        painter.drawRect(bounding_rect_10);
                        painter.drawRect(bounding_rect);
                    }
                    painter.setPen(Qt::black);
                    painter.drawText(bounding_rect_10, flags, "10");
                    painter.setFont(font1);
                    painter.drawText(bounding_rect, Qt::AlignBottom | Qt::AlignLeft, tick_mark_label);
                    painter.setFont(font);
                } else {
                    Real tick_mark_value = obj_y*grid_step[1];
                    if (m_log_scale_y && m_log_tick_mark_style == LogTickMarkStyle::simple_value) {
                        tick_mark_value = pow(10, tick_mark_value);
                    }
                    QString tick_mark_label = QString::number(tick_mark_value, 'g', 12);
                    QRect text_rect(text_win_x,
                                    win_y-1,
                                    1,
                                    2);
                    QRect bounding_rect = painter.boundingRect(text_rect, flags, tick_mark_label);
                    if (!is_mark_left) {
                        painter.setPen(Qt::white);
                        painter.drawRect(bounding_rect);
                    }
                    painter.setPen(Qt::black);
                    painter.drawText(bounding_rect, flags, tick_mark_label);
                }
            }
        }
    }

    // Значения выделенной точки
    if (m_chosen_point != -1) {
        QFont font("Arial", 11);//, QFont::Bold
        painter.setFont(font);
        QFontMetrics fm(font);
//        QString text = QString("%1 = %2; %3 = %4").arg(m_ids[m_coord1]).arg(m_data[m_chosen_chart][m_chosen_point][m_coord1]).arg(m_ids[m_coord2]).arg(m_data[m_chosen_chart][m_chosen_point][m_coord2]);
//        QString text = QString("%1 %2").arg(m_data[m_chosen_chart][m_chosen_point][m_coord1]).arg(m_data[m_chosen_chart][m_chosen_point][m_coord2]);
//        QString text1 = QString("%1").arg(m_data[m_chosen_chart][m_chosen_point][m_coord1]);
//        QString text2 = QString("%2").arg(m_data[m_chosen_chart][m_chosen_point][m_coord2]);

        const auto &point = getChartPoint(m_chosen_chart, m_chosen_point);
        const auto &ids = getChartColumnPairIDs(m_chosen_chart);
        QString text1 = QString("%1 = %2").arg(ids.first).arg(point.first, 0, 'g', 12);
        QString text2 = QString("%1 = %2").arg(ids.second).arg(point.second, 0, 'g', 12);

        QRect text_rect1 = fm.boundingRect(text1);
        QRect text_rect2 = fm.boundingRect(text2);

        QRect text_rect;

        QString text0 = m_tables[m_charts[m_chosen_chart].table_id].id;
        if (!text0.isEmpty() && m_is_show_chart_id) {
            QRect text_rect0 = fm.boundingRect(text0);
            text_rect.setWidth(std::max(text_rect0.width(), std::max(text_rect1.width(), text_rect2.width())) + 8);
            text_rect.setHeight(text_rect0.height() + 6 + text_rect1.height() + 6 + text_rect2.height());
        } else {
            text_rect.setWidth(std::max(text_rect1.width(), text_rect2.width()) + 8);
            text_rect.setHeight(text_rect1.height() + 6 + text_rect2.height());
        }

//        int win_x_chosen = static_cast<int>(projectX(point.first));
//        int win_y_chosen = static_cast<int>(projectY(point.second));
        int win_x_chosen = static_cast<int>(m_log_scale_x
                                            ? projectX(log10(point.first))
                                            : projectX(point.first));
        int win_y_chosen = static_cast<int>(m_log_scale_y
                                            ? projectY(log10(point.second))
                                            : projectY(point.second));

        text_rect.moveTo(win_x_chosen - text_rect.width() / 2,
                         win_y_chosen - text_rect.height()
                          - static_cast<int>(m_point_size + chosen_point_expansion));

        if (text_rect.left() < 2) {
            text_rect.moveLeft(2);
        } else if (text_rect.right() > width()-2) {
            text_rect.moveRight(width()-2);
        }
        if (text_rect.top() < 2) {
            text_rect.moveTop(2);
        } else if (text_rect.bottom() > height()-2) {
            text_rect.moveBottom(height()-2);
        }
        QPen pen(getChartColor(m_chosen_chart).darker());
        pen.setWidth(2);
        painter.setPen(pen);

        painter.drawRect(text_rect);

        if (!text0.isEmpty() && m_is_show_chart_id) {
            painter.drawText(text_rect,
                             Qt::AlignCenter,
                             text0 + "\n" + text1 + "\n" + text2);
        } else {
            painter.drawText(text_rect,
                             Qt::AlignCenter,
                             text1 + "\n" + text2);
        }
    }
    painter.end();
}


void ChartWindow::mousePressEvent(QMouseEvent *p_event)
{
//    if (m_charts.isEmpty()) {
//        return;
//    }
    m_last_mouse_pos = p_event->pos();
    m_mouse_press_pos = p_event->pos();
    m_is_mouse_pressed = true;

    if (p_event->buttons() == Qt::RightButton) {
        m_is_zoom_frame = true;
        QPointF position = p_event->position();
        m_zoom_frame_rect.setLeft(unProjectX(position.x()));
        m_zoom_frame_rect.setBottom(unProjectY(position.y()));
        m_zoom_frame_rect.setRight(unProjectX(position.x()));
        m_zoom_frame_rect.setTop(unProjectY(position.y()));
    } else if (p_event->buttons() == Qt::MiddleButton) {
        viewAll();
    }
//    else {
//        int new_chosen_point = -1;
//        int new_chosen_chart = -1;

//        Real min_diff = 100;

//        for (int di = 0; di < m_data.size(); ++di) {
//            for (int i = 0; i < m_data[di].columns.size(); ++i) {
//                QPointF columns = {m_data[di].columns[i][m_coord1], m_data[di].columns[i][m_coord2]};

//                columns.setX(projectX(columns.x()));
//                columns.setY(projectY(columns.y()));

//                QPointF vec = event->localPos()-columns;
//                Real diff = QPointF::dotProduct(vec, vec);
//                if (diff < 34 && diff < min_diff) {
//                    new_chosen_point = i;
//                    new_chosen_chart = di;
//                    min_diff = diff;
//                }
//            }
//        }

//        if (m_chosen_point != new_chosen_point) {
//            m_chosen_point = new_chosen_point;
//            m_chosen_chart = new_chosen_chart;
//        }
//        update();
//    }
}


void ChartWindow::mouseReleaseEvent(QMouseEvent *p_event)
{
    m_is_mouse_pressed = false;

//    if (m_charts.isEmpty()) {
//        return;
//    }
    if (m_is_zoom_frame) {
        m_is_zoom_frame = false;
        if (QVector2D diff(m_last_mouse_pos-m_mouse_press_pos); diff.length() < context_menu_call_diff) {
            showContextMenu(p_event);
        } else if (!qFuzzyCompare(m_zoom_frame_rect.left(), m_zoom_frame_rect.right()) &&
                !qFuzzyCompare(m_zoom_frame_rect.bottom(), m_zoom_frame_rect.top())) {
            if (m_zoom_frame_rect.left() < m_zoom_frame_rect.right()) {
                m_range_x.min = m_zoom_frame_rect.left();
                m_range_x.max = m_zoom_frame_rect.right();
            } else {
                m_range_x.min = m_zoom_frame_rect.right();
                m_range_x.max = m_zoom_frame_rect.left();
            }
            if (m_zoom_frame_rect.bottom() < m_zoom_frame_rect.top()) {
                m_range_y.min = m_zoom_frame_rect.bottom();
                m_range_y.max = m_zoom_frame_rect.top();
            } else {
                m_range_y.min = m_zoom_frame_rect.top();
                m_range_y.max = m_zoom_frame_rect.bottom();
            }
        }
        update();
    }
}


void ChartWindow::mouseMoveEvent(QMouseEvent *p_event)
{
//    if (m_charts.isEmpty()) {
//        return;
//    }

    QPointF position = p_event->position();
    if (p_event->buttons() == Qt::NoButton) {
        int new_chosen_point = -1;
        int new_chosen_chart = -1;

        Real min_diff = static_cast<Real>(std::max(minimum_choose_range,
                                                   m_point_size / 2.0f + chosen_point_expansion));

        QRect chart_window_rect(m_frame_margin_left,
                                m_frame_margin_top,
                                width() - m_frame_margin_right - m_frame_margin_left,
                                height() - m_frame_margin_bottom - m_frame_margin_top);

        Real pos_x_min = m_log_scale_x
                ? pow(10, unProjectX(position.x() - min_diff))
                : unProjectX(position.x() - min_diff);
        Real pos_y_min = m_log_scale_y
                ? pow(10, unProjectY(position.y() + min_diff))
                : unProjectY(position.y() + min_diff);
        Real pos_x_max = m_log_scale_x
                ? pow(10, unProjectX(position.x() + min_diff))
                : unProjectX(position.x() + min_diff);
        Real pos_y_max = m_log_scale_y
                ? pow(10, unProjectY(position.y() - min_diff))
                : unProjectY(position.y() - min_diff);

        if (m_is_use_cells) {
            for (int di = 0; di < m_charts.size(); ++di) {
                const auto &chart = m_charts[di];
                const auto &local_range_x = m_ranges[chart.table_id][chart.column_x];
                const auto &local_range_y = m_ranges[chart.table_id][chart.column_y];

                // Определяем ячейку
                int cell_index_x_1 = local_range_x.getCellIndex(pos_x_min, m_cells_count);
                if (cell_index_x_1 > m_cells_count - 1) {
                    continue;
                }
                int cell_index_x_2 = local_range_x.getCellIndex(pos_x_max, m_cells_count);
                if (cell_index_x_2 < 0) {
                    continue;
                }
                int cell_index_y_1 = local_range_y.getCellIndex(pos_y_min, m_cells_count);
                if (cell_index_y_1 > m_cells_count - 1) {
                    continue;
                }
                int cell_index_y_2 = local_range_y.getCellIndex(pos_y_max, m_cells_count);
                if (cell_index_y_2 < 0) {
                    continue;
                }

                if (cell_index_x_1 < 0) {
                    cell_index_x_1 = 0;
                }
                if (cell_index_x_2 > m_cells_count - 1) {
                    cell_index_x_2 = m_cells_count - 1;
                }
                if (cell_index_y_1 < 0) {
                    cell_index_y_1 = 0;
                }
                if (cell_index_y_2 > m_cells_count - 1) {
                    cell_index_y_2 = m_cells_count - 1;
                }

                int cell_points_count_x = 0;
                int cell_points_count_y = 0;

                for (int cell_index_x = cell_index_x_1; cell_index_x <= cell_index_x_2; ++cell_index_x) {
                    cell_points_count_x += local_range_x.cells[cell_index_x].size();
                }
                for (int cell_index_y = cell_index_y_1; cell_index_y <= cell_index_y_2; ++cell_index_y) {
                    cell_points_count_y += local_range_y.cells[cell_index_y].size();
                }

                const ValueRange* local_range_ptr;
                int cell_index_1, cell_index_2;
                if (cell_points_count_x <= cell_points_count_y) {
                    local_range_ptr = &local_range_x;
                    cell_index_1 = cell_index_x_1;
                    cell_index_2 = cell_index_x_2;
                } else {
                    local_range_ptr = &local_range_y;
                    cell_index_1 = cell_index_y_1;
                    cell_index_2 = cell_index_y_2;
                }

                const auto &chart_rows_x = m_tables[chart.table_id].columns[chart.column_x].rows;
                const auto &chart_rows_y = m_tables[chart.table_id].columns[chart.column_y].rows;

                for (int cell_index = cell_index_1; cell_index <= cell_index_2; ++cell_index) {
                    for (const auto &value_index : local_range_ptr->cells[cell_index]) {
                        if (chart_rows_x[value_index] > pos_x_min &&
                            chart_rows_x[value_index] < pos_x_max) {
                            if (chart_rows_y[value_index] > pos_y_min &&
                                chart_rows_y[value_index] < pos_y_max) {

                                QPointF columns = { projectX(m_log_scale_x
                                                    ? log10(chart_rows_x[value_index])
                                                    : chart_rows_x[value_index]),
                                                    projectY(m_log_scale_y
                                                    ? log10(chart_rows_y[value_index])
                                                    : chart_rows_y[value_index]) };

                                if (!chart_window_rect.contains(static_cast<int>(columns.x()),
                                                                static_cast<int>(columns.y()))) {
                                    continue;
                                }

                                QPointF vec = position - columns;
                                Real diff = sqrt(QPointF::dotProduct(vec, vec));
                                if (diff < min_diff) {
                                    new_chosen_point = value_index;
                                    new_chosen_chart = di;
                                    min_diff = diff;
                                }
                            }
                        }
                    }
                }
            }
        } else {
            for (int di = 0; di < m_charts.size(); ++di) {
                const auto &chart = m_charts[di];
                const auto &chart_rows_x = m_tables[chart.table_id].columns[chart.column_x].rows;
                const auto &chart_rows_y = m_tables[chart.table_id].columns[chart.column_y].rows;
                for (int i = 0; i < chart_rows_x.size(); ++i) {
                    if (chart_rows_x[i] > pos_x_min && chart_rows_x[i] < pos_x_max) {
                        if (chart_rows_y[i] > pos_y_min && chart_rows_y[i] < pos_y_max) {
                            QPointF columns = { projectX(m_log_scale_x
                                                ? log10(chart_rows_x[i])
                                                : chart_rows_x[i]),
                                                projectY(m_log_scale_y
                                                ? log10(chart_rows_y[i])
                                                : chart_rows_y[i]) };

                            if (!chart_window_rect.contains(static_cast<int>(columns.x()),
                                                            static_cast<int>(columns.y()))) {
                                continue;
                            }

                            QPointF vec = p_event->position() - columns;
                            Real diff = sqrt(QPointF::dotProduct(vec, vec));
                            if (diff < min_diff) {
                                new_chosen_point = i;
                                new_chosen_chart = di;
                                min_diff = diff;
                            }
                        }
                    }
                }
            }
        }

        if (m_chosen_point != new_chosen_point) {
            m_chosen_point = new_chosen_point;
            m_chosen_chart = new_chosen_chart;
        }
    }

    if (p_event->buttons() == Qt::RightButton && m_is_zoom_frame) {
        m_zoom_frame_rect.setRight(unProjectX(position.x()));
        m_zoom_frame_rect.setTop(unProjectY(position.y()));

    } else if (m_is_mouse_pressed) {
        if ((m_chosen_point != -1) && !m_is_fixed) {
            Real dx = m_log_scale_x
                    ? pow(10, unProjectX(position.x())) - pow(10, unProjectX(m_last_mouse_pos.x()))
                    : unProjectX(position.x()) - unProjectX(m_last_mouse_pos.x());
            Real dy = m_log_scale_y
                    ? pow(10, unProjectY(position.y())) - pow(10, unProjectY(m_last_mouse_pos.y()))
                    : unProjectY(position.y()) - unProjectY(m_last_mouse_pos.y());

            auto &point_x = m_tables[m_charts[m_chosen_chart].table_id]
                                .columns[m_charts[m_chosen_chart].column_x].rows[m_chosen_point];
            auto &point_y = m_tables[m_charts[m_chosen_chart].table_id]
                                .columns[m_charts[m_chosen_chart].column_y].rows[m_chosen_point];

            if (p_event->modifiers() & Qt::ShiftModifier) {
                dy = 0;
            } else if (p_event->modifiers() & Qt::ControlModifier) {
                dx = 0;
            }

            if (point_x + dx < m_minimum_x) {
                dx = m_minimum_x - point_x;
            }
            if (point_x + dx > m_maximum_x) {
                dx = m_maximum_x - point_x;
            }
            if (point_y + dy < m_minimum_y) {
                dy = m_minimum_y - point_y;
            }
            if (point_y + dy > m_maximum_y) {
                dy = m_maximum_y - point_y;
            }

            point_x += dx;
            point_y += dy;

            updateRange(m_charts[m_chosen_chart].table_id, m_charts[m_chosen_chart].column_x);
            updateRange(m_charts[m_chosen_chart].table_id, m_charts[m_chosen_chart].column_y);

            updateChosenPointBufferData();

            emit dataChanged(m_charts[m_chosen_chart].table_id,
                             m_charts[m_chosen_chart].column_x,
                             m_charts[m_chosen_chart].column_y);
        } else {
            Real dx = unProjectX(position.x()) - unProjectX(m_last_mouse_pos.x());
            Real dy = unProjectY(position.y()) - unProjectY(m_last_mouse_pos.y());
            if (p_event->modifiers() & Qt::ShiftModifier) {
                m_range_x.min -= dx;
                m_range_x.max -= dx;
            } else if (p_event->modifiers() & Qt::ControlModifier) {
                m_range_y.min -= dy;
                m_range_y.max -= dy;
            } else {
                m_range_x.min -= dx;
                m_range_x.max -= dx;
                m_range_y.min -= dy;
                m_range_y.max -= dy;
            }
        }
    }

    m_last_mouse_pos = p_event->pos();

    update();
}


void ChartWindow::mouseDoubleClickEvent(QMouseEvent *p_event)
{
    Q_UNUSED(p_event)

    if (m_charts.isEmpty()) {
        return;
    }
    if (m_is_fixed) {
        return;
    }

    if (p_event->buttons() == Qt::LeftButton && m_chosen_point != -1) {
        m_is_mouse_pressed = false;
        showChangePointDialog();
    }
}


void ChartWindow::wheelEvent(QWheelEvent *p_event)
{
//    if (m_charts.isEmpty()) {
//        return;
//    }
    Real ds;
    if (p_event->angleDelta().y() < 0) {
        ds = 1.1;
    } else {
        ds = 1.0/1.1;
    }

    Real x = unProjectX(p_event->position().x());
    Real y = unProjectY(p_event->position().y());

    Real new_range_x_min = x + (m_range_x.min - x) * ds;
    Real new_range_x_max = x + (m_range_x.max - x) * ds;
    Real new_range_y_min = y + (m_range_y.min - y) * ds;
    Real new_range_y_max = y + (m_range_y.max - y) * ds;

    if (p_event->modifiers() & Qt::ShiftModifier) {
        if (!qFuzzyCompare(float(new_range_x_min), float(new_range_x_max))) {
            m_range_x.min = new_range_x_min;
            m_range_x.max = new_range_x_max;
        }
    } else if (p_event->modifiers() & Qt::ControlModifier) {
        if (!qFuzzyCompare(float(new_range_y_min), float(new_range_y_max))) {
            m_range_y.min = new_range_y_min;
            m_range_y.max = new_range_y_max;
        }
    } else {
        if (!qFuzzyCompare(float(new_range_x_min), float(new_range_x_max)) &&
            !qFuzzyCompare(float(new_range_y_min), float(new_range_y_max))) {
            m_range_x.min = new_range_x_min;
            m_range_x.max = new_range_x_max;
            m_range_y.min = new_range_y_min;
            m_range_y.max = new_range_y_max;
        }
    }
    update();
}


void ChartWindow::keyPressEvent(QKeyEvent *p_event)
{
    if (p_event->key() == Qt::Key_Escape && m_is_zoom_frame) {
        m_is_zoom_frame = false;
        update();
    }
}


void ChartWindow::showContextMenu(QMouseEvent *p_event)
{
    QMenu context_menu(tr("Context menu"));

    QList<QAction*> m_context_menu_point_actions;
    QList<QAction*> m_context_menu_chart_actions;

    if ((m_chosen_point != -1) && !m_is_fixed) {
        QAction *insert_action = new QAction(tr("Добавить точку после"), this);
        m_context_menu_point_actions.append(insert_action);
        connect(insert_action, &QAction::triggered, this, [this](){
            addRow(m_charts[m_chosen_chart].table_id, m_chosen_point+1);
            m_chosen_chart = -1;
            m_chosen_point = -1;
        });

        QAction *remove_action = new QAction(tr("Удалить точку"), this);
        m_context_menu_point_actions.append(remove_action);
        connect(remove_action, &QAction::triggered, this, [this](){
            removeRow(m_charts[m_chosen_chart].table_id, m_chosen_point);
            m_chosen_chart = -1;
            m_chosen_point = -1;
        });

        QAction *change_action = new QAction(tr("Редактировать точку"), this);
        m_context_menu_point_actions.append(change_action);
        connect(change_action, &QAction::triggered, this, [this](){
            showChangePointDialog();
        });
    }

    QAction *view_all_action = new QAction(tr("Подогнать масштаб под окно"), this);
    m_context_menu_chart_actions.append(view_all_action);
    connect(view_all_action, &QAction::triggered, this, &ChartWindow::viewAll);

    QAction *set_log_scale_x_action = new QAction(tr("Логарифмический масштаб по X"), this);
    set_log_scale_x_action->setCheckable(true);
    set_log_scale_x_action->setChecked(m_log_scale_x);
    m_context_menu_chart_actions.append(set_log_scale_x_action);
    connect(set_log_scale_x_action, &QAction::toggled, this, &ChartWindow::setLogScaleX);

    QAction *set_log_scale_y_action = new QAction(tr("Логарифмический масштаб по Y"), this);
    set_log_scale_y_action->setCheckable(true);
    set_log_scale_y_action->setChecked(m_log_scale_y);
    m_context_menu_chart_actions.append(set_log_scale_y_action);
    connect(set_log_scale_y_action, &QAction::toggled, this, &ChartWindow::setLogScaleY);

    if (!m_context_menu_point_actions.isEmpty()) {
        context_menu.addActions( m_context_menu_point_actions );
        context_menu.addSeparator();
    }
    context_menu.addActions( m_context_menu_chart_actions );

    if (!context_menu.actions().isEmpty()) {
        context_menu.exec(p_event->globalPosition().toPoint());
    }
}


void ChartWindow::showChangePointDialog()
{
    auto &chosen_x =
        m_tables[m_charts[m_chosen_chart].table_id].columns[m_charts[m_chosen_chart].column_x];
    auto &chosen_y =
        m_tables[m_charts[m_chosen_chart].table_id].columns[m_charts[m_chosen_chart].column_y];

    auto dialog = ChangePointDialog(tr("Редактирование точки"),
                                    chosen_x.rows[m_chosen_point],
                                    chosen_y.rows[m_chosen_point],
                                    chosen_x.id,
                                    chosen_y.id);

    dialog.setRangeX(m_minimum_x, m_maximum_x);
    dialog.setRangeY(m_minimum_y, m_maximum_y);

    if (dialog.exec() == QDialog::Accepted) {
        chosen_x.rows[m_chosen_point] = dialog.pointX();
        chosen_y.rows[m_chosen_point] = dialog.pointY();

        // Валидатор в диалоге работает только с нулём, поэтому дополнительная проверка попадания в пределы
        if (chosen_x.rows[m_chosen_point] < m_minimum_x) {
            chosen_x.rows[m_chosen_point] = m_minimum_x;
        }
        if (chosen_x.rows[m_chosen_point] > m_maximum_x) {
            chosen_x.rows[m_chosen_point] = m_maximum_x;
        }
        if (chosen_y.rows[m_chosen_point] < m_minimum_y) {
            chosen_y.rows[m_chosen_point] = m_minimum_y;
        }
        if (chosen_y.rows[m_chosen_point] > m_maximum_y) {
            chosen_y.rows[m_chosen_point] = m_maximum_y;
        }

        updateRange(m_charts[m_chosen_chart].table_id, m_charts[m_chosen_chart].column_x);
        updateRange(m_charts[m_chosen_chart].table_id, m_charts[m_chosen_chart].column_y);

        updateChosenPointBufferData();

        emit dataChanged(m_charts[m_chosen_chart].table_id,
                         m_charts[m_chosen_chart].column_x,
                         m_charts[m_chosen_chart].column_y);
        m_chosen_chart = -1;
        m_chosen_point = -1;
        update();
    }
}


void ChartWindow::updateChosenPointBufferData()
{
    makeCurrent();
    auto &vbo_vertices = m_lines[m_chosen_chart].vbo_vertices;
    vbo_vertices->bind();

    if (auto *vertices = static_cast<GLfloat*>(vbo_vertices->map(QOpenGLBuffer::WriteOnly))) {
        const auto &chart_data = m_tables[m_charts[m_chosen_chart].table_id].columns;
        const auto &column_x_rows = chart_data[m_charts[m_chosen_chart].column_x].rows;
        const auto &column_y_rows = chart_data[m_charts[m_chosen_chart].column_y].rows;

        if (m_log_scale_x) {
            vertices[m_chosen_point * 2 + 0] = static_cast<GLfloat>(log10(column_x_rows[m_chosen_point]));
        } else {
            vertices[m_chosen_point * 2 + 0] = static_cast<GLfloat>(column_x_rows[m_chosen_point]);
        }

        if (m_log_scale_y) {
            vertices[m_chosen_point * 2 + 1] = static_cast<GLfloat>(log10(column_y_rows[m_chosen_point]));
        } else {
            vertices[m_chosen_point * 2 + 1] = static_cast<GLfloat>(column_y_rows[m_chosen_point]);
        }

        vbo_vertices->unmap();
    }

    vbo_vertices->release();
}


void ChartWindow::updateBufferData()
{
    makeCurrent();

    int chart_index{ 0 };

    for (const auto &chart : m_charts) {
        auto &vbo_vertices = m_lines[chart_index++].vbo_vertices;
        vbo_vertices->bind();

        if (auto *vertices = static_cast<GLfloat*>(vbo_vertices->map(QOpenGLBuffer::WriteOnly))) {
            int points_count = m_tables[chart.table_id].columns[0].rows.size();
            const auto &column_x_rows = m_tables[chart.table_id].columns[chart.column_x].rows;
            const auto &column_y_rows = m_tables[chart.table_id].columns[chart.column_y].rows;

            for (int i_point = 0; i_point < points_count; ++i_point) {
                if (m_log_scale_x) {
                    vertices[i_point * 2 + 0] = static_cast<GLfloat>(log10(column_x_rows[i_point]));
                } else {
                    vertices[i_point * 2 + 0] = static_cast<GLfloat>(column_x_rows[i_point]);
                }

                if (m_log_scale_y) {
                    vertices[i_point * 2 + 1] = static_cast<GLfloat>(log10(column_y_rows[i_point]));
                } else {
                    vertices[i_point * 2 + 1] = static_cast<GLfloat>(column_y_rows[i_point]);
                }
            }

            vbo_vertices->unmap();
        }

        vbo_vertices->release();
    }
}


void ChartWindow::updateBufferData(int p_chart_index)
{
    makeCurrent();

    const auto &chart = m_charts[p_chart_index];
    auto &vbo_vertices = m_lines[p_chart_index].vbo_vertices;
    vbo_vertices->bind();

    if (auto *vertices = static_cast<GLfloat*>(vbo_vertices->map(QOpenGLBuffer::WriteOnly))) {
        int points_count = m_tables[chart.table_id].columns[0].rows.size();
        const auto &column_x_rows = m_tables[chart.table_id].columns[chart.column_x].rows;
        const auto &column_y_rows = m_tables[chart.table_id].columns[chart.column_y].rows;

        for (int i_point = 0; i_point < points_count; ++i_point) {
            if (m_log_scale_x) {
                vertices[i_point * 2 + 0] = static_cast<GLfloat>(log10(column_x_rows[i_point]));
            } else {
                vertices[i_point * 2 + 0] = static_cast<GLfloat>(column_x_rows[i_point]);
            }

            if (m_log_scale_y) {
                vertices[i_point * 2 + 1] = static_cast<GLfloat>(log10(column_y_rows[i_point]));
            } else {
                vertices[i_point * 2 + 1] = static_cast<GLfloat>(column_y_rows[i_point]);
            }
        }

        vbo_vertices->unmap();
    }

    vbo_vertices->release();
}


void ChartWindow::resizeAndUpdateBufferData(int p_chart_index)
{
    makeCurrent();

    const auto &chart = m_charts[p_chart_index];

    QList<GLfloat> line_vertices;

    const auto &table_columns = m_tables[chart.table_id].columns;
    const int points_count = table_columns[0].rows.size();

    const auto &column_x_rows = table_columns[chart.column_x].rows;
    const auto &column_y_rows = table_columns[chart.column_y].rows;

    line_vertices.reserve(points_count * 2);
    for (int i = 0; i < points_count; ++i) {
        if (m_log_scale_x) {
            line_vertices.append(static_cast<GLfloat>(log10(column_x_rows[i])));
        } else {
            line_vertices.append(static_cast<GLfloat>(column_x_rows[i]));
        }

        if (m_log_scale_y) {
            line_vertices.append(static_cast<GLfloat>(log10(column_y_rows[i])));
        } else {
            line_vertices.append(static_cast<GLfloat>(column_y_rows[i]));
        }
    }

    m_lines[p_chart_index].point_count = points_count;

    auto &lines = m_lines[p_chart_index];

    lines.vbo_vertices->bind();
    lines.vbo_vertices->allocate(line_vertices.constData(),
                                 line_vertices.size() * static_cast<int>(sizeof(GLfloat)));
    lines.vbo_vertices->release();
}


void ChartWindow::updateGridBufferData()
{
    makeCurrent();
    m_grid.vbo_vertices->bind();
    auto vertices = static_cast<GLfloat*>(m_grid.vbo_vertices->map(QOpenGLBuffer::WriteOnly));

    if (vertices) {
        GLfloat left = static_cast<GLfloat>(m_frame_margin_left) / width() * 2.0f - 1.0f;
        GLfloat right = 1.0f - static_cast<GLfloat>(m_frame_margin_right) / width() * 2.0f;
        GLfloat bottom = static_cast<GLfloat>(m_frame_margin_bottom) / height() * 2.0f - 1.0f;
        GLfloat top = 1.0f - static_cast<GLfloat>(m_frame_margin_top) / height() * 2.0f;

        vertices[0] = left;
        vertices[1] = bottom;

        vertices[2] = right;
        vertices[3] = bottom;

        vertices[4] = left;
        vertices[5] = top;

        vertices[6] = right;
        vertices[7] = top;

        m_grid.vbo_vertices->unmap();
    }

    m_grid.vbo_vertices->release();
}


void ChartWindow::updateZoomFrameBufferData()
{
    makeCurrent();
    m_zoom_frame.vbo_vertices->bind();
    auto vertices = static_cast<GLfloat*>(m_zoom_frame.vbo_vertices->map(QOpenGLBuffer::WriteOnly));

    if (vertices) {
        vertices[0] = static_cast<GLfloat>(m_zoom_frame_rect.left());
        vertices[1] = static_cast<GLfloat>(m_zoom_frame_rect.bottom());

        vertices[2] = static_cast<GLfloat>(m_zoom_frame_rect.right());
        vertices[3] = static_cast<GLfloat>(m_zoom_frame_rect.bottom());

        vertices[4] = static_cast<GLfloat>(m_zoom_frame_rect.right());
        vertices[5] = static_cast<GLfloat>(m_zoom_frame_rect.top());

        vertices[6] = static_cast<GLfloat>(m_zoom_frame_rect.left());
        vertices[7] = static_cast<GLfloat>(m_zoom_frame_rect.top());

        m_zoom_frame.vbo_vertices->unmap();
    }

    m_zoom_frame.vbo_vertices->release();
}


void ChartWindow::viewAll()
{
    findMinMax();
    m_chosen_chart = -1;
    m_chosen_point = -1;
    update();
}


void ChartWindow::setLogScaleX(bool p_log_scale_x)
{
    m_log_scale_x = p_log_scale_x;
    findMinMax();
    updateBufferData();
}


void ChartWindow::setLogScaleY(bool p_log_scale_y)
{
    m_log_scale_y = p_log_scale_y;
    findMinMax();
    updateBufferData();
}
