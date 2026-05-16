#include "Debug.h"
#include <iostream>
#include <iomanip>

using namespace std;

void disassembleChunk(Chunk* chunk, const char* name) {
    cout << "\n== " << name << " ==" << endl;
    
    for (size_t offset = 0; offset < chunk->code.size(); ) {
        offset = disassembleInstruction(chunk, offset);
    }
    cout << "==================\n" << endl;
}

static int simpleInstruction(const char* name, int offset) {
    cout << name << endl;
    return offset + 1;
}

static int constantInstruction(const char* name, Chunk* chunk, int offset) {
    uint8_t constant = chunk->code[offset + 1];
    cout << left << setw(16) << name << " " 
         << right << setw(4) << (int)constant << " '";
    
    Value val = chunk->constants[constant];
    cout << valueToString(val) << "'" << endl;
    
    return offset + 2;
}

static int jumpInstruction(const char* name, int sign, Chunk* chunk, int offset) {
    uint16_t jump = (uint16_t)(chunk->code[offset + 1] << 8);
    jump |= chunk->code[offset + 2];
    
    cout << left << setw(16) << name << " " 
         << right << setw(4) << offset << " -> " 
         << offset + 3 + sign * jump << endl;
         
    return offset + 3;
}

int disassembleInstruction(Chunk* chunk, int offset) {
    cout << setfill('0') << setw(4) << offset << " " << setfill(' ');
    
    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
        cout << "   | ";
    } else {
        cout << setw(4) << chunk->lines[offset] << " ";
    }

    uint8_t instruction = chunk->code[offset];
    switch ((Opcode)instruction) {
        case Opcode::OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case Opcode::OP_ADD:
            return simpleInstruction("OP_ADD", offset);
        case Opcode::OP_SUBTRACT:
            return simpleInstruction("OP_SUBTRACT", offset);
        case Opcode::OP_MULTIPLY:
            return simpleInstruction("OP_MULTIPLY", offset);
        case Opcode::OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE", offset);
        case Opcode::OP_EQUAL:
            return simpleInstruction("OP_EQUAL", offset);
        case Opcode::OP_LESS:
            return simpleInstruction("OP_LESS", offset);
        case Opcode::OP_GREATER:
            return simpleInstruction("OP_GREATER", offset);
        case Opcode::OP_NOT:
            return simpleInstruction("OP_NOT", offset);
        case Opcode::OP_PRINT:
            return simpleInstruction("OP_PRINT", offset);
        case Opcode::OP_INPUT:
            return simpleInstruction("OP_INPUT", offset);
        case Opcode::OP_POP:
            return simpleInstruction("OP_POP", offset);
        case Opcode::OP_DEFINE_GLOBAL:
            return constantInstruction("OP_DEFINE_GLOBAL", chunk, offset);
        case Opcode::OP_GET_GLOBAL:
            return constantInstruction("OP_GET_GLOBAL", chunk, offset);
        case Opcode::OP_SET_GLOBAL:
            return constantInstruction("OP_SET_GLOBAL", chunk, offset);
        case Opcode::OP_JUMP:
            return jumpInstruction("OP_JUMP", 1, chunk, offset);
        case Opcode::OP_JUMP_IF_FALSE:
            return jumpInstruction("OP_JUMP_IF_FALSE", 1, chunk, offset);
        case Opcode::OP_LOOP:
            return jumpInstruction("OP_LOOP", -1, chunk, offset);
        case Opcode::OP_HALT:
            return simpleInstruction("OP_HALT", offset);
        default:
            cout << "Unknown opcode " << (int)instruction << endl;
            return offset + 1;
    }
}
