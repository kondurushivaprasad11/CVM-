cmake_minimum_required(VERSION 3.10)

project(CVMPlusPlus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

add_executable(cvm
    src/main.cpp
    src/lexer/Lexer.cpp
    src/parser/Parser.cpp
    src/ast/ASTPrinter.cpp
    src/compiler/Compiler.cpp
    src/vm/VM.cpp
    src/vm/Debug.cpp
)