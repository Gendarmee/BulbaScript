#include "Object.hpp"

Object::Object() : str(L""), num(0.0), boolean(false), nil(nullptr), type(Object_nil) {}

Object::Object(ObjType type) : str(L""), num(0.0), boolean(false), nil(nullptr), type(type) {}

std::wstring Object::toString() {
    switch (type) {
    case Object_nil:
        return L"nil";
    case Object_bool:
        return boolean ? L"true" : L"false";
    case Object_str:
        return str;
    case Object_fun:
        return L"func: ";
    //case Object_class:
    //    return lox_class->toString();
    //case Object_instance:
    //    return instance->toString();
    case Object_num:
        return std::to_wstring(num);
    default:
        return L"<unknown>";
    }
}

Object Object::make_num_obj(double num) {
    Object num_obj(Object_num);
    num_obj.num = num;
    return num_obj;
}

Object Object::make_str_obj(std::wstring str) {
    Object str_obj(Object_str);
    str_obj.str = str;
    return str_obj;
}

Object Object::make_bool_obj(bool boolean) {
    Object bool_obj(Object_bool);
    bool_obj.boolean = boolean;
    return bool_obj;
}

Object Object::make_nil_obj() {
    return Object(Object_nil);
}

//// Реализация оператора вывода для std::ostream
//std::ostream& operator<<(std::ostream& out, const Object& object) {
//    std::wstring result = object.toString();
//    out << std::string(result.begin(), result.end());  // Конвертируем из wstring в string для вывода в std::ostream
//    return out;
//}