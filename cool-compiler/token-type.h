
#ifndef COOL_COMPILER_TOKEN_TYPE_HPP
#define COOL_COMPILER_TOKEN_TYPE_HPP

#include <string>

    class TokenType {

    private:std::string _name;
    private:std::string _regExp;
    private:std::string _tokenType;

    public:std::string GetRegex() {
        return _regExp;
    }
    public:std::string GetTokenType() {
        return _tokenType;
    }
    public:std::string GetName() {
        return _name;
    }

    public:TokenType() {
        this->_name = "";
        this->_regExp = "";
        this->_tokenType = "unidentified token";
    }


    TokenType(std::string name) {
        this->_name = name;
        if (this->_name == "key_word") {
            this->_tokenType = "KEY_WORD";
            this->_regExp = "if|else|fi|in|class|inherits|let|loop|pool|then|while|case|esac|of|new|isvoid|str_const|int_const|bool_const|typeid|objectid|assign|error|let_stmt";
        }
        else if (this->_name == "operator") {
            this->_tokenType = "OPERATOR";
            this->_regExp = "not|<(-|=|)|=(>|=|)|>|\\+|\\*|\\/|-(>|)|\\.|\\~|\\@";
        }
        else if (this->_name == "punctuation") {
            this->_tokenType = "PUNCTUATION";
            this->_regExp = "\\,|\\;|\\(|\\)|\\{|\\}|\\:";
        }
        else if (this->_name == "identifier") {
            this->_tokenType = "IDENTIFIER";
            this->_regExp = "[a-zA-Z_][a-zA-Z_0-9]*";
        }
        else if (this->_name == "number") {
            this->_tokenType = "NUMBER";
            this->_regExp = "[0-9]*";
        }
        else if (this->_name == "float") {
            this->_tokenType = "FLOAT";
            this->_regExp = "^(0|([1-9][0-9]*))(\\.[0-9]+)?$";
        }
        else if (this->_name == "string") {
            this->_tokenType = "STR";
            this->_regExp = "\"";
        }
        else if (this->_name == "space") {
            this->_tokenType = "SPACE";
            this->_regExp = "[ \\n\\t\\r]";
        }
        else if (this->_name == "comments") {
            this->_tokenType = "COMMENTS";
            this->_regExp = "//";
        }
        else {
            this->_tokenType = "unidentified token";
            this->_regExp = "";
        }
    }


    
};
#endif//COOL_COMPILER_TOKEN_TYPE_HPP