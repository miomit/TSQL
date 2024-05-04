#include <stack>
#include "sql_cmd.hpp"
#include "parser.hpp"

auto sqlExe(std::string cmd) -> bool {
    auto tokens = lexer(cmd);
    switch (tokens[0].type) {
        case TOKEN_CREATE:  return create(tokens);
        case TOKEN_INSERT:  return insert(tokens);
        case TOKEN_DROP:    return drop(tokens);
    }

    return false;
}

auto create(std::vector<Token> tokens) -> bool {
    if (tokens.size() < 6) return false;
    if (tokens[0].type == TOKEN_CREATE && tokens[1].type == TOKEN_TABLE && tokens[2].type == TOKEN_IDENTIFIER) {
        std::string tableName = tokens[2].value;
        std::vector<TableHeader> tableHeader;
        if (tokens[3].type == TOKEN_LPAREN && tokens.back().type == TOKEN_RPAREN) {
            for (auto i = 4; i < tokens.size() - 2; i++) {
                if (tokens[i].type == TOKEN_COMMA) continue;
                if (tokens[i].type == TOKEN_IDENTIFIER) {
                    if (tokens[i+1].type == TOKEN_INT) {
                        tableHeader.push_back(TableHeader::NUM(tokens[i].value));
                        i += 2;
                    } else if (tokens[i+1].type == TOKEN_CHAR) {
                        if (tokens[i+2].type == TOKEN_LPAREN) {
                            if (tokens[i+3].type == TOKEN_NUM && tokens[i+4].type == TOKEN_RPAREN) {
                                tableHeader.push_back (
                                    TableHeader::TEXT (
                                        tokens[i].value,
                                        std::stoi(tokens[i+3].value)
                                    )
                                );
                                i += 5;
                            }
                        } else {
                            tableHeader.push_back (
                                TableHeader::SYMBOL (
                                        tokens[i].value
                                )
                            );
                            i += 2;
                        }
                    } else return false;
                }
            }
        } else return false;

        table::create(
            dirPath + tableName + ".tsql",
            tableHeader
        );

        return true;
    }

    return false;
}

auto insert(std::vector<Token> tokens) -> bool {
    if (tokens.size() < 6) return false;
    if (tokens[0].type == TOKEN_INSERT && tokens[1].type == TOKEN_INTO && tokens[2].type == TOKEN_IDENTIFIER) {
        std::string tableName = tokens[2].value;
        std::vector<std::string> row;

        if (tokens[3].type == TOKEN_LPAREN && tokens.back().type == TOKEN_RPAREN) {
            for (auto i = 4; i < tokens.size() - 1; i++) {
                if (tokens[i].type == TOKEN_COMMA) continue;
                if (tokens[i].type == TOKEN_NUM || tokens[i].type == TOKEN_STRING) {
                    row.push_back(tokens[i].value);
                } else return false;
            }
        } else return false;

        auto db = table::open(dirPath + tableName + ".tsql");
        db.insert(row);

        return true;
    }

    return false; 
}

auto drop(std::vector<Token> tokens) -> bool {
    if (tokens.size() < 3) return false;
    if (tokens[0].type == TOKEN_DROP && tokens[1].type == TOKEN_TABLE && tokens[2].type == TOKEN_IDENTIFIER) {
        std::remove((dirPath + tokens[2].value + ".tsql").c_str());
        return true;
    }

    return false;
}

auto where(std::shared_ptr<table> db, uint16_t row, std::vector<Token> tokens) -> bool {
    std::vector<Token> parseToken = parse(tokens);

    bool flag = false;
    std::stack<Token> stack;
    for (auto& tw : parseToken) {
        if (tw.type == TOKEN_NUM || tw.type == TOKEN_STRING || tw.type == TOKEN_IDENTIFIER) {
            stack.push(tw);
        } else  if (precedence(tw) != 0) {
            if (tw.type == TOKEN_NOT) {
                bool a = stack.top().value == "0";
                stack.pop();
                stack.push(Token{TOKEN_NUM, (a ? "1" : "0")});
            } else {
                auto a = stack.top(); stack.pop();
                auto b = stack.top(); stack.pop();

                if (a.type == TOKEN_NUM && a.type == b.type) {
                    switch (tw.type) {
                        case TOKEN_TIMES:
                            stack.push(Token{
                                TOKEN_NUM,std::to_string(std::stoi(b.value) * std::stoi(a.value))
                            });
                        break;
                        case TOKEN_SLASH:
                            stack.push(Token{
                                TOKEN_NUM,std::to_string(std::stoi(b.value) / std::stoi(a.value))
                            });
                        break;
                        case TOKEN_PLUS:
                            stack.push(Token{
                                TOKEN_NUM,std::to_string(std::stoi(b.value) + std::stoi(a.value))
                            });
                        break;
                        case TOKEN_MINUS:
                            stack.push(Token{
                                TOKEN_NUM,std::to_string(std::stoi(b.value) - std::stoi(a.value))
                            });
                        break;
                        case TOKEN_AND:
                            stack.push(Token{
                                TOKEN_NUM,std::to_string(std::stoi(b.value) && std::stoi(a.value))
                            });
                        break;
                        case TOKEN_OR:
                            stack.push(Token{
                                TOKEN_NUM, std::to_string(std::stoi(b.value) || std::stoi(a.value))
                            });
                        break;
                    }
                } else if (a.type == TOKEN_NUM || b.type == TOKEN_NUM) {
                    int var1, var2;

                    if (a.type == TOKEN_IDENTIFIER) {
                        var2 = db->getIntByCell(a.value, row);
                    } else {
                        var2 = std::stoi(a.value);
                    }

                    if (b.type == TOKEN_IDENTIFIER) {
                        var1 = db->getIntByCell(b.value, row);
                    } else {
                        var1 = std::stoi(b.value);
                    }

                    switch (tw.type) {
                        case TOKEN_EQ:
                            stack.push(Token{
                                TOKEN_NUM,std::to_string(var1 == var2)
                            });
                        break;
                        case TOKEN_LSS:
                            stack.push(Token{
                                TOKEN_NUM,std::to_string(var1 < var2)
                            });
                        break;
                        case TOKEN_GTR:
                            stack.push(Token{
                                TOKEN_NUM,std::to_string(var1 > var2)
                            });
                        break;
                        case TOKEN_LEQ:
                            stack.push(Token{
                                TOKEN_NUM,std::to_string(var1 <= var2)
                            });
                        break;
                        case TOKEN_NEQ:
                            stack.push(Token{
                                TOKEN_NUM,std::to_string(var1 != var2)
                            });
                        break;
                        case TOKEN_GEQ:
                            stack.push(Token{
                                TOKEN_NUM,std::to_string(var1 >= var2)
                            });
                        break;
                    }
                } else if (a.type == TOKEN_STRING || b.type == TOKEN_STRING) {
                    std::string var1, var2;

                    if (a.type == TOKEN_IDENTIFIER) {
                        var1 = db->getStringByCell(a.value, row);
                    } else {
                        var1 = a.value;
                    }

                    if (b.type == TOKEN_IDENTIFIER) {
                        var2 = db->getStringByCell(b.value, row);
                    } else {
                        var2 = b.value;
                    }

                    if (tw.type == TOKEN_EQ) {
                        stack.push(Token{
                            TOKEN_NUM, (var1 == var2? "1" : "0")
                        });
                    }
                }
            }
        }
        else return false;
    }

    if (stack.size() != 1) return false;
    
    return stack.top().value == "1" ? true : false;
}