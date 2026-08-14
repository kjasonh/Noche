#pragma once

#include "lexer/token.h"
#include "parser/ast.h"
#include <vector>
#include <string>
#include <stdexcept>
#include <initializer_list>

//Runtime Error -- it can happen while parsing is in progress.
class ParseError : public std::runtime_error {
public:
    explicit ParseError(const std::string& msg) : std::runtime_error(msg) {}
};


class Parser {
public:
    explicit Parser(std::vector<Token> tokens);

    std::vector<StmtPtr> parse();

    const std::vector<std::string>& getErrors() const { return errors_; }

private:
    std::vector<Token> tokens_;
    size_t current_ = 0;
    std::vector<std::string> errors_;

    // ---- declaration ----
    // if topLevel=false, fn will be an error
    // Only the top-level function is supported. NO NESTED FUNCTION.
    StmtPtr declaration(bool topLevel);
    StmtPtr varDeclaration();
    StmtPtr funDeclaration();

    // ---- statement ----
    StmtPtr statement();
    StmtPtr ifStatement();
    StmtPtr whileStatement();
    StmtPtr returnStatement();
    StmtPtr exprStatement();
    StmtPtr block(); // cosume from '{' to '}' as a Block

    // ---- Type ----
    TypeAnnotation parseType();

    // ---- expression (prioritized) ----
    ExprPtr expression();
    ExprPtr assignment();
    ExprPtr orExpr();
    ExprPtr andExpr();
    ExprPtr equality();
    ExprPtr comparison();
    ExprPtr term();
    ExprPtr factor();
    ExprPtr unary();
    ExprPtr call();
    ExprPtr primary();

    // ---- Token stream helper ----
    bool match(std::initializer_list<TokenType> types);
    bool check(TokenType type) const;
    Token advance();
    bool isAtEnd() const;
    Token peek() const;
    Token previous() const;
    Token consume(TokenType type, const std::string& message);

    ParseError error(const Token& token, const std::string& message);
    void synchronize(); // skip error part and go next
};
