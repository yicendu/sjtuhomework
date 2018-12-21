/****************************************************************************
** Meta object code from reading C++ file 'tdwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../tdwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tdwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TDWidget_t {
    QByteArrayData data[14];
    char stringdata0[121];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TDWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TDWidget_t qt_meta_stringdata_TDWidget = {
    {
QT_MOC_LITERAL(0, 0, 8), // "TDWidget"
QT_MOC_LITERAL(1, 9, 13), // "loadObjObject"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 8), // "fileName"
QT_MOC_LITERAL(4, 33, 8), // "filePath"
QT_MOC_LITERAL(5, 42, 14), // "SetBoundaryBox"
QT_MOC_LITERAL(6, 57, 8), // "Vector3f"
QT_MOC_LITERAL(7, 66, 4), // "bmin"
QT_MOC_LITERAL(8, 71, 4), // "bmax"
QT_MOC_LITERAL(9, 76, 6), // "hideIt"
QT_MOC_LITERAL(10, 83, 10), // "selectFile"
QT_MOC_LITERAL(11, 94, 10), // "deleteFile"
QT_MOC_LITERAL(12, 105, 11), // "showAllFile"
QT_MOC_LITERAL(13, 117, 3) // "COI"

    },
    "TDWidget\0loadObjObject\0\0fileName\0"
    "filePath\0SetBoundaryBox\0Vector3f\0bmin\0"
    "bmax\0hideIt\0selectFile\0deleteFile\0"
    "showAllFile\0COI"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TDWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x0a /* Public */,
       5,    2,   54,    2, 0x0a /* Public */,
       9,    0,   59,    2, 0x0a /* Public */,
      10,    2,   60,    2, 0x0a /* Public */,
      11,    1,   65,    2, 0x0a /* Public */,
      12,    3,   68,    2, 0x0a /* Public */,
      12,    2,   75,    2, 0x2a /* Public | MethodCloned */,

 // slots: parameters
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 6,    7,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QStringList, QMetaType::QStringList, QMetaType::Int,    3,    4,   13,
    QMetaType::Void, QMetaType::QStringList, QMetaType::QStringList,    3,    4,

       0        // eod
};

void TDWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TDWidget *_t = static_cast<TDWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { bool _r = _t->loadObjObject((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->SetBoundaryBox((*reinterpret_cast< const Vector3f(*)>(_a[1])),(*reinterpret_cast< const Vector3f(*)>(_a[2]))); break;
        case 2: _t->hideIt(); break;
        case 3: _t->selectFile((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->deleteFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->showAllFile((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 6: _t->showAllFile((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TDWidget::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_TDWidget.data,
      qt_meta_data_TDWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TDWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TDWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TDWidget.stringdata0))
        return static_cast<void*>(this);
    return QGLWidget::qt_metacast(_clname);
}

int TDWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
