#include "Object.hpp"

Object::Object() : type(ObjType::Nil), value(nullptr) {}

Object::Object(ObjType type, ValueType value) : type(type), value(std::move(value)) {}

std::wstring Object::toString() const {
    switch (type) {
    case ObjType::String: return std::get<std::wstring>(value);
    case ObjType::Number: return std::to_wstring(std::get<double>(value));
    case ObjType::Boolean: return std::get<bool>(value) ? L"true" : L"false";
    case ObjType::Nil: return L"nil";
    default: return L"<unsupported>";
    }
}

Object Object::make_num_obj(double num) {
    return Object(ObjType::Number, num);
}

Object Object::make_str_obj(const std::wstring& str) {
    return Object(ObjType::String, str);
}

Object Object::make_bool_obj(bool boolean) {
    return Object(ObjType::Boolean, boolean);
}

Object Object::make_fun_obj(shared_ptr<BSCallable> function)
{
    return Object(ObjType::Function, function);
}

Object Object::make_nil_obj() {
    return Object(ObjType::Nil, nullptr);
}

std::ostream& operator<<(std::ostream& out, const Object& object) {
    std::wstring ws = object.toString();
    std::string s(ws.begin(), ws.end());
    return out << s;
}
