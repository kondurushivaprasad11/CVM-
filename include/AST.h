#pragma once
#include "Token.h"
#include <memory>
#include <vector>
#include <string>

// Forward declarations for visitors
struct LiteralExpr;
struct BinaryExpr;
struct VariableExpr;
struct AssignExpr;
struct InputExpr;
struct LogicalExpr;

struct ExpressionStmt;
struct PrintStmt;
struct VarDeclStmt;
struct BlockStmt;
struct IfStmt;
struct WhileStmt;

class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;
    virtual void visitLiteralExpr(LiteralExpr* expr) = 0;
    virtual void visitBinaryExpr(BinaryExpr* expr) = 0;
    virtual void visitVariableExpr(VariableExpr* expr) = 0;
    virtual void visitAssignExpr(AssignExpr* expr) = 0;
    virtual void visitInputExpr(InputExpr* expr) = 0;
    virtual void visitLogicalExpr(LogicalExpr* expr) = 0;
};

class StmtVisitor {
public:
    virtual ~StmtVisitor() = default;
    virtual void visitExpressionStmt(ExpressionStmt* stmt) = 0;
    virtual void visitPrintStmt(PrintStmt* stmt) = 0;
    virtual void visitVarDeclStmt(VarDeclStmt* stmt) = 0;
    virtual void visitBlockStmt(BlockStmt* stmt) = 0;
    virtual void visitIfStmt(IfStmt* stmt) = 0;
    virtual void visitWhileStmt(WhileStmt* stmt) = 0;
};

struct Expr {
    virtual ~Expr() = default;
    virtual void accept(ExprVisitor* visitor) = 0;
};

struct Stmt {
    virtual ~Stmt() = default;
    virtual void accept(StmtVisitor* visitor) = 0;
};

// --- Expressions ---

struct LiteralExpr : public Expr {
    Token value; // Number or true/false
    LiteralExpr(Token value) : value(std::move(value)) {}
    void accept(ExprVisitor* visitor) override { visitor->visitLiteralExpr(this); }
};

struct BinaryExpr : public Expr {
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
    BinaryExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}
    void accept(ExprVisitor* visitor) override { visitor->visitBinaryExpr(this); }
};

struct VariableExpr : public Expr {
    Token name;
    VariableExpr(Token name) : name(std::move(name)) {}
    void accept(ExprVisitor* visitor) override { visitor->visitVariableExpr(this); }
};

struct AssignExpr : public Expr {
    Token name;
    std::unique_ptr<Expr> value;
    AssignExpr(Token name, std::unique_ptr<Expr> value)
        : name(std::move(name)), value(std::move(value)) {}
    void accept(ExprVisitor* visitor) override { visitor->visitAssignExpr(this); }
};

struct InputExpr : public Expr {
    Token keyword;
    InputExpr(Token keyword) : keyword(std::move(keyword)) {}
    void accept(ExprVisitor* visitor) override { visitor->visitInputExpr(this); }
};

struct LogicalExpr : public Expr {
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
    LogicalExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}
    void accept(ExprVisitor* visitor) override { visitor->visitLogicalExpr(this); }
};

// --- Statements ---

struct ExpressionStmt : public Stmt {
    std::unique_ptr<Expr> expression;
    ExpressionStmt(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
    void accept(StmtVisitor* visitor) override { visitor->visitExpressionStmt(this); }
};

struct PrintStmt : public Stmt {
    std::unique_ptr<Expr> expression;
    PrintStmt(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
    void accept(StmtVisitor* visitor) override { visitor->visitPrintStmt(this); }
};

struct VarDeclStmt : public Stmt {
    Token name;
    std::unique_ptr<Expr> initializer;
    VarDeclStmt(Token name, std::unique_ptr<Expr> initializer)
        : name(std::move(name)), initializer(std::move(initializer)) {}
    void accept(StmtVisitor* visitor) override { visitor->visitVarDeclStmt(this); }
};

struct BlockStmt : public Stmt {
    std::vector<std::unique_ptr<Stmt>> statements;
    BlockStmt(std::vector<std::unique_ptr<Stmt>> statements) : statements(std::move(statements)) {}
    void accept(StmtVisitor* visitor) override { visitor->visitBlockStmt(this); }
};

struct IfStmt : public Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;
    IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}
    void accept(StmtVisitor* visitor) override { visitor->visitIfStmt(this); }
};

struct WhileStmt : public Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
        : condition(std::move(condition)), body(std::move(body)) {}
    void accept(StmtVisitor* visitor) override { visitor->visitWhileStmt(this); }
};
