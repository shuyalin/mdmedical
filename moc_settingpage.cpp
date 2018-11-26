/****************************************************************************
** Meta object code from reading C++ file 'settingpage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "settingpage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settingpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CSettingPage_t {
    QByteArrayData data[13];
    char stringdata0[214];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CSettingPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CSettingPage_t qt_meta_stringdata_CSettingPage = {
    {
QT_MOC_LITERAL(0, 0, 12), // "CSettingPage"
QT_MOC_LITERAL(1, 13, 24), // "GetCureCycleCurrentValue"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 24), // "GetTargetTmpCurrentValue"
QT_MOC_LITERAL(4, 64, 23), // "GetMaxPowerCurrentValue"
QT_MOC_LITERAL(5, 88, 22), // "GetCurePosCurrentValue"
QT_MOC_LITERAL(6, 111, 12), // "GetYearValue"
QT_MOC_LITERAL(7, 124, 13), // "GetMonthValue"
QT_MOC_LITERAL(8, 138, 11), // "GetDayValue"
QT_MOC_LITERAL(9, 150, 12), // "GetHourValue"
QT_MOC_LITERAL(10, 163, 14), // "GetMinuteValue"
QT_MOC_LITERAL(11, 178, 16), // "CloseSettingPage"
QT_MOC_LITERAL(12, 195, 18) // "CloseSettingOkPage"

    },
    "CSettingPage\0GetCureCycleCurrentValue\0"
    "\0GetTargetTmpCurrentValue\0"
    "GetMaxPowerCurrentValue\0GetCurePosCurrentValue\0"
    "GetYearValue\0GetMonthValue\0GetDayValue\0"
    "GetHourValue\0GetMinuteValue\0"
    "CloseSettingPage\0CloseSettingOkPage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CSettingPage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x08 /* Private */,
       3,    0,   70,    2, 0x08 /* Private */,
       4,    0,   71,    2, 0x08 /* Private */,
       5,    0,   72,    2, 0x08 /* Private */,
       6,    0,   73,    2, 0x08 /* Private */,
       7,    0,   74,    2, 0x08 /* Private */,
       8,    0,   75,    2, 0x08 /* Private */,
       9,    0,   76,    2, 0x08 /* Private */,
      10,    0,   77,    2, 0x08 /* Private */,
      11,    0,   78,    2, 0x08 /* Private */,
      12,    0,   79,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CSettingPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CSettingPage *_t = static_cast<CSettingPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->GetCureCycleCurrentValue(); break;
        case 1: _t->GetTargetTmpCurrentValue(); break;
        case 2: _t->GetMaxPowerCurrentValue(); break;
        case 3: _t->GetCurePosCurrentValue(); break;
        case 4: _t->GetYearValue(); break;
        case 5: _t->GetMonthValue(); break;
        case 6: _t->GetDayValue(); break;
        case 7: _t->GetHourValue(); break;
        case 8: _t->GetMinuteValue(); break;
        case 9: _t->CloseSettingPage(); break;
        case 10: _t->CloseSettingOkPage(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CSettingPage::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CSettingPage.data,
      qt_meta_data_CSettingPage,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CSettingPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CSettingPage::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CSettingPage.stringdata0))
        return static_cast<void*>(const_cast< CSettingPage*>(this));
    return QDialog::qt_metacast(_clname);
}

int CSettingPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
