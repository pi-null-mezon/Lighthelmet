/****************************************************************************
** Meta object code from reading C++ file 'qspprocessor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../PC/qspprocessor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qspprocessor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QSPProcessor_t {
    QByteArrayData data[8];
    char stringdata0[93];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QSPProcessor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QSPProcessor_t qt_meta_stringdata_QSPProcessor = {
    {
QT_MOC_LITERAL(0, 0, 12), // "QSPProcessor"
QT_MOC_LITERAL(1, 13, 11), // "__showError"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 28), // "QSerialPort::SerialPortError"
QT_MOC_LITERAL(4, 55, 8), // "_errorid"
QT_MOC_LITERAL(5, 64, 10), // "__readData"
QT_MOC_LITERAL(6, 75, 11), // "__writeData"
QT_MOC_LITERAL(7, 87, 5) // "_data"

    },
    "QSPProcessor\0__showError\0\0"
    "QSerialPort::SerialPortError\0_errorid\0"
    "__readData\0__writeData\0_data"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QSPProcessor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x08 /* Private */,
       5,    0,   32,    2, 0x08 /* Private */,
       6,    1,   33,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::LongLong, QMetaType::QByteArray,    7,

       0        // eod
};

void QSPProcessor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QSPProcessor *_t = static_cast<QSPProcessor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->__showError((*reinterpret_cast< QSerialPort::SerialPortError(*)>(_a[1]))); break;
        case 1: _t->__readData(); break;
        case 2: { qint64 _r = _t->__writeData((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< qint64*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject QSPProcessor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QSPProcessor.data,
      qt_meta_data_QSPProcessor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QSPProcessor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QSPProcessor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QSPProcessor.stringdata0))
        return static_cast<void*>(const_cast< QSPProcessor*>(this));
    return QObject::qt_metacast(_clname);
}

int QSPProcessor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
