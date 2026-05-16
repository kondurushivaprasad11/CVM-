#include "Compiler.h"
#include <string>

Compiler::Compiler(Chunk* chunk) : chunk(chunk) {}

void Compiler::compile(const std::vector<std::unique_ptr<Stmt>>& statements) {
    for (const auto& stmt : statements) {
        if (stmt) {
            stmt->accept(this);
        }
    }
    emitByte(static_cast<uint8_t>(Opcode::OP_HALT));
}

void Compiler::emitByte(uint8_t byte) {
    chunk->write(byte, 1); 
}

void Compiler::emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

void Compiler::emitConstant(Value value) {
    emitBytes(static_cast<uint8_t>(Opcode::OP_CONSTANT), static_cast<uint8_t>(chunk->addConstant(value)));
}

int Compiler::emitJump(uint8_t instruction) {
    emitByte(instruction);
    emitByte(0xff);
    emitByte(0xff);
    return static_cast<int>(chunk->code.size()) - 2;
}

void Compiler::patchJump(int offset) {
    int jump = static_cast<int>(chunk->code.size()) - offset - 2;
    chunk->code[offset] = (jump >> 8) & 0xff;
    chunk->code[offset + 1] = jump & 0xff;
}

void Compiler::emitLoop(int loopStart) {
    emitByte(static_cast<uint8_t>(Opcode::OP_LOOP));
    int offset = static_cast<int>(chunk->code.size()) - loopStart + 2;
    emitByte((offset >> 8) & 0xff);
    emitByte(offset & 0xff);
}

// -- Expressions --

void Compiler::visitLiteralExpr(LiteralExpr* expr) {
    if (expr->value.type == TokenType::NUMBER) {
        emitConstant(std::stoi(expr->value.lexeme));
    } else if (expr->value.type == TokenType::TRUE_LITERAL) {
        emitConstant(true);
    } else if (expr->value.type == TokenType::FALSE_LITERAL) {
        emitConstant(false);
    } else if (expr->value.type == TokenType::STRING_LITERAL) {
        emitConstant(expr->value.lexeme);
    }
}

void Compiler::visitBinaryExpr(BinaryExpr* expr) {
    expr->left->accept(this);
    expr->right->accept(this);

    switch (expr->op.type) {
        case TokenType::PLUS: emitByte(static_cast<uint8_t>(Opcode::OP_ADD)); break;
        case TokenType::MINUS: emitByte(static_cast<uint8_t>(Opcode::OP_SUBTRACT)); break;
        case TokenType::STAR: emitByte(static_cast<uint8_t>(Opcode::OP_MULTIPLY)); break;
        case TokenType::SLASH: emitByte(static_cast<uint8_t>(Opcode::OP_DIVIDE)); break;
        case TokenType::EQUAL_EQUAL: emitByte(static_cast<uint8_t>(Opcode::OP_EQUAL)); break;
        case TokenType::BANG_EQUAL: emitBytes(static_cast<uint8_t>(Opcode::OP_EQUAL), static_cast<uint8_t>(Opcode::OP_NOT)); break;
        case TokenType::LESS: emitByte(static_cast<uint8_t>(Opcode::OP_LESS)); break;
        case TokenType::GREATER: emitByte(static_cast<uint8_t>(Opcode::OP_GREATER)); break;
        case TokenType::LESS_EQUAL: emitBytes(static_cast<uint8_t>(Opcode::OP_GREATER), static_cast<uint8_t>(Opcode::OP_NOT)); break; 
        case TokenType::GREATER_EQUAL: emitBytes(static_cast<uint8_t>(Opcode::OP_LESS), static_cast<uint8_t>(Opcode::OP_NOT)); break;
        default: break;
    }
}

void Compiler::visitVariableExpr(VariableExpr* expr) {
    uint8_t arg = static_cast<uint8_t>(chunk->addConstant(expr->name.lexeme));
    emitBytes(static_cast<uint8_t>(Opcode::OP_GET_GLOBAL), arg);
}

void Compiler::visitAssignExpr(AssignExpr* expr) {
    expr->value->accept(this);
    uint8_t arg = static_cast<uint8_t>(chunk->addConstant(expr->name.lexeme));
    emitBytes(static_cast<uint8_t>(Opcode::OP_SET_GLOBAL), arg);
}

void Compiler::visitInputExpr(InputExpr* /*expr*/) {
    emitByte(static_cast<uint8_t>(Opcode::OP_INPUT));
}

void Compiler::visitLogicalExpr(LogicalExpr* expr) {
    expr->left->accept(this);

    if (expr->op.type == TokenType::OR) {
        int elseJump = emitJump(static_cast<uint8_t>(Opcode::OP_JUMP_IF_FALSE));
        int endJump = emitJump(static_cast<uint8_t>(Opcode::OP_JUMP));

        patchJump(elseJump);
        emitByte(static_cast<uint8_t>(Opcode::OP_POP));

        expr->right->accept(this);
        patchJump(endJump);
    } else if (expr->op.type == TokenType::AND) {
        int endJump = emitJump(static_cast<uint8_t>(Opcode::OP_JUMP_IF_FALSE));

        emitByte(static_cast<uint8_t>(Opcode::OP_POP));
        expr->right->accept(this);

        patchJump(endJump);
    }
}

// -- Statements --

void Compiler::visitExpressionStmt(ExpressionStmt* stmt) {
    stmt->expression->accept(this);
    emitByte(static_cast<uint8_t>(Opcode::OP_POP));
}

void Compiler::visitPrintStmt(PrintStmt* stmt) {
    stmt->expression->accept(this);
    emitByte(static_cast<uint8_t>(Opcode::OP_PRINT));
}

void Compiler::visitVarDeclStmt(VarDeclStmt* stmt) {
    if (stmt->initializer) {
        stmt->initializer->accept(this);
    } else {
        emitConstant(0); 
    }
    uint8_t arg = static_cast<uint8_t>(chunk->addConstant(stmt->name.lexeme));
    emitBytes(static_cast<uint8_t>(Opcode::OP_DEFINE_GLOBAL), arg);
}

void Compiler::visitBlockStmt(BlockStmt* stmt) {
    for (const auto& statement : stmt->statements) {
        statement->accept(this);
    }
}

void Compiler::visitIfStmt(IfStmt* stmt) {
    stmt->condition->accept(this);

    int thenJump = emitJump(static_cast<uint8_t>(Opcode::OP_JUMP_IF_FALSE));
    emitByte(static_cast<uint8_t>(Opcode::OP_POP));

    stmt->thenBranch->accept(this);

    int elseJump = emitJump(static_cast<uint8_t>(Opcode::OP_JUMP));

    patchJump(thenJump);
    emitByte(static_cast<uint8_t>(Opcode::OP_POP));

    if (stmt->elseBranch) {
        stmt->elseBranch->accept(this);
    }

    patchJump(elseJump);
}

void Compiler::visitWhileStmt(WhileStmt* stmt) {
    int loopStart = static_cast<int>(chunk->code.size());

    stmt->condition->accept(this);

    int exitJump = emitJump(static_cast<uint8_t>(Opcode::OP_JUMP_IF_FALSE));
    emitByte(static_cast<uint8_t>(Opcode::OP_POP));

    stmt->body->accept(this);
    emitLoop(loopStart);

    patchJump(exitJump);
    emitByte(static_cast<uint8_t>(Opcode::OP_POP));
}
