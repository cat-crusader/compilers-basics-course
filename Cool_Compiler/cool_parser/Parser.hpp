

#ifndef COOL_COMPILER_PARSER_HPP
#define COOL_COMPILER_PARSER_HPP

#include <vector>
#include <exception>
#include "../Token.hpp"
#include "AST/ExpressionNode.hpp"


class Parser {
private:
    const std::vector<std::string> _types = {"Int","Float","Object","Bool"};

    std::vector<Token> token_list;

    void error_alert(Token token);

    void class_handler(ExpressionNode *node, int &num_token);

    void class_block_handler(ExpressionNode *node, int &num_token);

    void variable_handler(ExpressionNode *node, int &num_token, std::vector<std::string> stop);

    void expression_handler(ExpressionNode *node, int &num_token, std::vector<std::string> stop, bool blocks = false);

    void parametr_handler(ExpressionNode *node, int &num_token);

    void method_handler(ExpressionNode *node, int &num_token);

    void block_handler(ExpressionNode *node, int &num_token, std::string end, std::string end2);

    void let_in_handler(ExpressionNode *node, int &num_token);

    void loop_handler(ExpressionNode *node, int &num_token);

    void case_handler(ExpressionNode *node, int &num_token);

    void if_else_handler(ExpressionNode *node, int &num_token);



public:
    void parse();

    Parser(std::vector<Token> token_list);

};


#endif //COOL_COMPILER_PARSER_HPP
