# CVM++ (Custom Scripting Compiler)

CVM++ is a custom programming language and runtime environment built from scratch in C++. It includes a complete pipeline: Lexer, Recursive Descent Parser, Compiler (emitting custom bytecode), and a Stack-Based Virtual Machine to execute the compiled code.

## Features

- **Custom Lexer**: Tokenizes source code into meaningful symbols.
- **Recursive Descent Parser**: Constructs an Abstract Syntax Tree (AST) from the token stream.
- **Compiler**: Translates the AST into custom bytecode instructions.
- **Stack-Based Virtual Machine (VM)**: Executes the compiled bytecode.
- **Core Language Features**:
  - Arithmetic operations
  - Variable declarations and assignments
  - Control flow structures (e.g., `if` statements, `while` loops)

## Project Structure

```text
CVM++/
├── CMakeLists.txt      # CMake build configuration
├── include/            # Header files for Lexer, Parser, Compiler, VM, etc.
├── src/                # Implementation files
│   ├── main.cpp        # Entry point
│   ├── lexer/          # Lexer implementation
│   ├── parser/         # Parser implementation
│   ├── compiler/       # Compiler implementation
│   └── vm/             # Virtual Machine implementation
└── tests/              # Test scripts (e.g., test.cvm)
```

## Requirements

- C++17 compatible compiler (GCC, Clang, MSVC)
- CMake 3.10 or higher

## Build Instructions

This project uses CMake for its build system. To build the project, run the following commands from the root directory:

```bash
# Create a build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build the executable
cmake --build .
```

After building, the executable `cvm` (or `cvm.exe` on Windows) will be available in the `build` directory.

## Usage

To run a script using CVM++, pass the script file as an argument to the executable:

```bash
./cvm ../tests/test.cvm
```
