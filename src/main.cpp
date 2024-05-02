#include "lexer.hpp"
#include "table.hpp"
#include <stack>
#include <cstdio>

const std::string dirPath = "/home/miomit/";

const std::string sqlCreate = "CREATE TABLE Students (Firstname CHAR(10), Surname CHAR(15), Age INT, Phone CHAR(9), SEX CHAR )";
const std::string sqlInsert = "INSERT INTO Students ( 'Yura', 'Movsesyan', 21, '+7(926) 33 80 800', 'M')";
const std::string sqlDrop = "DROP TABLE Students";

auto getCodeSQL (int id) -> std::string {
    switch (id) {
        case 0: return sqlCreate;
        case 1: return sqlInsert;
        case 2: return sqlDrop;
    }
}

auto precedence (Token token) -> int {
    switch (token.type) {
        case TOKEN_NOT:     return 6;
        case TOKEN_TIMES:
        case TOKEN_SLASH:   return 5;
        case TOKEN_PLUS:
        case TOKEN_MINUS:   return 4;
        case TOKEN_EQ:
        case TOKEN_LSS:
        case TOKEN_GTR:
        case TOKEN_LEQ:
        case TOKEN_NEQ:
        case TOKEN_GEQ:     return 3;
        case TOKEN_AND:     return 2;
        case TOKEN_OR:      return 1;
        default:            return 0;
    }
}

auto parse (std::vector<Token> tokens) -> std::vector<Token> {
    std::vector<Token> res;
    std::stack<Token> buff;

    for (auto& t : tokens) {
        if (t.type == TOKEN_LPAREN) {
            buff.push(t);
        } else if (t.type == TOKEN_RPAREN) {
            while (buff.top().type != TOKEN_LPAREN) {
                res.push_back(buff.top());
                buff.pop();
            }
            buff.pop();
        } else {
            if (precedence(t) == 0) {
                res.push_back(t);
            } else {
                while (!buff.empty()
                        && buff.top().type != TOKEN_LPAREN
                        && precedence(buff.top()) >= precedence(t)) {
                    res.push_back(buff.top());
                    buff.pop();
                }

                buff.push(t);
            }
        }
    }

    while (!buff.empty()) {
        res.push_back(buff.top());
        buff.pop();
    }

    return res;
}

auto main() -> int {
    int c = 0;
    std::cout << "[CREATE - 0, INSERT - 1, DROP - 2]\n" << "> ";

    std::cin >> c;

    auto tokens = lexer(getCodeSQL(c));

    // CREATE
    if (tokens[0].type == TOKEN_CREATE
        && tokens[1].type == TOKEN_TABLE
        && tokens[2].type == TOKEN_IDENTIFIER) {
            std::string tableName = tokens[2].value;
            std::vector<TableHeader> tableHeader;
            if (tokens[3].type == TOKEN_LPAREN
                && tokens.back().type == TOKEN_RPAREN) {
                for (auto i = 4; i < tokens.size() - 2; i++) {
                    if (tokens[i].type == TOKEN_COMMA) continue;
                    if (tokens[i].type == TOKEN_IDENTIFIER) {
                        if (tokens[i+1].type == TOKEN_INT) {
                            tableHeader.push_back(TableHeader::NUM(tokens[i].value));
                            i += 2;
                        } else if (tokens[i+1].type == TOKEN_CHAR) {
                            if (tokens[i+2].type == TOKEN_LPAREN) {
                                if (tokens[i+3].type == TOKEN_NUM
                                    && tokens[i+4].type == TOKEN_RPAREN) {
                                    tableHeader.push_back(
                                        TableHeader::TEXT(
                                            tokens[i].value,
                                            std::stoi(tokens[i+3].value)
                                        )
                                    );
                                    i += 5;
                                }
                            } else {
                                tableHeader.push_back(
                                    TableHeader::SYMBOL(
                                        tokens[i].value
                                    )
                                );
                                i += 2;
                            }
                        }
                    }
                }
            }

            table::create(
                dirPath + tableName + ".tsql",
                tableHeader
            );
        } // INSERT
        else if (tokens[0].type == TOKEN_INSERT
                && tokens[1].type == TOKEN_INTO
                && tokens[2].type == TOKEN_IDENTIFIER) {
            std::string tableName = tokens[2].value;
            std::vector<std::string> row;

            if (tokens[3].type == TOKEN_LPAREN
                && tokens.back().type == TOKEN_RPAREN) {
                for (auto i = 4; i < tokens.size() - 1; i++) {
                    if (tokens[i].type == TOKEN_COMMA) continue;
                    if (tokens[i].type == TOKEN_NUM
                        || tokens[i].type == TOKEN_STRING) {
                        row.push_back(tokens[i].value);
                    }
                }
            }

            auto db = table::open(dirPath + tableName + ".tsql");
            db.insert(row);
        } // DROP
        else if (tokens[0].type == TOKEN_DROP
                && tokens[1].type == TOKEN_TABLE
                && tokens[2].type == TOKEN_IDENTIFIER) {
            std::remove((dirPath + tokens[2].value + ".tsql").c_str());
        }
}
