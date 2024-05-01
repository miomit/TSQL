#include "lexer.hpp"
#include "table.hpp"
#include <cstdio>

const std::string dirPath = "/home/miomit/";

const std::string sqlCreate = "CREATE TABLE Students (Firstname CHAR(10), Surname CHAR(15), Age INT, Phone CHAR(9), SEX CHAR )";
const std::string sqlInsert = "INSERT INTO Students ( 'Yura', 'Movsesyan', 21, '+7(926) 33 80 800', 'M')";
const std::string sqlDrop = "DROP TABLE Students";

auto main() -> int {
    
    auto tokens = lexer(sqlDrop);

    // CREATE
    if (tokens[0].type == TOKEN_CREATE
        && tokens[1].type == TOKEN_TABLE
        && tokens[2].type == TOKEN_IDENTIFIER) {
            std::string tableName = tokens[2].value;
            std::vector<TableHader> tableHeader;
            if (tokens[3].type == TOKEN_LPAREN
                && tokens.back().type == TOKEN_RPAREN) {
                for (auto i = 4; i < tokens.size() - 2; i++) {
                    if (tokens[i].type == TOKEN_COMMA) continue;
                    if (tokens[i].type == TOKEN_IDENTIFIER) {
                        if (tokens[i+1].type == TOKEN_INT) {
                            tableHeader.push_back(TableHader::NUM(tokens[i].value));
                            i += 2;
                        } else if (tokens[i+1].type == TOKEN_CHAR) {
                            if (tokens[i+2].type == TOKEN_LPAREN) {
                                if (tokens[i+3].type == TOKEN_NUM
                                    && tokens[i+4].type == TOKEN_RPAREN) {
                                    tableHeader.push_back(
                                        TableHader::TEXT(
                                            tokens[i].value,
                                            std::stoi(tokens[i+3].value)
                                        )
                                    );
                                    i += 5;
                                }
                            } else {
                                tableHeader.push_back(
                                    TableHader::SYMBOL(
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
