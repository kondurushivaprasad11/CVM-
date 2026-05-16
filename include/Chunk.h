#pragma once
#include "Opcode.h"
#include "Value.h"
#include <vector>
#include <cstdint>

struct Chunk {
    std::vector<uint8_t> code;
    std::vector<Value> constants;
    std::vector<int> lines;

    void write(uint8_t byte, int line) {
        code.push_back(byte);
        lines.push_back(line);
    }

    int addConstant(Value value) {
        constants.push_back(value);
        return constants.size() - 1;
    }
};
