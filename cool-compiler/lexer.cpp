
#include "lexer.h"


bool Lexer::IsTokenValid(const std::string& str, const std::string& reg_exp) {
	static const std::regex r(reg_exp);
	return std::regex_match(str.data(), r);
}
bool Lexer::NextToken(std::ifstream& file) {
	if (!file.eof())
		return false;
	return false;
}
void Lexer::LexAnalysis(const char* codePath) {
	std::ifstream code(codePath);
	_prevChar = code.get();
	while (NextToken(code)) {}
	code.close();
	_line = 0;
}