#ifndef TYPE_CONVERTER_H
#define TYPE_CONVERTER_H

#include <string>
#include <cstdint>
#include <variant>
#include "../structs/ValueWithTypeObject.h"

class TypeConverter {
public:
    static int64_t toInt64(const std::string& str);
    static uint64_t toUInt64(const std::string& str);
    static double toDouble(const std::string& str);
    static bool toBool(const std::string& str);
    static std::string variantToString(const ValueTypeVariant& value);
    static std::string getTypeName(const ValueTypeVariant& value);
};

#endif
