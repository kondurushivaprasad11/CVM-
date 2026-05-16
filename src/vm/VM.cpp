#include "VM.h"
#include <iostream>
#include <stdexcept>

VM::VM() : chunk(nullptr), ip(nullptr) {}

InterpretResult VM::interpret(Chunk* chunk) {
    this->chunk = chunk;
    this->ip = chunk->code.data();
    return run();
}

void VM::push(Value value) {
    stack.push_back(value);
}

Value VM::pop() {
    Value value = stack.back();
    stack.pop_back();
    return value;
}

Value VM::peek(int distance) {
    return stack[stack.size() - 1 - distance];
}

bool VM::isFalsey(Value value) {
    if (value.type == ValueType::BOOL) {
        return !value.boolVal;
    }
    if (value.type == ValueType::INT) {
        return value.intVal == 0;
    }
    return false;
}

uint8_t VM::readByte() {
    return *ip++;
}

Value VM::readConstant() {
    return chunk->constants[readByte()];
}

InterpretResult VM::run() {
    for (;;) {
        uint8_t instruction = readByte();
        switch (static_cast<Opcode>(instruction)) {
            case Opcode::OP_CONSTANT: {
                Value constant = readConstant();
                push(constant);
                break;
            }
            case Opcode::OP_ADD: {
                Value b = pop();
                Value a = pop();
                if (a.type == ValueType::INT && b.type == ValueType::INT) {
                    push(Value(a.intVal + b.intVal));
                } else if (a.type == ValueType::STRING && b.type == ValueType::STRING) {
                    push(Value(a.strVal + b.strVal));
                } else {
                    std::cerr << "Operands must be two numbers or two strings." << std::endl;
                    return InterpretResult::RUNTIME_ERROR;
                }
                break;
            }
            case Opcode::OP_SUBTRACT: {
                Value b = pop();
                Value a = pop();
                if (a.type == ValueType::INT && b.type == ValueType::INT) {
                    push(Value(a.intVal - b.intVal));
                } else {
                    std::cerr << "Operands must be numbers." << std::endl;
                    return InterpretResult::RUNTIME_ERROR;
                }
                break;
            }
            case Opcode::OP_MULTIPLY: {
                Value b = pop();
                Value a = pop();
                if (a.type == ValueType::INT && b.type == ValueType::INT) {
                    push(Value(a.intVal * b.intVal));
                } else {
                    std::cerr << "Operands must be numbers." << std::endl;
                    return InterpretResult::RUNTIME_ERROR;
                }
                break;
            }
            case Opcode::OP_DIVIDE: {
                Value b = pop();
                Value a = pop();
                if (a.type == ValueType::INT && b.type == ValueType::INT) {
                    int div = b.intVal;
                    if (div == 0) {
                        std::cerr << "Division by zero." << std::endl;
                        return InterpretResult::RUNTIME_ERROR;
                    }
                    push(Value(a.intVal / div));
                } else {
                    std::cerr << "Operands must be numbers." << std::endl;
                    return InterpretResult::RUNTIME_ERROR;
                }
                break;
            }
            case Opcode::OP_EQUAL: {
                Value b = pop();
                Value a = pop();
                if (a.type != b.type) {
                    push(Value(false));
                } else if (a.type == ValueType::INT) {
                    push(Value(a.intVal == b.intVal));
                } else if (a.type == ValueType::BOOL) {
                    push(Value(a.boolVal == b.boolVal));
                } else if (a.type == ValueType::STRING) {
                    push(Value(a.strVal == b.strVal));
                }
                break;
            }
            case Opcode::OP_LESS: {
                Value b = pop();
                Value a = pop();
                if (a.type == ValueType::INT && b.type == ValueType::INT) {
                    push(Value(a.intVal < b.intVal));
                } else {
                    std::cerr << "Operands must be numbers." << std::endl;
                    return InterpretResult::RUNTIME_ERROR;
                }
                break;
            }
            case Opcode::OP_GREATER: {
                Value b = pop();
                Value a = pop();
                if (a.type == ValueType::INT && b.type == ValueType::INT) {
                    push(Value(a.intVal > b.intVal));
                } else {
                    std::cerr << "Operands must be numbers." << std::endl;
                    return InterpretResult::RUNTIME_ERROR;
                }
                break;
            }
            case Opcode::OP_NOT: {
                push(Value(isFalsey(pop())));
                break;
            }
            case Opcode::OP_PRINT: {
                std::cout << valueToString(pop()) << std::endl;
                break;
            }
            case Opcode::OP_INPUT: {
                std::string input;
                std::cout << "> ";
                std::cin >> input;
                try {
                    push(Value(std::stoi(input)));
                } catch (...) {
                    if (input == "true") push(Value(true));
                    else if (input == "false") push(Value(false));
                    else push(Value(input));
                }
                break;
            }
            case Opcode::OP_POP: {
                pop();
                break;
            }
            case Opcode::OP_DEFINE_GLOBAL: {
                std::string name = readConstant().strVal;
                globals[name] = pop();
                break;
            }
            case Opcode::OP_GET_GLOBAL: {
                std::string name = readConstant().strVal;
                if (globals.find(name) == globals.end()) {
                    std::cerr << "Undefined variable '" << name << "'." << std::endl;
                    return InterpretResult::RUNTIME_ERROR;
                }
                push(globals[name]);
                break;
            }
            case Opcode::OP_SET_GLOBAL: {
                std::string name = readConstant().strVal;
                if (globals.find(name) == globals.end()) {
                    std::cerr << "Undefined variable '" << name << "'." << std::endl;
                    return InterpretResult::RUNTIME_ERROR;
                }
                globals[name] = peek(0);
                break;
            }
            case Opcode::OP_JUMP: {
                uint8_t offset1 = readByte();
                uint8_t offset2 = readByte();
                uint16_t offset = (offset1 << 8) | offset2;
                ip += offset;
                break;
            }
            case Opcode::OP_JUMP_IF_FALSE: {
                uint8_t offset1 = readByte();
                uint8_t offset2 = readByte();
                uint16_t offset = (offset1 << 8) | offset2;
                if (isFalsey(peek(0))) ip += offset;
                break;
            }
            case Opcode::OP_LOOP: {
                uint8_t offset1 = readByte();
                uint8_t offset2 = readByte();
                uint16_t offset = (offset1 << 8) | offset2;
                ip -= offset;
                break;
            }
            case Opcode::OP_HALT: {
                return InterpretResult::OK;
            }
        }
    }
}
