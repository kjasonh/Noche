#include "lexer/lexer.h"
#include <cctype>
#include <stdexcept>

const std::unordered_map<std::string, TokenType> Lexer::keywords_ = {
    {"let",    TokenType::LET},
    {"fn",     TokenType::FN},
    {"if",     TokenType::IF},
    {"else",   TokenType::ELSE},
    {"while",  TokenType::WHILE},
    {"return", TokenType::RETURN},
    {"true",   TokenType::TRUE_LIT},
    {"false",  TokenType::FALSE_LIT},
    {"int",    TokenType::TYPE_INT},
    {"float",  TokenType::TYPE_FLOAT},
    {"bool",   TokenType::TYPE_BOOL},
    {"string", TokenType::TYPE_STRING},
};

Lexer::Lexer(std::string source) : source_(std::move(source)) {}

std::vector<Token> Lexer::scanTokens() {
    while (!isAtEnd()) {
        skipWhitespaceAndComments();
        if (isAtEnd()) break;

        start_ = current_;
        tokenStartColumn_ = column_;
        scanToken();
    }

    tokens_.emplace_back(TokenType::END_OF_FILE, "", std::monostate{}, line_, column_);
    return tokens_;
}

void Lexer::skipWhitespaceAndComments() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r') {
            advance();
        } else if (c == '\n') {
            advance();
        } else if (c == '/' && peekNext() == '/') {
            // 한 줄 주석
            while (peek() != '\n' && !isAtEnd()) advance();
        } else if (c == '/' && peekNext() == '*') {
            // 블록 주석
            advance(); advance();
            while (!(peek() == '*' && peekNext() == '/') && !isAtEnd()) {
                advance();
            }
            if (isAtEnd()) {
                reportError("종료되지 않은 블록 주석");
                return;
            }
            advance(); advance(); // 닫는 "*/"
        } else {
            break;
        }
    }
}

void Lexer::scanToken() {
    char c = advance();

    switch (c) {
        case '(': addToken(TokenType::LPAREN); return;
        case ')': addToken(TokenType::RPAREN); return;
        case '{': addToken(TokenType::LBRACE); return;
        case '}': addToken(TokenType::RBRACE); return;
        case ',': addToken(TokenType::COMMA); return;
        case ';': addToken(TokenType::SEMICOLON); return;
        case ':': addToken(TokenType::COLON); return;

        case '+': addToken(TokenType::PLUS); return;
        case '*': addToken(TokenType::STAR); return;
        case '/': addToken(TokenType::SLASH); return;
        case '%': addToken(TokenType::PERCENT); return;

        case '-':
            addToken(match('>') ? TokenType::ARROW : TokenType::MINUS);
            return;

        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            return;
        case '!':
            addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            return;
        case '<':
            addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            return;
        case '>':
            addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            return;

        case '&':
            if (match('&')) { addToken(TokenType::AND_AND); return; }
            reportError("예상치 못한 문자 '&' (예: && 를 의도하셨나요?)");
            return;
        case '|':
            if (match('|')) { addToken(TokenType::OR_OR); return; }
            reportError("예상치 못한 문자 '|' (예: || 를 의도하셨나요?)");
            return;

        case '"':
            scanString();
            return;

        default:
            if (isDigit(c)) {
                scanNumber();
            } else if (isAlpha(c)) {
                scanIdentifierOrKeyword();
            } else {
                reportError(std::string("예상치 못한 문자: '") + c + "'");
            }
            return;
    }
}

void Lexer::scanString() {
    std::string value;
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            reportError("문자열이 줄바꿈 전에 닫히지 않음");
            return;
        }
        // 간단한 이스케이프 처리 (n, t, 쌍따옴표, 역슬래시)
        if (peek() == '\\' && !isAtEnd()) {
            advance();
            char esc = advance();
            switch (esc) {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case '"': value += '"'; break;
                case '\\': value += '\\'; break;
                default:
                    reportError(std::string("알 수 없는 이스케이프 시퀀스: \\") + esc);
            }
            continue;
        }
        value += advance();
    }

    if (isAtEnd()) {
        reportError("종료되지 않은 문자열 리터럴");
        return;
    }

    advance(); // 닫는 쌍따옴표
    addToken(TokenType::STRING_LITERAL, value);
}

void Lexer::scanNumber() {
    while (isDigit(peek())) advance();

    bool isFloat = false;
    if (peek() == '.' && isDigit(peekNext())) {
        isFloat = true;
        advance(); // '.'
        while (isDigit(peek())) advance();
    }

    std::string text = source_.substr(start_, current_ - start_);
    if (isFloat) {
        addToken(TokenType::FLOAT_LITERAL, std::stod(text));
    } else {
        addToken(TokenType::INT_LITERAL, static_cast<int64_t>(std::stoll(text)));
    }
}

void Lexer::scanIdentifierOrKeyword() {
    while (isAlphaNumeric(peek())) advance();

    std::string text = source_.substr(start_, current_ - start_);
    auto it = keywords_.find(text);
    if (it != keywords_.end()) {
        // true/false는 리터럴 값도 같이 채워줌
        if (it->second == TokenType::TRUE_LIT) {
            addToken(TokenType::TRUE_LIT, true);
        } else if (it->second == TokenType::FALSE_LIT) {
            addToken(TokenType::FALSE_LIT, false);
        } else {
            addToken(it->second);
        }
    } else {
        addToken(TokenType::IDENT);
    }
}

// ---- 문자 처리 헬퍼 ----

bool Lexer::isAtEnd() const {
    return current_ >= source_.size();
}

char Lexer::advance() {
    char c = source_[current_++];
    if (c == '\n') {
        line_++;
        column_ = 1;
    } else {
        column_++;
    }
    return c;
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source_[current_];
}

char Lexer::peekNext() const {
    if (current_ + 1 >= source_.size()) return '\0';
    return source_[current_ + 1];
}

bool Lexer::match(char expected) {
    if (isAtEnd() || source_[current_] != expected) return false;
    advance();
    return true;
}

// ---- 토큰 생성 ----

void Lexer::addToken(TokenType type) {
    addToken(type, std::monostate{});
}

void Lexer::addToken(TokenType type, LiteralValue literal) {
    std::string text = source_.substr(start_, current_ - start_);
    tokens_.emplace_back(type, text, std::move(literal), line_, tokenStartColumn_);
}

// ---- 문자 분류 ----

bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

void Lexer::reportError(const std::string& message) {
    errors_.emplace_back(line_, message);
}
