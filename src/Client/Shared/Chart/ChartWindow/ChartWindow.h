#ifndef CHART_WINDOW_H
#define CHART_WINDOW_H

#include <ChartData>

#include <QOpenGLWindow>

#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include <memory>

class ChartWindow final : public QOpenGLWindow, private QOpenGLFunctions_3_2_Core
{
    Q_OBJECT

    /// Стиль подписей осей
public:
    enum LabelStyle
    {
        none,
        top,
        sides
    };

    enum LogTickMarkStyle
    {
        power_of_ten,
        logarithm,
        simple_value
    };

private:
    struct LineData{
        std::shared_ptr<QOpenGLVertexArrayObject> vao;
        std::shared_ptr<QOpenGLBuffer> vbo_vertices;
        GLsizei point_count{0};
//        void updateBufferData();
    };

    QList<LineData> m_lines;

    struct {
        std::shared_ptr<QOpenGLVertexArrayObject> vao;
        std::shared_ptr<QOpenGLBuffer> vbo_vertices;
//        void updateBufferData();
    } m_grid;

    struct {
        std::shared_ptr<QOpenGLVertexArrayObject> vao;
        std::shared_ptr<QOpenGLBuffer> vbo_vertices;
//        void updateBufferData();
    } m_zoom_frame;

    class Shader : public QOpenGLShaderProgram
    {
    protected:
        virtual void setUniformLocations() {}
        virtual void bindAttributeLocations() {}

    public:
        void createProgramFromSourceFiles(const QString &p_vertex_filename,
                                          const QString &p_geometry_filename,
                                          const QString &p_fragment_filename) {
            addShaderFromSourceFile(QOpenGLShader::Vertex, p_vertex_filename);
            addShaderFromSourceFile(QOpenGLShader::Geometry, p_geometry_filename);
            addShaderFromSourceFile(QOpenGLShader::Fragment, p_fragment_filename);
            bindAttributeLocations();
            link();
            bind();

            setUniformLocations();
        }
        void createProgramFromSourceFiles(const QString &p_vertex_filename,
                                          const QString &p_fragment_filename) {
            addShaderFromSourceFile(QOpenGLShader::Vertex, p_vertex_filename);
            addShaderFromSourceFile(QOpenGLShader::Fragment, p_fragment_filename);
            bindAttributeLocations();
            link();
            bind();

            setUniformLocations();
        }
    };

    class PointShader : public Shader
    {
        int m_mvp_matrix_loc{};
        int m_color_loc{};
        int m_point_size_loc{};
        int m_point_style_loc{};
        int m_view_rect_loc{};

    protected:
        void setUniformLocations() override {
            m_mvp_matrix_loc = uniformLocation("mvp_matrix");
            m_color_loc = uniformLocation("color");
            m_point_size_loc = uniformLocation("point_size");
            m_point_style_loc = uniformLocation("point_style");
            m_view_rect_loc = uniformLocation("view_rect");
        }
        void bindAttributeLocations() override {
            bindAttributeLocation("vertex", 0);
        }

    public:
        void setMVPMatrix(const QMatrix4x4 &p_mvp_matrix) {
            setUniformValue(m_mvp_matrix_loc, p_mvp_matrix);
        }
        void setColor(const QColor &p_color) {
            setUniformValue(m_color_loc, p_color);
        }
        void setPointSize(float p_point_size) {
            setUniformValue(m_point_size_loc, p_point_size);
        }
        void setPointStyle(int p_style) {
            setUniformValue(m_point_style_loc, p_style);
        }
        void setViewRect(float p_x_min, float p_y_min, float p_x_max, float p_y_max) {
            setUniformValue(m_view_rect_loc, p_x_min, p_y_min, p_x_max, p_y_max);
        }
    };

    class LineShader : public Shader
    {
        int m_mvp_matrix_loc{};
        int m_color_loc{};
        int m_line_width_loc{};
        int m_pixels_loc{};
        int m_view_rect_loc{};

    protected:
        void setUniformLocations() override {
            m_mvp_matrix_loc = uniformLocation("mvp_matrix");
            m_color_loc = uniformLocation("color");
            m_line_width_loc = uniformLocation("line_width");
            m_pixels_loc = uniformLocation("pixels");
            m_view_rect_loc = uniformLocation("view_rect");
        }
        void bindAttributeLocations() override {
            bindAttributeLocation("vertex", 0);
        }

    public:
        void setMVPMatrix(const QMatrix4x4 &p_mvp_matrix) {
            setUniformValue(m_mvp_matrix_loc, p_mvp_matrix);
        }
        void setColor(const QColor &p_color) {
            setUniformValue(m_color_loc, p_color);
        }
        void setLineWidth(float p_line_width) {
            setUniformValue(m_line_width_loc, p_line_width);
        }
        void setPixels(float p_width, float p_height) {
            setUniformValue(m_pixels_loc, p_width, p_height);
        }
        void setViewRect(float p_x_min, float p_y_min, float p_x_max, float p_y_max) {
            setUniformValue(m_view_rect_loc, p_x_min, p_y_min, p_x_max, p_y_max);
        }
    };

