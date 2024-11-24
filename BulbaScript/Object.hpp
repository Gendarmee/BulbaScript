#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <variant>
#include <iostream>
#include <memory>
#include <optional>

enum class ObjType {
    String,    // string
    Number,    // double
    Boolean,   // bool
    Nil,       // null
    Function,  // function
    Instance,  // instance of a class
    Class      // class
};

class Object {
public:
    using ValueType = std::variant<std::wstring, double, bool, std::nullptr_t>;

    Object();
    explicit Object(ObjType type, ValueType value);

    std::wstring toString() const;

    // Фабричные методы
    static Object make_num_obj(double num);
    static Object make_str_obj(const std::wstring& str);
    static Object make_bool_obj(bool boolean);
    static Object make_nil_obj();
    
    ObjType getType() const {
        return type;    
    }

    ValueType getValue() const {
        return value;
    }

    friend std::ostream& operator<<(std::ostream& out, const Object& object);

private:
    ObjType type;
    ValueType value;
};

#endif
