#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include <string>
#include <vector>
#include <memory>
#include "Expr.hpp"
#include "Stmt.hpp"
#include "Object.hpp"

class AstPrinter : public NSExpr::Visitor {
public:
    std::wstring print(const std::shared_ptr<Expr>& expr);

    Object visit(const Call& expr) override;
    Object visit(const Variable& expr) override;
    Object visit(const Binary& expr) override;
    Object visit(const Assign& expr) override;
    Object visit(const Grouping& expr) override;
    Object visit(const Literal& expr) override;
    Object visit(const Unary& expr) override;

private:
    Object parenthesize(const std::wstring& name, const std::vector<std::shared_ptr<Expr>>& exprs);
};

#endif