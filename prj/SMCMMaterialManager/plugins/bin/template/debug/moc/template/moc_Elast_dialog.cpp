/****************************************************************************
** Meta object code from reading C++ file 'Elast_dialog.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../plugins/Elast_mod_new/Elast_dialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Elast_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ElastDialog_t {
    const uint offsetsAndSize[14];
    char stringdata0[137];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_ElastDialog_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_ElastDialog_t qt_meta_stringdata_ElastDialog = {
    {
QT_MOC_LITERAL(0, 11), // "ElastDialog"
QT_MOC_LITERAL(12, 26), // "updatePropertyValuesWidget"
QT_MOC_LITERAL(39, 0), // ""
QT_MOC_LITERAL(40, 22), // "updateScalarDataWidget"
QT_MOC_LITERAL(63, 23), // "updateDependencesWidget"
QT_MOC_LITERAL(87, 26), // "updateDependenceDataWidget"
QT_MOC_LITERAL(114, 22) // "on_calculatorE_clicked"

    },
    "ElastDialog\0updatePropertyValuesWidget\0"
    "\0updateScalarDataWidget\0updateDependencesWidget\0"
    "updateDependenceDataWidget\0"
    "on_calculatorE_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ElastDialog[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   44,    2, 0x08,    1 /* Private */,
       3,    0,   45,    2, 0x08,    2 /* Private */,
       4,    0,   46,    2, 0x08,    3 /* Private */,
       5,    0,   47,    2, 0x08,    4 /* Private */,
       6,    0,   48,    2, 0x08,    5 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ElastDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ElastDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->updatePropertyValuesWidget(); break;
        case 1: _t->updateScalarDataWidget(); break;
        case 2: _t->updateDependencesWidget(); break;
        case 3: _t->updateDependenceDataWidget(); break;
        case 4: _t->on_calculatorE_clicked(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject ElastDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<AbstractInterfaceDialog::staticMetaObject>(),
    qt_meta_stringdata_ElastDialog.offsetsAndSize,
    qt_meta_data_ElastDialog,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_ElastDialog_t
, QtPrivate::TypeAndForceComplete<ElastDialog, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *ElastDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ElastDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ElastDialog.stringdata0))
        return static_cast<void*>(this);
    return AbstractInterfaceDialog::qt_metacast(_clname);
}

int ElastDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractInterfaceDialog::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_PluginInterface_t {
    const uint offsetsAndSize[2];
    char stringdata0[16];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_PluginInterface_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_PluginInterface_t qt_meta_stringdata_PluginInterface = {
    {
QT_MOC_LITERAL(0, 15) // "PluginInterface"

    },
    "PluginInterface"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PluginInterface[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void PluginInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject PluginInterface::staticMetaObject = { {
    QMetaObject::SuperData::link<AbstractInterface::staticMetaObject>(),
    qt_meta_stringdata_PluginInterface.offsetsAndSize,
    qt_meta_data_PluginInterface,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_PluginInterface_t
, QtPrivate::TypeAndForceComplete<PluginInterface, std::true_type>



>,
    nullptr
} };


const QMetaObject *PluginInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PluginInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PluginInterface.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "SMCM_MATERIAL_MANAGER_PLUGIN"))
        return static_cast< AbstractInterface*>(this);
    return AbstractInterface::qt_metacast(_clname);
}

int PluginInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractInterface::qt_metacall(_c, _id, _a);
    return _id;
}

QT_PLUGIN_METADATA_SECTION
static constexpr unsigned char qt_pluginMetaData_PluginInterface[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', '!',
    // metadata version, Qt version, architectural requirements
    0, QT_VERSION_MAJOR, QT_VERSION_MINOR, qPluginArchRequirements(),
    0xbf, 
    // "IID"
    0x02,  0x78,  0x1c,  'S',  'M',  'C',  'M',  '_', 
    'M',  'A',  'T',  'E',  'R',  'I',  'A',  'L', 
    '_',  'M',  'A',  'N',  'A',  'G',  'E',  'R', 
    '_',  'P',  'L',  'U',  'G',  'I',  'N', 
    // "className"
    0x03,  0x6f,  'P',  'l',  'u',  'g',  'i',  'n', 
    'I',  'n',  't',  'e',  'r',  'f',  'a',  'c', 
    'e', 
    0xff, 
};
QT_MOC_EXPORT_PLUGIN(PluginInterface, PluginInterface)

QT_WARNING_POP
QT_END_MOC_NAMESPACE
