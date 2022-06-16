

#ifndef COOL_COMPILER_TOKEN_HPP
#define COOL_COMPILER_TOKEN_HPP

#include "TokenType.hpp"
#include <ostream>

class Token{

public:
    TokenType type;

    int line;

    int row;

    std::string name;

    Token(TokenType type, int line, int row, std::string name){
        this->type = type;
        this->line = line;
        this->row = row;
        this->name = name;
    }

    friend std::ostream& operator<<(std::ostream &out, Token token);
};

inline std::ostream &operator<<(std::ostream &out, Token token) {
    out <<"  "<<token.type.get_token_type()<<" "<<token.name<<" ("<<token.line<<"|"<<token.row<<")\n";
    return out;
}


#endif //COOL_COMPILER_TOKEN_HPP