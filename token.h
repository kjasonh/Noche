#pragma once

#include <string>
#include <variant>
#include <cstdint>

enum class TokenType {
    // 리터럴
    LITERAL_INT, LITERAL_FLOAT, LITERAL_STRING, IDENT,

    // 키워드
    LET, FN, IF, ELSE, WHILE, RETURN, TRUE_LIT, FALSE_LIT,
    TYPE_INT, TYPE_FLOAT, TYPE_BOOL, TYPE_STRING,

    // 연산자
    PLUS, MINUS, STAR, SLASH, PERCENT,
    EQUAL, EQUAL_EQUAL, BANG, BANG_EQUAL,
    LESS, LESS_EQUAL, GREATER, GREATER_EQUAL,
    AND_AND, OR_OR,

    // 구두점
    LPAREN, RPAREN, LBRACE, RBRACE,
    COMMA, SEMICOLON, COLON, ARROW,

    END_OF_FILE, ERROR
};

// 리터럴 값 저장용 (int/float/string/bool 중 하나이거나 없음)
using LiteralValue = std::variant<std::monostate, int64_t, double, std::string, bool>;

struct Token {
    TokenType type;
    std::string lexeme;      // 원본 텍스트 (예: "123", "myVar")
    LiteralValue literal;    // 파싱된 값 (숫자/문자열 리터럴일 때만 의미 있음)
    int line;
    int column;

    Token(TokenType type, std::string lexeme, LiteralValue literal, int line, int column)
        : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)),
          line(line), column(column) {}
};

// 디버깅용: 토큰 타입을 문자열로
std::string tokenTypeToString(TokenType type);
