#include "BSFunction.hpp"
#include "ReturnError.hpp"
#include "Interpritator.hpp"
#include "Stmt.hpp" // Здесь должен быть класс Function

Object BSFunction::call(Interpritator& interpreter, vector<Object> arguments)
{
    shared_ptr<Environment> environment(closure);

    for (int i = 0; i < declaration->params.size(); i++) {
        environment->define(declaration->params.at(i).lexeme,arguments.at(i));
    }
    try {
        interpreter.executeBlock(declaration->body, environment);
    }
    catch (ReturnError returnValue) {
        return returnValue.value;
    }
    return Object::make_nil_obj();
}



int BSFunction::arity() const
{
    return declaration->params.size();;
}

wstring BSFunction::toString() const
{
    return L"<fn " + declaration->name.lexeme + L">";
}
