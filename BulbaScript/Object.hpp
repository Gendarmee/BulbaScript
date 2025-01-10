#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <variant>
#include <iostream>
#include <memory>
#include <optional>
#include "BSCallable.hpp"

class BSCallable;

using std::shared_ptr, std::wstring, std::nullptr_t, std::variant;

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
    using ValueType = variant<wstring, double, bool,nullptr_t, shared_ptr<BSCallable>>;

    Object();
    explicit Object(ObjType type, ValueType value);

    wstring toString() const;

    static Object make_num_obj(double num);
    static Object make_str_obj(const wstring& str);
    static Object make_bool_obj(bool boolean);
    static Object make_fun_obj(shared_ptr<BSCallable> function);
    //static Object make_instance_obj(shared_ptr<LoxInstance> instance_);
    //static Object make_class_obj(shared_ptr<LoxClass> lox_class_);
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
