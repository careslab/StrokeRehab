/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[19];
    char stringdata0[404];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 19), // "on_FillInfo_clicked"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 19), // "on_SaveInfo_clicked"
QT_MOC_LITERAL(4, 52, 21), // "on_LeftHanded_toggled"
QT_MOC_LITERAL(5, 74, 7), // "checked"
QT_MOC_LITERAL(6, 82, 22), // "on_RightHanded_toggled"
QT_MOC_LITERAL(7, 105, 25), // "on_StrokedLeftArm_toggled"
QT_MOC_LITERAL(8, 131, 26), // "on_StrokedRightArm_toggled"
QT_MOC_LITERAL(9, 158, 25), // "on_StrokedBothArm_toggled"
QT_MOC_LITERAL(10, 184, 35), // "on_GloveIntensitySlider_slide..."
QT_MOC_LITERAL(11, 220, 8), // "position"
QT_MOC_LITERAL(12, 229, 26), // "on_EnableMirroring_clicked"
QT_MOC_LITERAL(13, 256, 31), // "on_SetMirroringStrength_clicked"
QT_MOC_LITERAL(14, 288, 27), // "on_SetGloveDeadzone_clicked"
QT_MOC_LITERAL(15, 316, 21), // "on_DrawShape1_clicked"
QT_MOC_LITERAL(16, 338, 21), // "on_DrawShape2_clicked"
QT_MOC_LITERAL(17, 360, 21), // "on_DrawShape3_clicked"
QT_MOC_LITERAL(18, 382, 21) // "on_DrawShape4_clicked"

    },
    "MainWindow\0on_FillInfo_clicked\0\0"
    "on_SaveInfo_clicked\0on_LeftHanded_toggled\0"
    "checked\0on_RightHanded_toggled\0"
    "on_StrokedLeftArm_toggled\0"
    "on_StrokedRightArm_toggled\0"
    "on_StrokedBothArm_toggled\0"
    "on_GloveIntensitySlider_sliderMoved\0"
    "position\0on_EnableMirroring_clicked\0"
    "on_SetMirroringStrength_clicked\0"
    "on_SetGloveDeadzone_clicked\0"
    "on_DrawShape1_clicked\0on_DrawShape2_clicked\0"
    "on_DrawShape3_clicked\0on_DrawShape4_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    0,   90,    2, 0x08 /* Private */,
       4,    1,   91,    2, 0x08 /* Private */,
       6,    1,   94,    2, 0x08 /* Private */,
       7,    1,   97,    2, 0x08 /* Private */,
       8,    1,  100,    2, 0x08 /* Private */,
       9,    1,  103,    2, 0x08 /* Private */,
      10,    1,  106,    2, 0x08 /* Private */,
      12,    0,  109,    2, 0x08 /* Private */,
      13,    0,  110,    2, 0x08 /* Private */,
      14,    0,  111,    2, 0x08 /* Private */,
      15,    0,  112,    2, 0x08 /* Private */,
      16,    0,  113,    2, 0x08 /* Private */,
      17,    0,  114,    2, 0x08 /* Private */,
      18,    0,  115,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_FillInfo_clicked(); break;
        case 1: _t->on_SaveInfo_clicked(); break;
        case 2: _t->on_LeftHanded_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->on_RightHanded_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_StrokedLeftArm_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->on_StrokedRightArm_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->on_StrokedBothArm_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->on_GloveIntensitySlider_sliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_EnableMirroring_clicked(); break;
        case 9: _t->on_SetMirroringStrength_clicked(); break;
        case 10: _t->on_SetGloveDeadzone_clicked(); break;
        case 11: _t->on_DrawShape1_clicked(); break;
        case 12: _t->on_DrawShape2_clicked(); break;
        case 13: _t->on_DrawShape3_clicked(); break;
        case 14: _t->on_DrawShape4_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
