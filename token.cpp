#include "lexer/token.h"

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::LITERAL_INT: return "LITERAL_INT";
        case TokenType::LITERAL_FLOAT: return "LITERAL_FLOAT";
        case TokenType::LITERAL_STRING: return "LITERAL_STRING";
        case TokenType::IDENT: return "IDENT";

        case TokenType::LET: return "LET";
        case TokenType::FN: return "FN";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::WHILE: return "WHILE";
        case TokenType::RETURN: return "RETURN";
        case TokenType::TRUE_LIT: return "TRUE";
        case TokenType::FALSE_LIT: return "FALSE";
        case TokenType::TYPE_INT: return "TYPE_INT";
        case TokenType::TYPE_FLOAT: return "TYPE_FLOAT";
        case TokenType::TYPE_BOOL: return "TYPE_BOOL";
        case TokenType::TYPE_STRING: return "TYPE_STRING";

        /*Arithmetic Operator*/
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::STAR: return "STAR";
        case TokenType::SLASH: return "SLASH";
        case TokenType::PERCENT: return "PERCENT";

        /*Relational Operator*/
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
        case TokenType::BANG: return "BANG";
        case TokenType::BANG_EQUAL: return "BANG_EQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::GREATER: return "GREATER";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";

        /*Logical Operator*/
        case TokenType::AND_AND: return "AND_AND";
        case TokenType::OR_OR: return "OR_OR";

        /*punctuatioln, delimeter*/
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::COMMA: return "COMMA";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COLON: return "COLON";
        case TokenType::ARROW: return "ARROW";

        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::ERROR: return "ERROR";
    }
    return "UNKNOWN";
}
