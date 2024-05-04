#pragma once
#include <string>
#include <vector>

enum TokenType {
    TOKEN_CREATE, TOKEN_INSERT, TOKEN_SELECT, TOKEN_UPDATE, TOKEN_DELETE, TOKEN_DROP,
    TOKEN_TABLE, TOKEN_SET, TOKEN_INTO, TOKEN_WHERE, TOKEN_FROM, TOKEN_IN, TOKEN_ALL,
    TOKEN_AND, TOKEN_OR, TOKEN_NOT,
    TOKEN_INT, TOKEN_CHAR,
    TOKEN_COMMA,
    TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_EQ, TOKEN_LSS, TOKEN_GTR,
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_TIMES, TOKEN_SLASH,
    TOKEN_LEQ, TOKEN_NEQ, TOKEN_GEQ,
    TOKEN_NUM, TOKEN_STRING, TOKEN_IDENTIFIER
};

const auto TLex = std::vector<std::string> {
    "CREATE", "INSERT", "SELECT", "UPDATE", "DELETE", "DROP",
    "TABLE", "SET", "INTO", "WHERE", "FROM", "IN", "ALL",
    "AND", "OR", "NOT",
    "INT", "CHAR",
    ",",
    "(", ")", "=", "<", ">",
    "+", "-", "*", "/",
    "<=", "!=", ">="
};

struct Token {
    TokenType type;
    std::string value;
    auto print() -> void;
};

auto lexer(std::string line) -> std::vector<Token>;