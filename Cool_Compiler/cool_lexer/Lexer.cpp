
#include "Lexer.hpp"


bool Lexer::is_valid_token(const std::string& str, std::string reg_exp) {
    std::regex r(reg_exp);
    return std::regex_match(str.data(), r);
}

bool Lexer::get_key_word_or_identifier(std::ifstream &file) {
    std::string result = prev_char;
    bool end = false;
    bool is_key_word;
    while(is_valid_token(result, types[1].get_regex())){
        is_key_word  = is_valid_token(result, types[0].get_regex());
        if(file.eof()){
            end = true;
            break;
        }
        prev_char = file.get();
        result += prev_char;
    }
    if(!end)
        result.pop_back();
    tokens.push_back(Token(is_key_word?types[0]:types[1],
                            line,
                            row,
                            result
            ));
    row += result.length();
    return !end;
}

bool Lexer::get_comment(std::ifstream &file) {
    std::string result = "--";
    prev_char = "";
    while(!file.eof() && prev_char!="\n"){
        result += prev_char;
        prev_char = file.get();
    }

    return true;

}

bool Lexer::get_operator(std::ifstream &file) {
    std::string result = prev_char;
    bool end = false;
    while(is_valid_token(result, types[2].get_regex())){
        if(file.eof()){
            end = true;
            break;
        }
        prev_char = file.get();
        result += prev_char;
    }
    if(is_valid_token(result, types[8].get_regex()))
        return get_comment(file);//--
    if(!end)
        result.pop_back();
    tokens.push_back(Token(types[2],
                           line,
                           row,
                           result
    ));
    row += result.length();
    return !end;
}

bool Lexer::get_punctuation(std::ifstream &file) {
    std::string result = prev_char;
    tokens.push_back(Token(types[3],
                           line,
                           row,
                           result
    ));
    row++;
    prev_char = file.get();
    return true;
}

bool Lexer::get_num_or_float(std::ifstream &file) {
    std::string result = prev_char;
    bool end = false;
    while(is_valid_token(result, types[4].get_regex())){
        if(file.eof()){
            end = true;
            break;
        }
        prev_char = file.get();
        result += prev_char;
    }
    if(result[result.length()-1] == '.' && !end){

        prev_char = file.get();
        if(is_valid_token(prev_char, types[4].get_regex())){
            while(is_valid_token(result+prev_char, types[5].get_regex())){
                if(file.eof()){
                    end = true;
                    break;
                }
                result += prev_char;
                prev_char = file.get();
            }
            tokens.push_back(Token(types[5],
                                   line,
                                   row,
                                   result
            ));

        } else{
            tokens.push_back(Token(types[9],
                                   line,
                                   row,
                                   result
            ));
            end = file.eof();
        }
    }
    else{
        if(!end)
            result.pop_back();
        tokens.push_back(Token(types[4],
                               line,
                               row,
                               result
        ));

    }
    row += result.length();
    return !end;
}

bool Lexer::get_string(std::ifstream &file) {
    std::string result = "";
    bool end = false;
    bool end_line = false;
    do{
        if(prev_char == "\n"){
            end_line = true;
            break;
        }
        result += prev_char;
        if(file.eof()){
            end = true;
            break;
        }
        prev_char = file.get();

    }while(!is_valid_token(prev_char, types[6].get_regex()));

    if(!end_line&&!end){
        result+= prev_char;
        prev_char = file.get();
    }
    tokens.push_back(Token(end||end_line?types[9]:types[6],
                           line,
                           row,
                           result
    ));
    return true;
}

bool Lexer::get_space(std::ifstream &file) {
    std::string result = prev_char;
    if(result == "\n") {
        line++;
        row = 1;
    } else if(result == " ")
        row++;
    prev_char = file.get();
    return true;
}

bool Lexer::error_handling(std::ifstream &file) {
    std::string result = prev_char;
    bool end = false;
    while(!is_valid_token(prev_char, types[1].get_regex())&&
            !is_valid_token(prev_char, types[2].get_regex())&&
            !is_valid_token(prev_char, types[3].get_regex())&&
            !is_valid_token(prev_char, types[4].get_regex())&&
            !is_valid_token(prev_char, types[6].get_regex())&&
            !is_valid_token(prev_char, types[7].get_regex())){
        if(file.eof()){
            end = true;
            break;
        }
        prev_char = file.get();
        result += prev_char;
    }
    if(!end)
        result.pop_back();
    tokens.push_back(Token(types[9],
                           line,
                           row,
                           result
    ));
    row += result.length();
    return !end;
}


bool Lexer::next_token(std::ifstream &file) {
   if(file.eof())
       return false;

   if(is_valid_token(prev_char, types[1].get_regex()))
       return get_key_word_or_identifier(file);
   if (is_valid_token(prev_char, types[2].get_regex()))
       return get_operator(file);
   if (is_valid_token(prev_char, types[3].get_regex()))
       return get_punctuation(file);
   if (is_valid_token(prev_char, types[4].get_regex()))
       return get_num_or_float(file);
   if (is_valid_token(prev_char, types[6].get_regex()))
       return get_string(file);
   if (is_valid_token(prev_char, types[7].get_regex()))
       return get_space(file);
   return error_handling(file);

}

void Lexer::lex_analysis(const char* code_path) {
    tokens = {};
    line  = 1;
    row = 1;
    std:: ifstream code(code_path);
    if(!code.is_open()){
        std::cout<<"file not found!\n";
    }
    prev_char = code.get();
    while(next_token(code)){}
    code.close();
}

std::vector<Token> Lexer::get_tokens(){
    return tokens;
}


