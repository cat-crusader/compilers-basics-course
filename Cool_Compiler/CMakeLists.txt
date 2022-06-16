cmake_minimum_required(VERSION 3.21)
project(Cool_Compiler)

set(CMAKE_CXX_STANDARD 23)

add_executable(Cool_Compiler main.cpp cool_lexer/Lexer.cpp cool_lexer/Lexer.hpp TokenType.hpp Token.hpp CodeHandler.hpp cool_parser/Parser.cpp cool_parser/Parser.hpp cool_parser/AST/ExpressionNode.hpp)
