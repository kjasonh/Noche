#pragma once

#include "lexer/token.h"
#include <vector>
#include <string>
#include <unordered_map>

class Lexer {
public:
    explicit Lexer(std::string source);

    // scan source and return vector of tokens (last token should be EOF(END_OF_FILE))
    std::vector<Token> scanTokens();

    // getting vector of errors
    const std::vector<std::pair<int, std::string>>& getErrors() const { return errors_; }

/**
 * source_           -- source code
 * start             -- start position number
 * current_          -- current position number
 * line_             -- current line count
 * column_           -- current column count
 * tokenStartColumn_ -- start position of specific token on current line
 * 
 * tokens_           -- vector of tokens
 * errors_           -- vector of errors
 * keywords_         -- unordered map mapping string to type
 */
private:
    std::string source_;
    size_t start_ = 0;
    size_t current_ = 0;
    int line_ = 1;
    int column_ = 1;
    int tokenStartColumn_ = 1;

    std::vector<Token> tokens_;
    std::vector<std::pair<int, std::string>> errors_;

    static const std::unordered_map<std::string, TokenType> keywords_;

    void scanToken();

    // Symbol processing
    bool isAtEnd() const;
    char advance();
    char peek() const;
    char peekNext() const;
    bool match(char expected);

    // Token generation
    void addToken(TokenType type);
    void addToken(TokenType type, LiteralValue literal);

    // Scanning Literals and Keywords
    void scanString();
    void scanNumber();
    void scanIdentifierOrKeyword();

    // Skipping whitespaces and Comments
    void skipWhitespaceAndComments();

    // Lexeme classfication
    static bool isDigit(char c);
    static bool isAlpha(char c);
    static bool isAlphaNumeric(char c);

    void reportError(const std::string& message);
};
