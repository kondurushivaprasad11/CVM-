#pragma once
#include "Token.h"
#include <vector>
#include <string>

class Lexer {
public:
    Lexer(std::string source);
    std::vector<Token> scanTokens();

private:
    std::string source;
    std::vector<Token> tokens;
    size_t start = 0;
    size_t current = 0;
    int line = 1;

    bool isAtEnd();
    void scanToken();
    char advance();
    bool match(char expected);
    char peek();
    char peekNext();
    void addToken(TokenType type);
    void addToken(TokenType type, std::string lexeme);
    
    void stringLiteral();
    void number();
    void identifier();
    
    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
};
