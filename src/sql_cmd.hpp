#include <string>
#include <vector>
#include <memory>
#include "lexer.hpp"
#include "table.hpp"
#include "vars.hpp"

auto sqlExe(std::string cmd) -> bool;

auto create(std::vector<Token> tokens) -> bool;
auto insert(std::vector<Token> tokens) -> bool;
auto drop(std::vector<Token> tokens) -> bool;
auto select(std::vector<Token> tokens) -> bool;

auto where(std::shared_ptr<table> db, uint16_t row, std::vector<Token> tokens) -> bool;