    class GridShader : public Shader
    {
        int m_grid_rect_loc{};
        int m_view_rect_loc{};
    protected:
        void setUniformLocations() override {
            m_grid_rect_loc = uniformLocation("grid_rect");
            m_view_rect_loc = uniformLocation("view_rect");
        }
        void bindAttributeLocations() override {
            bindAttributeLocation("vertex", 0);
        }

    public:
        void setGridRect(float p_grid_x, float p_grid_y, float p_grid_width, float p_grid_height) {
            setUniformValue(m_grid_rect_loc, p_grid_x, p_grid_y, p_grid_width, p_grid_height);
        }
        void setViewRect(float p_x_min, float p_y_min, float p_width, float p_height) {
            setUniformValue(m_view_rect_loc, p_x_min, p_y_min, p_width, p_height);
        }
    };


    PointShader m_point_program;
    LineShader m_line_program;
    GridShader m_grid_program;
    GridShader m_grid_program_log_X;
    GridShader m_grid_program_log_Y;
    GridShader m_grid_program_log_XY;
    QMatrix4x4 m_projection;
    QMatrix4x4 m_mv_matrix;

    QList<charts::Table> m_tables;
    QList<charts::Chart> m_charts;

    struct ValueRange
    {
        Real min{}, max{};
        QList<QList<int>> cells;
        inline int getCellIndex(Real p_value, int p_cells_count) const
        {
            if (qFuzzyCompare(p_value, max)) {
                return p_cells_count-1;
            }
            return static_cast<int>((p_value-min) / (max-min) * p_cells_count);
        }
    };
    QList<QList<ValueRange>> m_ranges;

    ValueRange m_range_x, m_range_y;
    QList<QColor> m_colors;

    int m_frame_margin_left;
    int m_frame_margin_right;
    int m_frame_margin_top;
    int m_frame_margin_bottom;
    int m_chosen_point;
    int m_chosen_chart;

    QPoint m_last_mouse_pos;
    QPoint m_mouse_press_pos;
    bool m_is_mouse_pressed;

    bool m_is_zoom_frame;
    QRectF m_zoom_frame_rect;

    bool m_is_initialized;

    float m_line_width;
    float m_point_size;

    bool m_is_fixed;
    bool m_is_show_points;
    LabelStyle m_label_style;
    LogTickMarkStyle m_log_tick_mark_style;
//    bool m_is_show_grid;
//    bool m_is_show_minor_grid;
    bool m_is_dynamic_scale;
    bool m_is_show_chart_id;

    bool m_is_use_cells;
    const int m_cells_count;

    unsigned int m_id;
    static unsigned int m_id_counter;

    const float chosen_point_expansion = 4.0f;
    const float minimum_choose_range = 6.0f;
    const float context_menu_call_diff = 5.0f;

    qreal m_device_pixel_ratio;

    bool m_log_scale_x = false;
    bool m_log_scale_y = false;

    Real m_minimum_x = REAL_MIN;
    Real m_maximum_x = REAL_MAX;
    Real m_minimum_y = REAL_MIN;
    Real m_maximum_y = REAL_MAX;

public:
    ChartWindow(QWindow *p_parent = nullptr);

    /// Освобождение памяти
    void clear();

    /// Задать массив таблиц данных
    void setData(const QList<charts::Table> &p_tables);

    /// Задать таблицу данных
    void setData(const charts::Table &p_table);

    /// Задать столбцы таблицы данных
    void setData(int                       p_table_index,
                 const QList<QList<Real>> &p_columns,
                 bool                      p_update_ranges = true);

    /// Задать столбцы таблицы данных
    void setData(const charts::Chart &p_chart,
                 const QList<Real>   &p_column_x_rows,
                 const QList<Real>   &p_column_y_rows,
                 bool                 p_update_ranges = true);

    /// Задать столбцы таблицы данных
    void setData(int                          p_table_index,
                 const QList<charts::Column> &p_columns,
                 bool                         p_update_ranges = true);

    /// Добавить таблицу данных в массив таблиц
    void addData(const charts::Table &p_table);

    /// Добавить массив таблиц данных в конец массива таблиц
    void addData(const QList<charts::Table> &p_tables);

    /// Добавить строчку в таблицу данных
    void addRow(int p_table_index, int p_next_row_index, const QList<Real> &p_row = {});

    /// Удалить строчку таблицы данных
    void removeRow(int p_table_index, int p_row_index);

    /// Задать названия столбцов таблицы данных
    void setLabels(int p_table_index, const QStringList &p_labels);

    /// Задать названия столбца таблицы данных
    void setLabel(int p_table_index, int p_column_index, const QString &p_label);

    /// Являются ли валидными данные о графике
    bool valid(const charts::Chart &p_chart) const noexcept;

    /// Задать данные о позиции данных графика для отображения графика
    void setChart(const charts::Chart &p_chart);

    /// Задать индексы столбцов таблиц данных для отображения массива графиков
    void setCharts(const QList<charts::Chart> &p_charts);

