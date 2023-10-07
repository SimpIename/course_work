#ifndef CHANGE_POINT_DIALOG_H
#define CHANGE_POINT_DIALOG_H

#include <QDialog>

#include <Defs>

QT_FORWARD_DECLARE_CLASS(QDialogButtonBox)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QDoubleValidator)

class ChangePointDialog final : public QDialog
{
    Q_OBJECT

public:

    explicit ChangePointDialog(const QString &p_title,
                               Real           p_x,
                               Real           p_y,
                               const QString &p_name_x,
                               const QString &p_name_y,
                               const QString &p_name_z = QString(),
                               Real           p_z = REAL_MAX,
                               QWidget       *p_parent = nullptr);

    void setRangeX(Real p_minimum, Real p_maximum);

    void setRangeY(Real p_minimum, Real p_maximum);

    void setRangeZ(Real p_minimum, Real p_maximum);

    Real pointX() const noexcept;

    Real pointY() const noexcept;

    Real pointZ() const noexcept;

private:

    QLineEdit        *m_line_edit_x;
    QLineEdit        *m_line_edit_y;
    QLineEdit        *m_line_edit_z;
    QDoubleValidator *m_coordinate_validator_x;
    QDoubleValidator *m_coordinate_validator_y;
    QDoubleValidator *m_coordinate_validator_z;
    QDialogButtonBox *m_button_box;
};

#endif // CHANGE_POINT_DIALOG_H
