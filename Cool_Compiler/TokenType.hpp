

#ifndef COOL_COMPILER_TOKENTYPE_HPP
#define COOL_COMPILER_TOKENTYPE_HPP

#include <string>



class TokenType{

private:
    std::string name;

    std::string reg_exp;

    std::string token_type;

public:
    TokenType(){
        this->name = "";
        this->token_type = "unidentified_token";
        this->reg_exp = "";
    }

    TokenType(std::string name){
        this->name = name;
        if(this->name == "keyword"){
            this->token_type = "KEYWORD";
            this->reg_exp = "not|if|then|else|fi|in|class|inherits|let|loop|pool|while|case|esac|of|new|isvoid|str_const|int_const|bool_const|typeid|objectid|assign|error|let_stmt|Int|Float|Bool|self|Object";
        }else if (this->name == "identifier"){
            this->token_type = "IDENTIFIER";
            this->reg_exp = "[a-zA-Z_][a-zA-Z_0-9]*";
        } else if(this->name == "operator") {
            this->token_type = "OPERATOR";
            this->reg_exp = "<(-|=|)|=(>|)|>|\\+|\\*|\\/|-(>|)|\\.|\\~|\\@";
        } else if(this->name == "punctuation"){
            this->token_type = "PUNCTUATION";
            this->reg_exp = "\\,|\\;|\\(|\\)|\\{|\\}|\\:";
        }else if(this->name == "number"){
            this->token_type = "NUMBER";
            this->reg_exp = "[0-9]*";
        } else if (this->name == "float"){
            this->token_type = "FLOAT";
            this->reg_exp = "^(0|([1-9][0-9]*))(\\.[0-9]+)?$";
        }else if (this->name == "string"){
            this->token_type = "STR";
            this->reg_exp = "\"";
        }else if (this->name == "space"){
            this->token_type = "SPACE";
            this->reg_exp = "[ \\n\\t]";
        }else if (this->name == "comments"){
            this->token_type = "COMMENTS";
            this->reg_exp = "--";
        }else{
            this->token_type = "ERROR_TOKEN";
            this->reg_exp = "";
        }
    }

    std::string get_regex(){
        return reg_exp;
    }

    std::string get_token_type(){
        return token_type;
    }

    std::string get_name(){
        return name;
    }

};

static TokenType types[10] = {
        TokenType("keyword"),
        TokenType("identifier"),
        TokenType("operator"),
        TokenType("punctuation"),
        TokenType("number"),
        TokenType("float"),
        TokenType("string"),
        TokenType("space"),
        TokenType("comments"),
        TokenType("error")
};

#endif //COOL_COMPILER_TOKENTYPE_HPP