#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include <string>
#include "Expr.hpp"
#include "Stmt.hpp"

class AstPrinter : public NSExpr::Visitor {
public:
    std::wstring print(const std::shared_ptr<Expr>& expr) {
        return expr->accept(*this).toString();
    }
   
    Object visit(const Binary& expr) override {
        return parenthesize(expr.opr.lexeme, { expr.left, expr.right });
    }

    Object visit(const Assign& expr) override {
        return parenthesize(expr.name.lexeme, {  });
    }

    Object visit(const Variable& expr) override {
        return parenthesize(expr.name.lexeme,{});
    }

    Object visit(const Grouping& expr) override {
        return parenthesize(L"group", { expr.expression });
    }

    Object visit(const Literal& expr) override {
        return expr.value;
    }

    Object visit(const Unary& expr) override {
        return parenthesize(expr.opr.lexeme, { expr.right });
    }

private:
    Object parenthesize(const std::wstring& name, const std::vector<std::shared_ptr<Expr>>& exprs) {
        std::wstring result = L"(" + name;
        for (const auto& expr : exprs) {
            result += L" " + expr->accept(*this).toString();
        }
        result += L")";
        return Object::make_str_obj(result);
    }
};

#endif
