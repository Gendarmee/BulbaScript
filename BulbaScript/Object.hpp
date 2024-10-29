#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <iostream>

typedef enum ObjType {
    Object_str, // string
    Object_num, // number
    Object_bool, // bool
    Object_nil, // null
    Object_fun, // функция
    Object_instance, // экземпляр класса
    Object_class // класс
};

class Object {
public:
    Object();
    Object(ObjType type);

    std::wstring str;
    double num;
    bool boolean;
    int* nil;
    ObjType type;

    std::wstring toString();

    static Object make_num_obj(double num);
    static Object make_str_obj(std::wstring str);
    static Object make_bool_obj(bool boolean);
    static Object make_nil_obj();

    //static Object make_fun_obj(shared_ptr<LoxCallable> function_);
    //static Object make_instance_obj(shared_ptr<LoxInstance> instance_);
    //static Object make_class_obj(shared_ptr<LoxClass> lox_class_);

    friend std::ostream& operator<<(std::ostream& out, const Object& object);
};

#endif