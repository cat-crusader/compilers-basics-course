#ifndef COOL_COMPILER_CODEHANDLER_HPP
#define COOL_COMPILER_CODEHANDLER_HPP

#include <vector>
#include "cool_lexer/Lexer.hpp"
#include "cool_parser/Parser.hpp"

class CodeHandler{

private:
     Lexer lexer;

public:
    void start(const char* code_path){

            lexer.lex_analysis(code_path);
            for(auto token:lexer.get_tokens()){
                std::cout<<token;
            }
            std::cout<<"\n\n\n\n\n";
            system("pause");

            Parser parser(lexer.get_tokens());
            parser.parse();

    }
};

#endif //COOL_COMPILER_CODEHANDLER_HPP
