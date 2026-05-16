#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

#include "Lexer.h"
#include "Parser.h"
#include "Compiler.h"
#include "VM.h"
#include "Chunk.h"
#include "ASTPrinter.h"
#include "Debug.h"

using namespace std;

bool debugMode = false;

// Function to run a string of code
void run(VM& vm, string source) {
    Lexer lexer(source);
    vector<Token> tokens = lexer.scanTokens();

    Parser parser(tokens);
    vector<unique_ptr<Stmt>> statements;
    
    try {
        statements = parser.parse();
    } catch (...) {
        return; 
    }

    if (debugMode) {
        ASTPrinter printer;
        printer.print(statements);
    }

    Chunk chunk;
    Compiler compiler(&chunk);
    compiler.compile(statements);

    if (debugMode) {
        disassembleChunk(&chunk, "Main Chunk");
    }

    vm.interpret(&chunk);
}

// Function to run code from a file
void runFile(string path) {
    ifstream file(path);
    if (!file.is_open()) {
        cout << "Cannot open script: " << path << endl;
        return;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string sourceCode = buffer.str();
    
    VM vm;
    run(vm, sourceCode);
}

// Interactive prompt (REPL)
void runPrompt() {
    string line;
    VM vm;
    
    cout << "---------------------------------" << endl;
    cout << " CVM++ Interpreter v1.0" << endl;
    cout << " Developed by Shiva Prasad" << endl;
    if (debugMode) cout << " [DEBUG MODE ENABLED]" << endl;
    cout << "---------------------------------" << endl;
    cout << "Type 'exit' to close the interpreter." << endl;

    while (true) {
        cout << "cvm> ";
        if (!getline(cin, line)) {
            break;
        }
        
        if (line == "exit") {
            break;
        }
        
        run(vm, line);
    }
}

int main(int argc, char* argv[]) {
    string path = "";
    
    // Parse arguments
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--debug") {
            debugMode = true;
        } else {
            path = arg;
        }
    }

    if (path == "") {
        runPrompt();
    } else {
        runFile(path);
    }
    
    return 0;
}
