#ifndef BS_CALLABLE_H
#define BS_CALLABLE_H

#include <vector>
#include <string>
#include "Object.hpp"
class Object;
class Interpritator;
using std::vector, std::wstring;
class BSCallable {
public:
    virtual ~BSCallable() = default;
    virtual Object call(Interpritator& interpreter, vector<Object> arguments) = 0;
    virtual int arity() const = 0;
    virtual wstring toString() const = 0;
};

#endif
