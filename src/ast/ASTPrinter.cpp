#include "ASTPrinter.h"
#include <iostream>

void ASTPrinter::print(const std::vector<std::unique_ptr<Stmt>>& statements) {
    std::cout << "--- Abstract Syntax Tree ---" << std::endl;
    for (const auto& stmt : statements) {
        stmt->accept(this);
    }
    std::cout << "----------------------------" << std::endl;
}

void ASTPrinter::printIndent() {
    for (int i = 0; i < indentLevel; i++) {
        std::cout << "  ";
    }
}

// --- Expressions ---

void ASTPrinter::visitLiteralExpr(LiteralExpr* expr) {
    std::cout << expr->value.lexeme;
}

void ASTPrinter::visitBinaryExpr(BinaryExpr* expr) {
    std::cout << "(" << expr->op.lexeme << " ";
    expr->left->accept(this);
    std::cout << " ";
    expr->right->accept(this);
    std::cout << ")";
}

void ASTPrinter::visitVariableExpr(VariableExpr* expr) {
    std::cout << expr->name.lexeme;
}

void ASTPrinter::visitAssignExpr(AssignExpr* expr) {
    std::cout << "(assign " << expr->name.lexeme << " ";
    expr->value->accept(this);
    std::cout << ")";
}

void ASTPrinter::visitInputExpr(InputExpr* expr) {
    std::cout << "(input)";
}

void ASTPrinter::visitLogicalExpr(LogicalExpr* expr) {
    std::cout << "(" << expr->op.lexeme << " ";
    expr->left->accept(this);
    std::cout << " ";
    expr->right->accept(this);
    std::cout << ")";
}

// --- Statements ---

void ASTPrinter::visitExpressionStmt(ExpressionStmt* stmt) {
    printIndent();
    std::cout << "ExprStmt: ";
    stmt->expression->accept(this);
    std::cout << std::endl;
}

void ASTPrinter::visitPrintStmt(PrintStmt* stmt) {
    printIndent();
    std::cout << "PrintStmt: ";
    stmt->expression->accept(this);
    std::cout << std::endl;
}

void ASTPrinter::visitVarDeclStmt(VarDeclStmt* stmt) {
    printIndent();
    std::cout << "VarDeclStmt: " << stmt->name.lexeme;
    if (stmt->initializer) {
        std::cout << " = ";
        stmt->initializer->accept(this);
    }
    std::cout << std::endl;
}

void ASTPrinter::visitBlockStmt(BlockStmt* stmt) {
    printIndent();
    std::cout << "BlockStmt {" << std::endl;
    indentLevel++;
    for (const auto& s : stmt->statements) {
        s->accept(this);
    }
    indentLevel--;
    printIndent();
    std::cout << "}" << std::endl;
}

void ASTPrinter::visitIfStmt(IfStmt* stmt) {
    printIndent();
    std::cout << "IfStmt: ";
    stmt->condition->accept(this);
    std::cout << std::endl;
    
    indentLevel++;
    stmt->thenBranch->accept(this);
    if (stmt->elseBranch) {
        printIndent();
        std::cout << "Else:" << std::endl;
        stmt->elseBranch->accept(this);
    }
    indentLevel--;
}

void ASTPrinter::visitWhileStmt(WhileStmt* stmt) {
    printIndent();
    std::cout << "WhileStmt: ";
    stmt->condition->accept(this);
    std::cout << std::endl;
    
    indentLevel++;
    stmt->body->accept(this);
    indentLevel--;
}
