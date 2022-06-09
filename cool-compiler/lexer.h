
#ifndef COOL_COMPILER_LEXER_H
#define COOL_COMPILER_LEXER_H


#include <iostream>
#include <vector>
#include <regex>
#include <fstream>

#include "token.h"
#include "token-type.h"

class Lexer {

	private:std::vector<Token> tokens = {};

	private:int _line = 0;

	private:std::string _prevChar;



	bool IsTokenValid(const std::string& str, const std::string& reg_exp);

	void GetKeyWordOrIdentifier();

	void GetNumOrFloat();

	void GetOperator();

	void GetComment();

	void GetString();

	void ErrorHandling();

	 private:bool NextToken(std::ifstream& file);

	 public:void LexAnalysis(const char* codePath);

};

#endif