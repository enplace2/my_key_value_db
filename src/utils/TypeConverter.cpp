#include "TypeConverter.h"
#include <algorithm>
#include <stdexcept>

int64_t TypeConverter::toInt64(const std::string& str) {
    try {
        size_t pos;
        int64_t result = std::stoll(str, &pos);
        if (pos != str.length()) {
            throw std::invalid_argument("Invalid integer format");
        }
        return result;
    } catch (const std::invalid_argument&) {
        throw std::invalid_argument("Cannot convert '" + str + "' to int64");
    } catch (const std::out_of_range&) {
        throw std::invalid_argument("Value '" + str + "' is out of range for int64");
    }
}

uint64_t TypeConverter::toUInt64(const std::string& str) {
    if (str.find('-') != std::string::npos) {
        throw std::invalid_argument("Cannot convert negative value to uint64");
    }
    try {
        size_t pos;
        uint64_t result = std::stoull(str, &pos);
        if (pos != str.length()) {
            throw std::invalid_argument("Invalid unsigned integer format");
        }
        return result;
    } catch (const std::invalid_argument&) {
        throw std::invalid_argument("Cannot convert '" + str + "' to uint64");
    } catch (const std::out_of_range&) {
        throw std::invalid_argument("Value '" + str + "' is out of range for uint64");
    }
}

double TypeConverter::toDouble(const std::string& str) {
    try {
        size_t pos;
        double result = std::stod(str, &pos);
        if (pos != str.length()) {
            throw std::invalid_argument("Invalid double format");
        }
        return result;
    } catch (const std::invalid_argument&) {
        throw std::invalid_argument("Cannot convert '" + str + "' to double");
    } catch (const std::out_of_range&) {
        throw std::invalid_argument("Value '" + str + "' is out of range for double");
    }
}

bool TypeConverter::toBool(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if (lower == "true") {
        return true;
    } else if (lower == "false") {
        return false;
    } else {
        throw std::invalid_argument("Cannot convert '" + str + "' to bool. Expected 'true' or 'false'");
    }
}

std::string TypeConverter::variantToString(const ValueTypeVariant& value) {
    return std::visit([](auto&& arg) -> std::string {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::string>) {
            return arg;
        } else if constexpr (std::is_same_v<T, int64_t>) {
            return std::to_string(arg);
        } else if constexpr (std::is_same_v<T, uint64_t>) {
            return std::to_string(arg);
        } else if constexpr (std::is_same_v<T, double>) {
            return std::to_string(arg);
        } else if constexpr (std::is_same_v<T, bool>) {
            return arg ? "true" : "false";
        } else if constexpr (std::is_same_v<T, KVMap>) {
            return "{map}";
        }
    }, value);
}

std::string TypeConverter::getTypeName(const ValueTypeVariant& value) {
    return std::visit([](auto&& arg) -> std::string {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::string>) {
            return "string";
        } else if constexpr (std::is_same_v<T, int64_t>) {
            return "int";
        } else if constexpr (std::is_same_v<T, uint64_t>) {
            return "uint";
        } else if constexpr (std::is_same_v<T, double>) {
            return "double";
        } else if constexpr (std::is_same_v<T, bool>) {
            return "bool";
        } else if constexpr (std::is_same_v<T, KVMap>) {
            return "map";
        }
    }, value);
}
