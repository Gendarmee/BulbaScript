#include <stdexcept>
#include "Object.hpp"
using std::runtime_error;

class ReturnError : public runtime_error {
public:
    Object value;
    explicit ReturnError(Object value) : runtime_error(""), value(value) {};
};