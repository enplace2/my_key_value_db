//
// Created by Nick Place on 1/28/24.
//



#ifndef MY_KEY_VALUE_DB_VALUEWITHTYPEOBJECT_H
#define MY_KEY_VALUE_DB_VALUEWITHTYPEOBJECT_H

#endif //MY_KEY_VALUE_DB_VALUEWITHTYPEOBJECT_H

struct ValueWithTypeObject;

using KVMap = std::unordered_map<std::string, ValueWithTypeObject>;

//note recursive reference to ValueWithTypeObject. This enables nested objects to be stored
using ValueTypeVariant = std::variant<std::string, int64_t, bool, double, uint64_t, KVMap>;

struct ValueWithTypeObject {
    ValueTypeVariant value;
    std::string type;
};