    /// Задать индексы столбцов таблицы данных для отображения графика
    inline void setCoordinateColumns(int p_table, int p_column_x, int p_column_y)
    {
        setChart({ p_table, p_column_x, p_column_y });
    }

    /// Задать индексы столбцов последней таблицы данных для отображения дополнительного графика
    inline void setCoordinateColumns(int p_column_x, int p_column_y)
    {
        setChart({ static_cast<int>(m_tables.size()) - 1, p_column_x, p_column_y });
    }

    /// Добавить данные о позиции данных графика для отображения графика
    void addChart(const charts::Chart& p_chart);

    /// Добавить индексы столбцов таблицы данных для отображения дополнительного графика
    inline void addCoordinateColumns(int p_table, int p_column_x, int p_column_y)
    {
        addChart({ p_table, p_column_x, p_column_y });
    }

    /// Добавить индексы столбцов последней таблицы данных для отображения дополнительного графика
    inline void addCoordinateColumns(int p_column_x, int p_column_y)
    {
        addChart({ static_cast<int>(m_tables.size()) - 1, p_column_x, p_column_y });
    }

    /// Задать пределы изменения координат по X
    void setRangeX(Real p_minimum, Real p_maximum);

    /// Задать пределы изменения координат по Y
    void setRangeY(Real p_minimum, Real p_maximum);

    /// Выбрать, график с закреплёнными точками или редактируемый
    void setFixed(bool p_is_fixed);

    /// Выбрать, подбирать ли масштаб под содержимое
    void setDynamicScale(bool p_is_dynamic);

    /// Выбрать, использовать ли распределение значений столбцов по ячейкам.
    /// Ускоряет поиск выделеной точки, но замедляет редактирование.
    void setUseSelectionCells(bool p_use);

    /// Выбрать, показывать точки или нет
    void setShowPoints(bool p_is_show);

    /// Выбрать стиль подписей осей
    void setLabelStyle(LabelStyle p_style);

    /// Выбрать стиль подписей осей логарифмического масштаба
    void setLogTickMarkStyle(LogTickMarkStyle p_style);

    /// Задать ширину линий
    void setLineWidth(float p_line_width);

    /// Задать размер точек
    void setPointSize(float p_point_size);

    /// Выбрать, подписывать ли название таблицы данных при выделении точки
    void setShowChartId(bool p_is_show);

    /// Вернуть все таблицы данных
    inline const QList<charts::Table>& data() const noexcept
    {
        return m_tables;
    }

    /// Вернуть таблицу данных по номеру
    inline const charts::Table& data(int p_table) const
    {
        return m_tables[p_table];
    }

    /// Вернуть данные об отображаемых графиках
    inline const QList<charts::Chart>& charts() const noexcept
    {
        return m_charts;
    }

    QColor getChartColor(int p_chart_index) const;

protected:
    void findValueRange(const QList<Real> &p_values, ValueRange &o_range) const;

    void updateRange(int p_table_index, int p_column_index);

    void findMinMax();

    Real projectX(Real p_obj_x) const;
    Real projectY(Real p_obj_y) const;
    Real unProjectX(Real p_win_x) const;
    Real unProjectY(Real p_win_y) const;

    inline QPair<Real, Real> getChartPoint(int p_chart, int p_point) const
    {
        const auto &chart = m_charts[p_chart];
        return { m_tables[chart.table_id].columns[chart.column_x].rows[p_point],
                 m_tables[chart.table_id].columns[chart.column_y].rows[p_point] };
    }

    inline QPair<QString, QString> getChartColumnPairIDs(int p_chart) const
    {
        const auto &chart = m_charts[p_chart];
        return { m_tables[chart.table_id].columns[chart.column_x].id,
                 m_tables[chart.table_id].columns[chart.column_y].id };
    }

    void addLineBuffer(const charts::Chart &p_chart);
    void clearLineBuffers();
    void createPalette();
    void createShaders();

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *p_event) override;
    void mouseReleaseEvent(QMouseEvent *p_event) override;
    void mouseMoveEvent(QMouseEvent *p_event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *p_event) override;
    void keyPressEvent(QKeyEvent *p_event) override;

    void showContextMenu(QMouseEvent *p_event);
    void showChangePointDialog();

    /// Обновить координаты выделенной точки в буфере соответствующего графика
    void updateChosenPointBufferData();

    /// Обновить буферы всех графиков без изменения размеров
    void updateBufferData();

    /// Обновить буфер графика без изменения размера
    void updateBufferData(int p_chart_index);

    /// Обновить буфер графика с изменением размера
    void resizeAndUpdateBufferData(int p_chart_index);

    /// Обновить буфер сетки
    void updateGridBufferData();

    /// Обновить буфер рамки масштабирования
    void updateZoomFrameBufferData();

public slots:
    void viewAll();
    void setLogScaleX(bool p_log_scale_x);
    void setLogScaleY(bool p_log_scale_y);

signals:
    /// Сигнал. Инициализирован OpenGL контекст
    void contextInitialized();

    /// Сигнал. Изменены данные таблицы p_table в столбцах p_column_x и p_column_y
    void dataChanged(int p_table, int p_column_x, int p_column_y);
};

#endif // CHART_WINDOW_H
