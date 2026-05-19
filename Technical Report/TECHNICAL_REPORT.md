# CVM++ Technical Report

## 1. Executive Summary

CVM++ is a custom-designed, general-purpose programming language and runtime environment developed entirely in C++17. It encompasses a complete compilation pipeline from source code to execution. The primary components of this ecosystem include a Lexical Analyzer (Lexer), a Recursive Descent Parser, an Abstract Syntax Tree (AST) to Bytecode Compiler, and a Stack-Based Virtual Machine (VM). The project aims to provide a fast, modular, and extensible platform for executing scripts, illustrating the fundamental mechanics of modern language design and implementation.

## 2. Architecture Overview

The system processes source code through a standard multi-stage pipeline:

1.  **Source Code (`.cvm`)**: The plain text input file written in the CVM++ custom syntax.
2.  **Lexical Analysis (Lexer)**: Transforms the raw source code into a stream of categorized `Token` objects.
3.  **Syntactic Analysis (Parser)**: Constructs an Abstract Syntax Tree (AST) to represent the hierarchical structure and precedence of expressions and statements.
4.  **Compilation (Compiler)**: Traverses the AST and emits custom bytecodes, packaged into `Chunk` structures along with associated constants.
5.  **Execution (Virtual Machine)**: Interprets and executes the compiled bytecode instructions using a stack-based memory model.

## 3. Component Deep Dive

### 3.1. Lexical Analyzer (Lexer)
The Lexer's role is to convert raw string inputs into a sequence of tokens (`Token.h`). It supports:
-   **Single-Character Tokens**: Punctuation and basic operators (e.g., `(`, `)`, `{`, `}`, `+`, `-`, `*`, `/`).
-   **Multi-Character Tokens**: Relational and logical operators (e.g., `==`, `!=`, `<=`, `>=`).
-   **Literals**: Numeric values (`NUMBER`), text strings (`STRING_LITERAL`), and boolean values.
-   **Identifiers and Keywords**: Distinguishes user-defined variables from built-in keywords (e.g., `LET`, `IF`, `ELSE`, `WHILE`, `PRINT`, `INPUT`).
-   **Error Handling**: Emits `ERROR_TOKEN` when invalid characters are encountered.

### 3.2. Abstract Syntax Tree (AST) & Parser
The Parser utilizes a recursive descent algorithm to parse the token stream and enforce language grammar rules.
-   **Grammar Support**: It supports variable declarations (`let x = 10;`), assignment, basic control flow (`if`/`else`, `while` loops), and complex expressions respecting standard operator precedence.
-   **AST Nodes (`AST.h`)**: Different node types represent various constructs, such as `BinaryExpr`, `UnaryExpr`, `LiteralExpr`, `VarDeclStmt`, `BlockStmt`, and `IfStmt`.
-   **AST Printer (`ASTPrinter.h`)**: Provides a utility to visualize the tree structure for debugging purposes.

### 3.3. Compiler
The Compiler translates the intermediate representation (AST) into lower-level operations.
-   **Bytecode Emission**: Translates AST nodes into sequential bytecodes (`Opcode.h`).
-   **Constants Management**: Constants (numbers, strings) are stored in a dedicated constant pool within a `Chunk`, and referenced by `OP_CONSTANT` instructions.
-   **Variable Resolution**: Handles the scoping, defining (`OP_DEFINE_GLOBAL`), retrieving (`OP_GET_GLOBAL`), and setting (`OP_SET_GLOBAL`) of global variables.
-   **Control Flow Translation**: Converts `if`/`else` and `while` loops into conditional jump instructions (`OP_JUMP_IF_FALSE`, `OP_JUMP`, `OP_LOOP`) coupled with precise offset calculations.

### 3.4. Bytecode & Opcodes
The system relies on a well-defined set of instructions (`Opcode.h`) which are executed by the VM. These instructions include:
-   **Stack Operations**: `OP_CONSTANT`, `OP_POP`
-   **Binary Operations**: `OP_ADD`, `OP_SUBTRACT`, `OP_MULTIPLY`, `OP_DIVIDE`
-   **Comparison & Logical Operations**: `OP_EQUAL`, `OP_LESS`, `OP_GREATER`, `OP_NOT`
-   **I/O Operations**: `OP_PRINT`, `OP_INPUT`
-   **Variable Handling**: `OP_DEFINE_GLOBAL`, `OP_GET_GLOBAL`, `OP_SET_GLOBAL`
-   **Control Flow**: `OP_JUMP`, `OP_JUMP_IF_FALSE`, `OP_LOOP`
-   **System**: `OP_HALT`

### 3.5. Stack-Based Virtual Machine (VM)
The Virtual Machine acts as the core runtime. It interprets the compiled `Chunk` structures.
-   **Execution Model**: It uses a Last-In, First-Out (LIFO) stack to evaluate expressions. For example, `OP_ADD` pops two operands from the stack, computes their sum, and pushes the result back.
-   **Type System (`Value.h`)**: The VM operates on a dynamically typed `Value` structure that can represent different types (numbers, booleans, and objects/strings) safely.
-   **Global State**: Maintains a global symbol table mapping identifier names to their corresponding `Value`.
-   **Debugging (`Debug.h`)**: Includes tools to disassemble bytecode instructions and trace stack states during execution.

## 4. Build System and Dependencies

The project relies on CMake to manage its build process, ensuring cross-platform compatibility (Windows, macOS, Linux).
-   **Language Standard**: Requires C++17 or later for features like `std::string_view` and `std::variant` (depending on implementation specifics).
-   **Directory Structure**: The project is strictly organized into `src` (implementations), `include` (headers), and `tests` (scripts).

## 5. Testing and Validation

A suite of `.cvm` test scripts (e.g., `tests/variables.cvm`, `tests/math_test.cvm`, `tests/control_flow.cvm`) exists to validate the end-to-end functionality of the lexer, parser, compiler, and VM. These integration tests confirm that the runtime behaviors match the defined specifications.

## 6. Future Enhancements

Potential areas for extending CVM++ include:
-   **Functions & Closures**: Adding support for user-defined functions, arguments, and return values, and lexical scoping/closures.
-   **Garbage Collection**: Implementing an automated memory management system to handle dynamically allocated objects like strings and instances.
-   **Data Structures**: Introducing built-in lists, arrays, or dictionary/map structures.
-   **Object-Oriented Features**: Adding classes, methods, and inheritance.
-   **Standard Library**: Expanding the standard I/O and providing string manipulation, math utilities, or file handling APIs.
