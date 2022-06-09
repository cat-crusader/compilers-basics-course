#pragma once
#ifndef COOL_COMPILER_TOKEN_HPP
#define COOL_COMPILER_TOKEN_HPP
#include "token-type.h"

class Token {
	public:TokenType Type;
	public:int Line;
	public:std::string Name;
	public:bool LexError;

		  Token(TokenType type, int line, std::string name, bool lex_error = false) {
			  this->Type = type;
			  this->Line = line;
			  this->Name = name;
			  this->LexError = lex_error;
		  }

std::string ToString() {
	std::string result = "Token  {\n\t" + Type.GetTokenType() + " : " + Name;
	if (LexError)
		result += ("\n\terror in line " + std::to_string(Line));
	result += "\n\t}\n";
	return result;
}
};

#endif 