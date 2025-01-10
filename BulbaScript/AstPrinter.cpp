#include "AstPrinter.hpp"

std::wstring AstPrinter::print(const std::shared_ptr<Expr>& expr) {
    if (!expr) {
        return L"nil";
    }
    return expr->accept(*this).toString();
}

Object AstPrinter::visit(const Binary& expr) {
    return parenthesize(expr.opr.lexeme, { expr.left, expr.right });
}

Object AstPrinter::visit(const Assign& expr) {
    return parenthesize(expr.name.lexeme, { expr.value });
}

Object AstPrinter::visit(const Call& expr)
{
    return parenthesize(expr.paren.lexeme, { expr.arguments });
}

Object AstPrinter::visit(const Variable& expr) {
    return parenthesize(expr.name.lexeme, {});
}

Object AstPrinter::visit(const Grouping& expr) {
    return parenthesize(L"group", { expr.expression });
}

Object AstPrinter::visit(const Literal& expr) {
    if (expr.value.getType() == ObjType::Nil) {
        return Object::make_str_obj(L"nil");
    }
    return expr.value;
}

Object AstPrinter::visit(const Unary& expr) {
    return parenthesize(expr.opr.lexeme, { expr.right });
}

Object AstPrinter::parenthesize(const std::wstring& name, const std::vector<std::shared_ptr<Expr>>& exprs) {
    std::wstring result = L"(" + name;
    for (const auto& expr : exprs) {
        if (expr) {
            result += L" " + expr->accept(*this).toString();
        }
        else {
            result += L" nil";
        }
    }
    result += L")";
    return Object::make_str_obj(result);
}
