#include <stack>
#include "parser.hpp"

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