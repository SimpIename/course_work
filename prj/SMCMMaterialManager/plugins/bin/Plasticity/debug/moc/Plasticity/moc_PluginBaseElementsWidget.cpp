/****************************************************************************
** Meta object code from reading C++ file 'PluginBaseElementsWidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../common/src/PluginBaseElementsWidget/PluginBaseElementsWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PluginBaseElementsWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PluginBaseElementsWidget_t {
    const uint offsetsAndSize[32];
    char stringdata0[256];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_PluginBaseElementsWidget_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_PluginBaseElementsWidget_t qt_meta_stringdata_PluginBaseElementsWidget = {
    {
QT_MOC_LITERAL(0, 24), // "PluginBaseElementsWidget"
QT_MOC_LITERAL(25, 6), // "accept"
QT_MOC_LITERAL(32, 0), // ""
QT_MOC_LITERAL(33, 14), // "addScalarValue"
QT_MOC_LITERAL(48, 8), // "addTable"
QT_MOC_LITERAL(57, 14), // "addTableColumn"
QT_MOC_LITERAL(72, 13), // "addTableValue"
QT_MOC_LITERAL(86, 18), // "tableValuesChanged"
QT_MOC_LITERAL(105, 18), // "scalarValueChanged"
QT_MOC_LITERAL(124, 11), // "QModelIndex"
QT_MOC_LITERAL(136, 9), // "p_topLeft"
QT_MOC_LITERAL(146, 12), // "p_botomRight"
QT_MOC_LITERAL(159, 20), // "propertyValueChanged"
QT_MOC_LITERAL(180, 23), // "on_acceptButton_clicked"
QT_MOC_LITERAL(204, 25), // "on_loadTextButton_clicked"
QT_MOC_LITERAL(230, 25) // "on_saveTextButton_clicked"

    },
    "PluginBaseElementsWidget\0accept\0\0"
    "addScalarValue\0addTable\0addTableColumn\0"
    "addTableValue\0tableValuesChanged\0"
    "scalarValueChanged\0QModelIndex\0p_topLeft\0"
    "p_botomRight\0propertyValueChanged\0"
    "on_acceptButton_clicked\0"
    "on_loadTextButton_clicked\0"
    "on_saveTextButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PluginBaseElementsWidget[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   80,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   81,    2, 0x08,    2 /* Private */,
       4,    0,   82,    2, 0x08,    3 /* Private */,
       5,    0,   83,    2, 0x08,    4 /* Private */,
       6,    0,   84,    2, 0x08,    5 /* Private */,
       7,    0,   85,    2, 0x08,    6 /* Private */,
       8,    2,   86,    2, 0x08,    7 /* Private */,
      12,    2,   91,    2, 0x08,   10 /* Private */,
      13,    0,   96,    2, 0x08,   13 /* Private */,
      14,    0,   97,    2, 0x08,   14 /* Private */,
      15,    0,   98,    2, 0x08,   15 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 9,   10,   11,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 9,   10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PluginBaseElementsWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PluginBaseElementsWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->accept(); break;
        case 1: _t->addScalarValue(); break;
        case 2: _t->addTable(); break;
        case 3: _t->addTableColumn(); break;
        case 4: _t->addTableValue(); break;
        case 5: _t->tableValuesChanged(); break;
        case 6: _t->scalarValueChanged((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[2]))); break;
        case 7: _t->propertyValueChanged((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[2]))); break;
        case 8: _t->on_acceptButton_clicked(); break;
        case 9: _t->on_loadTextButton_clicked(); break;
        case 10: _t->on_saveTextButton_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PluginBaseElementsWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PluginBaseElementsWidget::accept)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject PluginBaseElementsWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_PluginBaseElementsWidget.offsetsAndSize,
    qt_meta_data_PluginBaseElementsWidget,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_PluginBaseElementsWidget_t
, QtPrivate::TypeAndForceComplete<PluginBaseElementsWidget, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>, QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>, QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *PluginBaseElementsWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PluginBaseElementsWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PluginBaseElementsWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PluginBaseElementsWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void PluginBaseElementsWidget::accept()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
