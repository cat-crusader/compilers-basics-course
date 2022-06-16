

#ifndef COOL_COMPILER_LEXER_HPP
#define COOL_COMPILER_LEXER_HPP

#include <iostream>
#include <vector>
#include <regex>
#include <fstream>

#include "../TokenType.hpp"
#include "../Token.hpp"


class Lexer {

private:
    std::vector<Token> tokens;

    int line;

    int row;

    std::string prev_char;

    bool is_valid_token(const std::string& str,std::string reg_exp);

    bool get_key_word_or_identifier(std::ifstream &file);

    bool get_num_or_float(std::ifstream &file);

    bool get_operator(std::ifstream &file);

    bool get_punctuation(std::ifstream &file);

    bool get_string(std::ifstream &file);

    bool get_space(std::ifstream &file);

    bool get_comment(std::ifstream &file);

    bool error_handling(std::ifstream &file);

    bool next_token(std::ifstream &file);

public:
    void lex_analysis(const char* code_path);

    std::vector<Token> get_tokens();

};

#endif
