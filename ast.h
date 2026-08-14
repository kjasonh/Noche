#pragma once

#include "lexer/token.h"
#include <memory>
#include <vector>
#include <string>

// ---- Expression Node ----

struct BinaryExpr; struct UnaryExpr; struct LiteralExpr; struct VarExpr;
struct AssignExpr; struct CallExpr; struct LogicalExpr; struct GroupingExpr;

struct ExprVisitor {
    virtual ~ExprVisitor() = default;
    virtual void visitBinaryExpr(BinaryExpr&) = 0;
    virtual void visitUnaryExpr(UnaryExpr&) = 0;
    virtual void visitLiteralExpr(LiteralExpr&) = 0;
    virtual void visitVarExpr(VarExpr&) = 0;
    virtual void visitAssignExpr(AssignExpr&) = 0;
    virtual void visitCallExpr(CallExpr&) = 0;
    virtual void visitLogicalExpr(LogicalExpr&) = 0;
    virtual void visitGroupingExpr(GroupingExpr&) = 0;
};

struct Expr {
    virtual ~Expr() = default;
    virtual void accept(ExprVisitor& v) = 0;
};

using ExprPtr = std::unique_ptr<Expr>;

// binary operator/expression
struct BinaryExpr : Expr {
    ExprPtr left;
    Token op;
    ExprPtr right;
    BinaryExpr(ExprPtr l, Token o, ExprPtr r)
        : left(std::move(l)), op(std::move(o)), right(std::move(r)) {}
    void accept(ExprVisitor& v) override { v.visitBinaryExpr(*this); }
};

// a && b, a || b -- This is also binary, but dividend for Short-cirsut evaluation
struct LogicalExpr : Expr {
    ExprPtr left;
    Token op;
    ExprPtr right;
    LogicalExpr(ExprPtr l, Token o, ExprPtr r)
        : left(std::move(l)), op(std::move(o)), right(std::move(r)) {}
    void accept(ExprVisitor& v) override { v.visitLogicalExpr(*this); }
};

// -a, !a
struct UnaryExpr : Expr {
    Token op;
    ExprPtr right;
    UnaryExpr(Token o, ExprPtr r) : op(std::move(o)), right(std::move(r)) {}
    void accept(ExprVisitor& v) override { v.visitUnaryExpr(*this); }
};

// 123, 3.14, "hi", true, false
struct LiteralExpr : Expr {
    LiteralValue value;
    explicit LiteralExpr(LiteralValue v) : value(std::move(v)) {}
    void accept(ExprVisitor& v) override { v.visitLiteralExpr(*this); }
};

// referencing a variable (e.g. x)
struct VarExpr : Expr {
    Token name;
    explicit VarExpr(Token n) : name(std::move(n)) {}
    void accept(ExprVisitor& v) override { v.visitVarExpr(*this); }
};

// assigning to a variable (e.g. x = expr)
struct AssignExpr : Expr {
    Token name;
    ExprPtr value;
    AssignExpr(Token n, ExprPtr v) : name(std::move(n)), value(std::move(v)) {}
    void accept(ExprVisitor& v) override { v.visitAssignExpr(*this); }
};

// only Top-level function, so just a function name is enough to identify it along functions.
struct CallExpr : Expr {
    Token callee;
    std::vector<ExprPtr> arguments;
    CallExpr(Token c, std::vector<ExprPtr> args)
        : callee(std::move(c)), arguments(std::move(args)) {}
    void accept(ExprVisitor& v) override { v.visitCallExpr(*this); }
};

// (expr) — expression sandwiched with round brackets
struct GroupingExpr : Expr {
    ExprPtr expression;
    explicit GroupingExpr(ExprPtr e) : expression(std::move(e)) {}
    void accept(ExprVisitor& v) override { v.visitGroupingExpr(*this); }
};

// ---- Type annotation ----

// present means if the type annotation can be skipped or not
// if present=false, then it can be skipped(dynamic).
// if true, then it is essential. it means static.
struct TypeAnnotation {
    bool present = false;
    TokenType type = TokenType::TYPE_INT;
};

