/****************************************************************************
** Meta object code from reading C++ file 'ChartWindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../../../../src/Client/Shared/Chart/ChartWindow/ChartWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ChartWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ChartWindow_t {
    const uint offsetsAndSize[24];
    char stringdata0[136];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_ChartWindow_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_ChartWindow_t qt_meta_stringdata_ChartWindow = {
    {
QT_MOC_LITERAL(0, 11), // "ChartWindow"
QT_MOC_LITERAL(12, 18), // "contextInitialized"
QT_MOC_LITERAL(31, 0), // ""
QT_MOC_LITERAL(32, 11), // "dataChanged"
QT_MOC_LITERAL(44, 7), // "p_table"
QT_MOC_LITERAL(52, 10), // "p_column_x"
QT_MOC_LITERAL(63, 10), // "p_column_y"
QT_MOC_LITERAL(74, 7), // "viewAll"
QT_MOC_LITERAL(82, 12), // "setLogScaleX"
QT_MOC_LITERAL(95, 13), // "p_log_scale_x"
QT_MOC_LITERAL(109, 12), // "setLogScaleY"
QT_MOC_LITERAL(122, 13) // "p_log_scale_y"

    },
    "ChartWindow\0contextInitialized\0\0"
    "dataChanged\0p_table\0p_column_x\0"
    "p_column_y\0viewAll\0setLogScaleX\0"
    "p_log_scale_x\0setLogScaleY\0p_log_scale_y"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChartWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   44,    2, 0x06,    1 /* Public */,
       3,    3,   45,    2, 0x06,    2 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       7,    0,   52,    2, 0x0a,    6 /* Public */,
       8,    1,   53,    2, 0x0a,    7 /* Public */,
      10,    1,   56,    2, 0x0a,    9 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    4,    5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,   11,

       0        // eod
};

void ChartWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ChartWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->contextInitialized(); break;
        case 1: _t->dataChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 2: _t->viewAll(); break;
        case 3: _t->setLogScaleX((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->setLogScaleY((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ChartWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChartWindow::contextInitialized)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ChartWindow::*)(int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChartWindow::dataChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject ChartWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QOpenGLWindow::staticMetaObject>(),
    qt_meta_stringdata_ChartWindow.offsetsAndSize,
    qt_meta_data_ChartWindow,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_ChartWindow_t
, QtPrivate::TypeAndForceComplete<ChartWindow, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>


>,
    nullptr
} };


const QMetaObject *ChartWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChartWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ChartWindow.stringdata0))
        return static_cast<void*>(this);
    return QOpenGLWindow::qt_metacast(_clname);
}

int ChartWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void ChartWindow::contextInitialized()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ChartWindow::dataChanged(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
