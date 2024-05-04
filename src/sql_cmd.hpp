#include <string>
#include <vector>
#include "lexer.hpp"
#include "vars.hpp"

auto sqlExe(std::string cmd) -> bool;

auto create(std::vector<Token> tokens) -> bool;
auto insert(std::vector<Token> tokens) -> bool;
auto drop(std::vector<Token> tokens) -> bool;