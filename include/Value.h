#pragma once
#include <string>

enum class ValueType {
    INT,
    BOOL,
    STRING
};

struct Value {
    ValueType type;
    int intVal;
    bool boolVal;
    std::string strVal;

    Value() : type(ValueType::INT), intVal(0), boolVal(false) {}
    Value(int v) : type(ValueType::INT), intVal(v), boolVal(false) {}
    Value(bool v) : type(ValueType::BOOL), intVal(0), boolVal(v) {}
    Value(std::string v) : type(ValueType::STRING), intVal(0), boolVal(false), strVal(v) {}
    Value(const char* v) : type(ValueType::STRING), intVal(0), boolVal(false), strVal(v) {}
};

inline std::string valueToString(const Value& val) {
    if (val.type == ValueType::INT) {
        return std::to_string(val.intVal);
    } else if (val.type == ValueType::BOOL) {
        return val.boolVal ? "true" : "false";
    } else if (val.type == ValueType::STRING) {
        return val.strVal;
    }
    return "unknown";
}