// ============ Statement Node ============

struct ExprStmt; struct VarDeclStmt; struct BlockStmt; struct IfStmt;
struct WhileStmt; struct FuncDeclStmt; struct ReturnStmt;

struct StmtVisitor {
    virtual ~StmtVisitor() = default;
    virtual void visitExprStmt(ExprStmt&) = 0;
    virtual void visitVarDeclStmt(VarDeclStmt&) = 0;
    virtual void visitBlockStmt(BlockStmt&) = 0;
    virtual void visitIfStmt(IfStmt&) = 0;
    virtual void visitWhileStmt(WhileStmt&) = 0;
    virtual void visitFuncDeclStmt(FuncDeclStmt&) = 0;
    virtual void visitReturnStmt(ReturnStmt&) = 0;
};

struct Stmt {
    virtual ~Stmt() = default;
    virtual void accept(StmtVisitor& v) = 0;
};

using StmtPtr = std::unique_ptr<Stmt>;

// only function call (e.g. expr;)
struct ExprStmt : Stmt {
    ExprPtr expression;
    explicit ExprStmt(ExprPtr e) : expression(std::move(e)) {}
    void accept(StmtVisitor& v) override { v.visitExprStmt(*this); }
};

// variable declaration (e.g. let x: int = 5;)
struct VarDeclStmt : Stmt {
    Token name;
    TypeAnnotation type;
    ExprPtr initializer;
    VarDeclStmt(Token n, TypeAnnotation t, ExprPtr init)
        : name(std::move(n)), type(t), initializer(std::move(init)) {}
    void accept(StmtVisitor& v) override { v.visitVarDeclStmt(*this); }
};

// block (e.g. { ... })
struct BlockStmt : Stmt {
    std::vector<StmtPtr> statements;
    explicit BlockStmt(std::vector<StmtPtr> stmts) : statements(std::move(stmts)) {}
    void accept(StmtVisitor& v) override { v.visitBlockStmt(*this); }
};

// conditional statement (e.g. if cond { ... } else { ... })
struct IfStmt : Stmt {
    Token keyword; // this can show 'line number' where the error happen 
    ExprPtr condition;
    StmtPtr thenBranch;
    StmtPtr elseBranch; // nullptr is okay
    IfStmt(Token kw, ExprPtr c, StmtPtr t, StmtPtr e)
        : keyword(std::move(kw)), condition(std::move(c)), thenBranch(std::move(t)), elseBranch(std::move(e)) {}
    void accept(StmtVisitor& v) override { v.visitIfStmt(*this); }
};

// while loop (e.g. while cond { ... })
struct WhileStmt : Stmt {
    Token keyword;
    ExprPtr condition;
    StmtPtr body;
    WhileStmt(Token kw, ExprPtr c, StmtPtr b) : keyword(std::move(kw)), condition(std::move(c)), body(std::move(b)) {}
    void accept(StmtVisitor& v) override { v.visitWhileStmt(*this); }
};

struct Param {
    Token name;
    TypeAnnotation type;
};

// fn name(params) -> type { ... }  (only for top-level, not first-class function)
struct FuncDeclStmt : Stmt {
    Token name;
    std::vector<Param> params;
    TypeAnnotation returnType;
    StmtPtr body; // always BlockStmt
    FuncDeclStmt(Token n, std::vector<Param> p, TypeAnnotation rt, StmtPtr b)
        : name(std::move(n)), params(std::move(p)), returnType(rt), body(std::move(b)) {}
    void accept(StmtVisitor& v) override { v.visitFuncDeclStmt(*this); }
};

// return expr?;
struct ReturnStmt : Stmt {
    Token keyword;
    ExprPtr value; // nullptr is okay (return;)
    ReturnStmt(Token k, ExprPtr v) : keyword(std::move(k)), value(std::move(v)) {}
    void accept(StmtVisitor& v) override { v.visitReturnStmt(*this); }
};
