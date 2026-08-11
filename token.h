#pragma once

#include <string>
#include <variant>
#include <cstdint>

/**
 * Kinds of Token type
 */
enum class TokenType {
    // Literals
    LITERAL_INT, LITERAL_FLOAT, LITERAL_STRING, IDENT,

    // Keywords
    LET, FN, IF, ELSE, WHILE, RETURN, TRUE_LIT, FALSE_LIT,
    TYPE_INT, TYPE_FLOAT, TYPE_BOOL, TYPE_STRING,

    // Arithmetic OP
    PLUS, MINUS, STAR, SLASH, PERCENT,
    // Assignment OP
    EQUAL, 
    // Relational OP
    EQUAL_EQUAL, BANG, BANG_EQUAL,
    LESS, LESS_EQUAL, GREATER, GREATER_EQUAL,
    //Logical OP
    AND_AND, OR_OR,

    // Punctuation
    LPAREN, RPAREN, LBRACE, RBRACE,
    COMMA, SEMICOLON, COLON, ARROW,

    // EOF, Error
    END_OF_FILE, ERROR
};

// for saving Literals (along int/float/string/bool or nothing)
using LiteralValue = std::variant<std::monostate, int64_t, double, std::string, bool>;

struct Token {
    TokenType type;
    std::string lexeme;      // Lexeme, Raw Text (e.g. "123", "myVar")
    LiteralValue literal;    // parsed data (only meaningful when literal is number or string)
    int line;                // line
    int column;              // column

    Token(TokenType type, std::string lexeme, LiteralValue literal, int line, int column)
        : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)),
          line(line), column(column) {}
};

/**
 * change Token to String
 */
std::string tokenTypeToString(TokenType type);




