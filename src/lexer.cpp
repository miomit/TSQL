#include <iostream>
#include "lexer.hpp"

auto Token::print() -> void {
    std::cout << "Token { \"";
    if (type < TLex.size()) {
        std::cout << TLex[type];
    } else {
        switch (type) {
            case TOKEN_NUM: std::cout << "NUM"; break;
            case TOKEN_STRING: std::cout << "STRING"; break;
            case TOKEN_IDENTIFIER: std::cout << "IDENTIFIER"; break;
            default: std::cout << type;
        }
    }
    std::cout << "\", \"" << value << "\" }" << std::endl;
}

auto lexer(std::string line) -> std::vector<Token> {
    std::string buff = "";
    std::vector<Token> tokens;
    enum state { H, IDENT, ALE, NUMB, NEQN, LINE };
    state CS = H;
    for (auto i = 0; i <= line.size(); i++) {
        char c = (i == line.size()? '\n' : line[i]);
        switch (CS) {
            case H:
                if ( c==' ' || c == '\n' || c== '\r' || c == '\t' );
                else if (c == '\'') {
                    CS = LINE;
                }
                else if (isalpha(c)) {
                    buff += c;
                    CS = IDENT;
                } else if (isdigit(c)) {
                    buff += c;
                    CS = NUMB;
                } else if (c == '-') {
                    buff += c;
                    CS = NEQN;
                } else if (c == '<' || c == '>') {
                    buff += c;
                    CS  = ALE;
                } else {
                    buff += c;
                    bool isAddedToken = false;
                    for (int j = 0; j < TLex.size(); j++) {
                        if (buff == TLex[j]) {
                            isAddedToken = true;
                            tokens.push_back(Token{TokenType(j), buff});
                            break;
                        }
                    }
                    if (!isAddedToken) {
                        throw "\nToken is not exist [" + buff + "]\n";
                    }
                    buff = "";
                }
            break;
            case IDENT:
                if (isalpha(c) || isdigit(c)) buff += c;
                else {
                    bool isAddedToken = false;
                    for (int j = 0; j < TLex.size(); j++) {
                        if (buff == TLex[j]) {
                            isAddedToken = true;
                            tokens.push_back(Token{TokenType(j), buff});
                            break;
                        }
                    }
                    if (!isAddedToken) {
                        tokens.push_back(Token{TOKEN_IDENTIFIER, buff});
                    }
                    buff = ""; CS = H; i--;
                }
            break;
            case NUMB:
                if (isdigit(c)) buff += c;
                else {
                    tokens.push_back(Token{TOKEN_NUM, buff});
                    buff = ""; CS = H; i--;
                }
            break;
            case NEQN:
                if (isdigit(c)) {
                    buff += c;
                    CS = NUMB;
                } else {
                    tokens.push_back(Token{TOKEN_MINUS, buff});
                    buff = ""; CS = H; i--;
                }
            break;
            case ALE:
                if (c == '=') {
                    buff += c;
                    for (int j = 0; j < TLex.size(); j++) {
                        if (buff == TLex[j]) {
                            tokens.push_back(Token{TokenType(j), buff});
                            break;
                        }
                    }
                    buff = ""; CS = H;
                } else {
                    for (int j = 0; j < TLex.size(); j++) {
                        if (buff == TLex[j]) {
                            tokens.push_back(Token{TokenType(j), buff});
                            break;
                        }
                    }
                    buff = ""; CS = H; i--;
                }
            break;
            case LINE:
                if (c == '\'') {
                    tokens.push_back(Token{TOKEN_STRING, buff});
                    buff = ""; CS = H;
                } else {
                    buff += c;
                }
            break;
        }
    }

    return tokens;
}
