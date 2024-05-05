#include <string>
#include <vector>
#include <memory>
#include "lexer.hpp"
#include "table.hpp"
#include "vars.hpp"

auto sqlExe(std::string cmd) -> bool;

auto CREATE(std::vector<Token> tokens)  -> bool;
auto INSERT(std::vector<Token> tokens)  -> bool;
auto DROP(std::vector<Token> tokens)    -> bool;
auto SELECT(std::vector<Token> tokens)  -> bool;
auto DELETE(std::vector<Token> tokens)  -> bool;
auto UPDATE(std::vector<Token> tokens)  -> bool;

auto WHERE(std::shared_ptr<table> db, uint16_t row, std::vector<Token> tokens) -> bool;
auto SET(std::shared_ptr<table> db, uint16_t row, std::vector<Token> tokens) -> bool;