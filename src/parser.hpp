#include <vector>
#include "lexer.hpp"

auto precedence (Token token) -> int;

auto parse (std::vector<Token> tokens) -> std::vector<Token>;