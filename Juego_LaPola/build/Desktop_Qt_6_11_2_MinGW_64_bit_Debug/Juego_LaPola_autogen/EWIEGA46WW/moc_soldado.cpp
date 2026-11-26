/****************************************************************************
** Meta object code from reading C++ file 'soldado.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../soldado.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'soldado.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN7SoldadoE_t {};
} // unnamed namespace

template <> constexpr inline auto Soldado::qt_create_metaobjectdata<qt_meta_tag_ZN7SoldadoE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Soldado"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Soldado, qt_meta_tag_ZN7SoldadoE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Soldado::staticMetaObject = { {
    QMetaObject::SuperData::link<Enemigo::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7SoldadoE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7SoldadoE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7SoldadoE_t>.metaTypes,
    nullptr
} };

void Soldado::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Soldado *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *Soldado::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Soldado::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7SoldadoE_t>.strings))
        return static_cast<void*>(this);
    return Enemigo::qt_metacast(_clname);
}

int Soldado::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Enemigo::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
