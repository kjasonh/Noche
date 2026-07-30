#pragma once

#include "lexer/token.h"
#include <vector>
#include <string>
#include <unordered_map>

class Lexer {
public:
    explicit Lexer(std::string source);

    // 소스 전체를 스캔해서 토큰 벡터 반환 (마지막은 항상 END_OF_FILE)
    std::vector<Token> scanTokens();

    // 스캔 중 발생한 에러 메시지들 (line, message)
    const std::vector<std::pair<int, std::string>>& getErrors() const { return errors_; }

private:
    std::string source_;
    size_t start_ = 0;      // 현재 토큰의 시작 위치
    size_t current_ = 0;    // 현재 읽고 있는 위치
    int line_ = 1;
    int column_ = 1;
    int tokenStartColumn_ = 1;

    std::vector<Token> tokens_;
    std::vector<std::pair<int, std::string>> errors_;

    static const std::unordered_map<std::string, TokenType> keywords_;

    void scanToken();

    // 문자 처리 헬퍼
    bool isAtEnd() const;
    char advance();
    char peek() const;
    char peekNext() const;
    bool match(char expected);

    // 토큰 생성 헬퍼
    void addToken(TokenType type);
    void addToken(TokenType type, LiteralValue literal);

    // 리터럴/식별자 스캔
    void scanString();
    void scanNumber();
    void scanIdentifierOrKeyword();

    // 공백/주석 스킵
    void skipWhitespaceAndComments();

    static bool isDigit(char c);
    static bool isAlpha(char c);
    static bool isAlphaNumeric(char c);

    void reportError(const std::string& message);
};
