#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>

using String = std::string;

enum Type
{
    VAR,
    LITERAL,
    ADD,
    LPAREN,
    RPAREN,
    MULT,
    DIV,
    SUBT
};
class Token
{
    String value;
    Type type;

public:
    // Create the token:
    Token(String, enum Type);
    // Return the precedence value of the current type
    int getPrecedence();
    inline String &getValue()
    {
        return this->value;
    }
    inline Type &getType()
    {
        return this->type;
    }
    char getTypeChar();
    inline String toString()
    {
        return std::string(1, this->getTypeChar()) + " : " + this->getValue();
    }
    bool isBinaryOperation();
    bool isUnaryOperation();
    inline bool isParen()
    {
        return this->getType() == Type::RPAREN || this->getType() == Type::LPAREN;
    }
    inline bool isRightParen()
    {
        return this->type == Type::RPAREN;
    }
    inline bool isLeftParen()
    {
        return this->type == Type::LPAREN;
    }
    bool isHigherPrecedenceThan(Token);
    void setValue(String string);
};
std::vector<Token> tokenization(String);
std::vector<Token> infixtopostfix(std::vector<Token>);
double evaluate(String);
double eval(std::vector<Token>);
int print(std::vector<Token>);
inline bool isLetter(char pos)
{
    return (pos >= 'a' && pos <= 'z') || (pos >= 'A' && pos <= 'Z');
}
inline bool isDigit(char pos)
{
    return pos >= '0' && pos <= '9';
}
inline Token consume(char token, Type type)
{
    return Token{std::string(1, token), type};
}
int consumeVar(String, unsigned, std::vector<Token> &);
int consumeLit(String, unsigned, std::vector<Token> &);
#endif