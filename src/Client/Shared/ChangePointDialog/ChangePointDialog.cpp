// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "ChangePointDialog.h"

#include <QDialogButtonBox>
#include <QPushButton>
#include <QDoubleSpinBox>

#include <QLineEdit>
#include <QValidator>

#include <QFormLayout>

ChangePointDialog::ChangePointDialog(const QString &p_title,
                                     Real           p_x,
                                     Real           p_y,
                                     const QString &p_name_x,
                                     const QString &p_name_y,
                                     const QString &p_name_z,
                                     Real           p_z,
                                     QWidget       *p_parent):
QDialog(p_parent, Qt::Dialog | Qt::WindowCloseButtonHint)
{
    m_button_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    m_button_box->button(QDialogButtonBox::Ok)->setText(tr("OK"));
    m_button_box->button(QDialogButtonBox::Cancel)->setText(tr("Отмена"));

    connect(m_button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

    m_line_edit_x = new QLineEdit(this);
    m_line_edit_y = new QLineEdit(this);

    QFormLayout *layout = new QFormLayout(this);
    layout->addRow(p_name_x + ":", m_line_edit_x);
    layout->addRow(p_name_y + ":", m_line_edit_y);

    m_coordinate_validator_x = new QDoubleValidator(REAL_MIN, REAL_MAX, 6, this);
    m_coordinate_validator_x->setNotation(QDoubleValidator::ScientificNotation);
    m_coordinate_validator_y = new QDoubleValidator(REAL_MIN, REAL_MAX, 6, this);
    m_coordinate_validator_y->setNotation(QDoubleValidator::ScientificNotation);

    m_line_edit_x->setValidator(m_coordinate_validator_x);
    m_line_edit_y->setValidator(m_coordinate_validator_y);

    m_line_edit_x->setText(QString::number(p_x));
    m_line_edit_y->setText(QString::number(p_y));

    if(!p_name_z.isEmpty()) {
        m_line_edit_z = new QLineEdit(this);
        layout->addRow(p_name_z + ":", m_line_edit_z);
        m_coordinate_validator_z = new QDoubleValidator(REAL_MIN, REAL_MAX, 6, this);
        m_coordinate_validator_z->setNotation(QDoubleValidator::ScientificNotation);
        m_line_edit_z->setValidator(m_coordinate_validator_z);
        m_line_edit_z->setText(QString::number(p_z));
    } else {
        m_coordinate_validator_z = nullptr;
    }

    layout->addRow(m_button_box);
    setLayout(layout);
    setWindowTitle(p_title);
}


void ChangePointDialog::setRangeX(Real p_minimum, Real p_maximum)
{
    m_coordinate_validator_x->setRange(p_minimum, p_maximum);
}


void ChangePointDialog::setRangeY(Real p_minimum, Real p_maximum)
{
    m_coordinate_validator_y->setRange(p_minimum, p_maximum);
}


void ChangePointDialog::setRangeZ(Real p_minimum, Real p_maximum)
{
    if (m_coordinate_validator_z) {
        m_coordinate_validator_z->setRange(p_minimum, p_maximum);
    }
}


Real ChangePointDialog::pointX() const noexcept
{
    return m_line_edit_x->text().toDouble();
}


Real ChangePointDialog::pointY() const noexcept
{
    return m_line_edit_y->text().toDouble();
}


Real ChangePointDialog::pointZ() const noexcept
{
    if(m_line_edit_z)
        return m_line_edit_z->text().toDouble();

    return REAL_MAX;
}
