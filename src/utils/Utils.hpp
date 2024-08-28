#ifndef UTILS_HPP
#define UTILS_HPP

#include <QMetaEnum>


template<typename QEnum>
const char* QtEnumToString(const QEnum value) {
    return QMetaEnum::fromType<QEnum>().valueToKey(value);
}


#endif // UTILS_HPP
