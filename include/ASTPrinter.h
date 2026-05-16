#pragma once
#include "AST.h"
#include <string>

class ASTPrinter : public ExprVisitor, public StmtVisitor {
public:
    void print(const std::vector<std::unique_ptr<Stmt>>& statements);

    // Expressions
    void visitLiteralExpr(LiteralExpr* expr) override;
    void visitBinaryExpr(BinaryExpr* expr) override;
    void visitVariableExpr(VariableExpr* expr) override;
    void visitAssignExpr(AssignExpr* expr) override;
    void visitInputExpr(InputExpr* expr) override;
    void visitLogicalExpr(LogicalExpr* expr) override;

    // Statements
    void visitExpressionStmt(ExpressionStmt* stmt) override;
    void visitPrintStmt(PrintStmt* stmt) override;
    void visitVarDeclStmt(VarDeclStmt* stmt) override;
    void visitBlockStmt(BlockStmt* stmt) override;
    void visitIfStmt(IfStmt* stmt) override;
    void visitWhileStmt(WhileStmt* stmt) override;

private:
    int indentLevel = 0;
    void printIndent();
};
