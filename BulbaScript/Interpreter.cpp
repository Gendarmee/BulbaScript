#include "Interpreter.hpp"

void Interpritator::interpret(std::vector<std::shared_ptr<Stmt>> statements) {
    try {
        for (const auto& statement : statements) {
            if (!statement) {
                throw std::runtime_error("Nullptr found in statements vector.");
            }
            execute(statement);
        }
    }
    catch (const RuntimeError& error) {
        BS::runtimeError(error);
    }
}

void Interpritator::execute(const std::shared_ptr<Stmt>& stmt) {
    if (!stmt) {
        throw std::runtime_error("Attempted to execute a nullptr statement.");
    }
    stmt->accept(*this);
}

Object Interpritator::evaluate(const std::shared_ptr<Expr>& expr) {
    return expr->accept(*this);
}

std::wstring Interpritator::stringify(const Object& object) {
    if (object.getType() == ObjType::Nil) return L"nil";

    if (object.getType() == ObjType::Number) {
        std::wstring text = object.toString();
        const std::wstring suffix = L".0";
        if (text.length() >= suffix.length() &&
            text.compare(text.length() - suffix.length(), suffix.length(), suffix) == 0) {
            text.erase(text.length() - suffix.length());
        }
        return text;
    }

    return object.toString();
}

Object Interpritator::visit(const Expression& stmt) {
    evaluate(stmt.expression);
    return Object::make_nil_obj();
}

Object Interpritator::visit(const Var& stmt) {
    Object value = Object::make_nil_obj();
    if (stmt.initializer != nullptr) {
        value = evaluate(stmt.initializer);
    }
    environment->define(stmt.name.lexeme, value);
    return Object::make_nil_obj();
}

Object Interpritator::visit(const Block& stmt) {
    executeBlock(stmt.statements, std::make_shared<Environment>(environment));
    return Object::make_nil_obj();
}

Object Interpritator::visit(const Print& stmt) {
    Object value = evaluate(stmt.expression);
    std::wcout << stringify(value) << std::endl;
    return Object::make_nil_obj();
}

Object Interpritator::visit(const Variable& expr) {
    return environment->get(expr.name);
}

Object Interpritator::visit(const Literal& literalExpr) {
    return literalExpr.value;
}

Object Interpritator::visit(const Grouping& groupingExpr) {
    return evaluate(groupingExpr.expression);
}

Object Interpritator::visit(const Unary& unaryExpr) {
    Object right = evaluate(unaryExpr.right);
    switch (unaryExpr.opr.type) {
    case TokenType::MINUS:
        checkNumberOperand(unaryExpr.opr, right);
        return Object::make_num_obj(-std::get<double>(right.getValue()));
    case TokenType::BANG:
        return Object::make_bool_obj(!isTruthy(right));
    default:
        break;
    }
    return Object::make_nil_obj();
}

Object Interpritator::visit(const Binary& binaryExpr) {
    Object left = evaluate(binaryExpr.left);
    Object right = evaluate(binaryExpr.right);

    switch (binaryExpr.opr.type) {
    case TokenType::BANG_EQUAL:
        return Object::make_bool_obj(!isEqual(left, right));
    case TokenType::EQUAL_EQUAL:
        return Object::make_bool_obj(isEqual(left, right));
    case TokenType::GREATER:
        checkNumberOperands(binaryExpr.opr, left, right);
        return Object::make_bool_obj(std::get<double>(left.getValue()) > std::get<double>(right.getValue()));
    case TokenType::GREATER_EQUAL:
        checkNumberOperands(binaryExpr.opr, left, right);
        return Object::make_bool_obj(std::get<double>(left.getValue()) >= std::get<double>(right.getValue()));
    case TokenType::LESS:
        checkNumberOperands(binaryExpr.opr, left, right);
        return Object::make_bool_obj(std::get<double>(left.getValue()) < std::get<double>(right.getValue()));
    case TokenType::LESS_EQUAL:
        checkNumberOperands(binaryExpr.opr, left, right);
        return Object::make_bool_obj(std::get<double>(left.getValue()) <= std::get<double>(right.getValue()));
    case TokenType::MINUS:
        checkNumberOperands(binaryExpr.opr, left, right);
        return Object::make_num_obj(std::get<double>(left.getValue()) - std::get<double>(right.getValue()));
    case TokenType::PLUS:
        if (left.getType() == ObjType::Number && right.getType() == ObjType::Number) {
            return Object::make_num_obj(std::get<double>(left.getValue()) + std::get<double>(right.getValue()));
        }
        if (left.getType() == ObjType::String || right.getType() == ObjType::String) {
            return Object::make_str_obj(left.toString() + right.toString());
        }
        throw RuntimeError(binaryExpr.opr, L"Operands must be two numbers or at least one string.");
    case TokenType::SLASH:
        checkNumberOperands(binaryExpr.opr, left, right);
        if (std::get<double>(right.getValue()) == 0) {
            throw RuntimeError(binaryExpr.opr, L"Division by zero.");
        }
        return Object::make_num_obj(std::get<double>(left.getValue()) / std::get<double>(right.getValue()));
    case TokenType::STAR:
        checkNumberOperands(binaryExpr.opr, left, right);
        return Object::make_num_obj(std::get<double>(left.getValue()) * std::get<double>(right.getValue()));
    default:
        break;
    }
    return Object::make_nil_obj();
}

void Interpritator::executeBlock(const std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment> newEnvironment)
{
    auto previous = environment;
    try {
        environment = newEnvironment;
        for (const auto& statement : statements) {
            execute(statement);
        }
    }
    catch (...) {
        environment = previous;
        throw;
    }
    environment = previous;
}

Object Interpritator::visit(const Assign& expr) {
    Object value = evaluate(expr.value);
    environment->assign(expr.name, value);
    return value;
}

void Interpritator::checkNumberOperand(const Token& opr, const Object& operand) {
    if (operand.getType() == ObjType::Number) return;
    throw RuntimeError(opr, L"Operand must be a number.");
}

void Interpritator::checkNumberOperands(const Token& opr, const Object& left, const Object& right) {
    if (left.getType() == ObjType::Number && right.getType() == ObjType::Number) return;
    throw RuntimeError(opr, L"Operands must be numbers.");
}

bool Interpritator::isEqual(const Object& a, const Object& b) {
    if (a.getType() == ObjType::Nil && b.getType() == ObjType::Nil) return true;
    if (a.getType() == ObjType::Nil) return false;

    return a.getValue() == b.getValue();
}

bool Interpritator::isTruthy(const Object& obj) {
    if (obj.getType() == ObjType::Nil) return false;
    if (obj.getType() == ObjType::Boolean) return std::get<bool>(obj.getValue());
    return true;
}
