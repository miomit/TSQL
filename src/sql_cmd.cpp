#include "sql_cmd.hpp"
#include "table.hpp"

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