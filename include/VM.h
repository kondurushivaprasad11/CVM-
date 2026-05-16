#pragma once
#include "Chunk.h"
#include "Value.h"
#include <vector>
#include <unordered_map>
#include <string>

enum class InterpretResult {
    OK,
    COMPILE_ERROR,
    RUNTIME_ERROR
};

class VM {
public:
    VM();
    InterpretResult interpret(Chunk* chunk);

private:
    Chunk* chunk;
    uint8_t* ip;
    std::vector<Value> stack;
    std::unordered_map<std::string, Value> globals;

    void push(Value value);
    Value pop();
    Value peek(int distance);
    bool isFalsey(Value value);
    
    InterpretResult run();
    uint8_t readByte();
    Value readConstant();
};
