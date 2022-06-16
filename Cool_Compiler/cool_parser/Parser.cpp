
#include "Parser.hpp"

void Parser::error_alert(Token token) {
    throw std::runtime_error("expected syntax error in  "+ std::to_string(token.line)+
                             "|" + std::to_string(token.row));
}

void Parser::class_handler(ExpressionNode *node, int &num_token) {
    auto* _class = new ExpressionNode;
    _class->text = "class";
    ++num_token;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    auto* class_id = new ExpressionNode;
    if(token_list[num_token].type.get_token_type() == "IDENTIFIER"){
        class_id->text = token_list[num_token].name;
    }
    else error_alert(token_list[num_token]);
    ++num_token;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    if(token_list[num_token].name == "inherits"){
        auto* _inh = new ExpressionNode;
        _inh->text = "inherits";
        _inh->child_nodes.push_back(class_id);
        ++num_token;

        if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
        auto* class_id_parrent = new ExpressionNode;
        if(token_list[num_token].type.get_token_type() == "IDENTIFIER")
            class_id_parrent->text = token_list[num_token].name;
        else error_alert(token_list[num_token]);
        _inh->child_nodes.push_back(class_id_parrent);
        ++num_token;
        if (num_token >= token_list.size()) error_alert(token_list[num_token-1]);
        if(token_list[num_token].name == "{"){
            class_block_handler(_class, num_token);
        }
        _class->child_nodes.push_back(_inh);
    } else if(token_list[num_token].name == "{"){
        _class->child_nodes.push_back(class_id);
        class_block_handler(_class, num_token);
    } else error_alert(token_list[num_token]);

    if(token_list[num_token].name == ";")
        node->child_nodes.push_back(_class);
    else error_alert(token_list[num_token]);
}

void Parser::class_block_handler(ExpressionNode *node, int &num_token) {//next
    auto* class_body = new ExpressionNode;
    class_body->text = "class_body";
    ++num_token;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);

    while(token_list[num_token].name !="}"){
        if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
        if(token_list[num_token].type.get_token_type() == "IDENTIFIER"){
            if(num_token+1 >= token_list.size()) error_alert(token_list[num_token]);
            if(token_list[num_token+1].name == ":")
                variable_handler(class_body, num_token, {";"});
            else if(token_list[num_token+1].name == "(")
                method_handler(class_body, num_token);
            else error_alert(token_list[num_token+1]);
        }
        else error_alert(token_list[num_token]);
    }
    node->child_nodes.push_back(class_body);
    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token]);
}

void Parser::variable_handler(ExpressionNode *node, int &num_token, std::vector<std::string> stop){
    auto* _id = new ExpressionNode;
    _id->text = token_list[num_token].name;
    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    auto* semilicon  = new ExpressionNode;
    semilicon->text = ":";
    semilicon->child_nodes.push_back(_id);
    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    bool is_type = false;
    for(auto t: _types){
        if(t == token_list[num_token].name)
            is_type = true;
    }
    if(is_type || token_list[num_token].type.get_token_type() == "IDENTIFIER"){
        auto* type  = new ExpressionNode;
        type->text = token_list[num_token].name;
        semilicon->child_nodes.push_back(type);
        num_token++;
        if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
        bool is_stop = false;
        for(auto s:stop){
            if(token_list[num_token].name==s)
                is_stop = true;
        }
        if(is_stop){
            node->child_nodes.push_back(semilicon);
        } else if (token_list[num_token].name=="<-"){
            auto* op  = new ExpressionNode;
            op->text = "<-";
            op->child_nodes.push_back(semilicon);
            num_token++;
            if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
            expression_handler(op,num_token, stop);
            node->child_nodes.push_back(op);
        } else error_alert(token_list[num_token]);

    } else error_alert(token_list[num_token]);
    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
}

void Parser::expression_handler(ExpressionNode *node, int &num_token, std::vector<std::string> stop, bool blocks){
    bool is_open = true;
    bool is_first = true;
    auto* op = new ExpressionNode;
    op->text = "expression";
    bool level = true;
    bool is_id = true;
    ExpressionNode** last_id = &op;

    while(true){

        for(auto s:stop){
            if(token_list[num_token].name==s){
                is_open = false;
            }
        }
        if(!is_open) break;

        if(token_list[num_token].name == "if" && blocks){
            if_else_handler(op, num_token);
            node->child_nodes.push_back(op);
            return;
        }
        if(token_list[num_token].name == "case" && blocks){
            case_handler(op,num_token);
            node->child_nodes.push_back(op);
            return;
        }
        if(token_list[num_token].name == "while" && blocks){
            loop_handler(op,num_token);
            node->child_nodes.push_back(op);
            return;
        }
        if(is_first && token_list[num_token].name == "new"){
            num_token++;
            if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
            auto* _new = new ExpressionNode;
            _new->text = "new";
            if(token_list[num_token].type.get_token_type() == "IDENTIFIER"||
                    token_list[num_token].type.get_token_type()=="NUMBER"||
                    token_list[num_token].type.get_token_type()=="FLOAT"||
                    token_list[num_token].type.get_token_type()=="STR"){
                auto* _id = new ExpressionNode;
                _id->text = token_list[num_token].name;
                num_token++;
                if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
                if(token_list[num_token].name == "("&&token_list[num_token].type.get_token_type() == "IDENTIFIER")
                    parametr_handler(_id, num_token);
                _new->child_nodes.push_back(_id);
                node->child_nodes.push_back(_new);

                for(auto s:stop){
                    if(token_list[num_token].name==s)
                        is_open = false;
                }
                if(!is_open){
                    return;
                } else error_alert(token_list[num_token]);
            } else error_alert(token_list[num_token]);
        }
        is_first = false;
        if((token_list[num_token].type.get_token_type() == "IDENTIFIER"||
                token_list[num_token].type.get_token_type() == "FLOAT"||
                token_list[num_token].type.get_token_type() == "NUMBER"||
                token_list[num_token].name == "Object"||
                token_list[num_token].type.get_token_type() == "STR")&&is_id){


            is_id = false;

            int num = op->child_nodes.size()-1;


            if(level){
                if(num>-1){
                    op->child_nodes[num]->child_nodes.push_back(new ExpressionNode(token_list[num_token].name));
                    last_id = &op->child_nodes[num]->child_nodes[op->child_nodes[num]->child_nodes.size()-1];
                }
                else {
                    op->child_nodes.push_back(new ExpressionNode(token_list[num_token].name));
                    last_id = &op->child_nodes[0];
                }
            } else{
                (*last_id)->child_nodes.push_back(new ExpressionNode(token_list[num_token].name));
                last_id = &(*last_id)->child_nodes[(*last_id)->child_nodes.size()-1];
            }


            num_token++;
            if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
            continue;
        } else if(token_list[num_token].name=="not"){

            (*last_id)->child_nodes.push_back(new ExpressionNode("not"));
            last_id = &(*last_id)->child_nodes[(*last_id)->child_nodes.size()-1];
            num_token++;
            if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
            level = false;
            continue;
        }else if( token_list[num_token].name=="=>"){
            is_id = true;
            op->text = "=>";
            num_token++;
            if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
            last_id=&op;
            continue;
        }else if ((token_list[num_token].type.get_token_type() == "OPERATOR" && !is_id) ||
                (token_list[num_token].name == ":" && !is_id)){

            is_id = true;
            auto* cnst = new ExpressionNode;
            cnst->text = token_list[num_token].name;

            if(token_list[num_token].name!="."&&
            token_list[num_token].name!="<-"&&
            token_list[num_token].name!=":"){
                cnst->child_nodes = op->child_nodes;
                op->child_nodes.clear();
                op->child_nodes.push_back(cnst);
                level = true;
            } else{

                  auto* id1 = new ExpressionNode;
                  id1->text = (*last_id)->text;
                  id1->child_nodes = (*last_id)->child_nodes;
                  (*last_id)->text = cnst->text;

                  (*last_id)->child_nodes.clear();
                  (*last_id)->child_nodes.push_back(id1);
                  level = false;
            }
            num_token++;
            if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
            continue;

        } else if(token_list[num_token].name == "(") {
            parametr_handler((*last_id), num_token);
            continue;
        } else error_alert(token_list[num_token]);


    }
    node->child_nodes.push_back(op);

}

void Parser::parametr_handler(ExpressionNode *node, int &num_token) {
    ++num_token;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    auto* param = new ExpressionNode;
    param->text = "parameters";
    while(true){

        if(token_list[num_token].type.get_token_type() == "IDENTIFIER"||
        token_list[num_token].type.get_token_type() == "FLOAT"||
        token_list[num_token].type.get_token_type() == "NUMBER"||
        token_list[num_token].type.get_token_type() == "STR")
            expression_handler(param, num_token, {",",")"});
        else if(token_list[num_token].name == ",")
            ++num_token;
        else if(token_list[num_token].name == ")")
            break;
        else error_alert(token_list[num_token]);

    }
    node->child_nodes.push_back(param);
    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
}

void Parser::method_handler(ExpressionNode *node, int &num_token){

    auto* _id = new ExpressionNode;
    _id->text = token_list[num_token].name;
    num_token+=2;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);

    auto* param = new ExpressionNode("parameters");


    while(token_list[num_token].name!="{"){

        if(token_list[num_token].type.get_token_type()=="IDENTIFIER"){
            if(num_token+1 >= token_list.size()) error_alert(token_list[num_token]);
            if(token_list[num_token+1].name == ":")
                variable_handler(param, num_token, {",",")"});
        } else if(token_list[num_token].name==":"){
            break;
        }else if (token_list[num_token].name==",")
            num_token++;
        else error_alert(token_list[num_token]);
        if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    }

    _id->child_nodes.push_back(param);


    if(token_list[num_token].name==":"){


        num_token++;
        if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
        bool is_type = false;
        for(auto t: _types){
            if(t == token_list[num_token].name)
                is_type = true;
        }
        if(is_type || token_list[num_token].type.get_token_type() == "IDENTIFIER"){
            auto* ch = new ExpressionNode(_id->text);
            ch->child_nodes = _id->child_nodes;
            _id->child_nodes.clear();
            _id->child_nodes.push_back(ch);
            _id->child_nodes.push_back(new ExpressionNode(token_list[num_token].name));
            _id->text = ":";
        } else error_alert(token_list[num_token]);
        num_token++;
        if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    }


    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);

    block_handler(_id, num_token, "}",";");

    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    node->child_nodes.push_back(_id);

}

void Parser::block_handler(ExpressionNode *node, int &num_token, std::string end, std::string end2) {
    auto* block = new ExpressionNode;
    block->text = "block";
    if(token_list[num_token].name=="let"){
        let_in_handler(block, num_token);

    }

    while(token_list[num_token].name!=end){
         if(token_list[num_token].type.get_token_type() == "IDENTIFIER"||
            token_list[num_token].type.get_token_type()=="NUMBER"||
            token_list[num_token].type.get_token_type()=="FLOAT"||
            token_list[num_token].type.get_token_type()=="STR")
             expression_handler(block, num_token, {";"}, true);
         else if(token_list[num_token].name == "if")
             if_else_handler(block,num_token);
         else if(token_list[num_token].name == "while")
             loop_handler(block,num_token);
         else if(token_list[num_token].name == "case")
             case_handler(block,num_token);
         else error_alert(token_list[num_token]);
         num_token++;
        if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    }

    if(end2.length()){
        num_token++;
        if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
        if(token_list[num_token].name!=end2) error_alert(token_list[num_token]);
    }
    node->child_nodes.push_back(block);
}

void Parser::loop_handler(ExpressionNode *node, int &num_token) {
    auto* loop = new ExpressionNode;
    loop->text = "while";
    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    expression_handler(loop,num_token, {"loop"});

    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    if(token_list[num_token].name!="{") error_alert(token_list[num_token]);
    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    block_handler(loop, num_token, "}","pool");

    node->child_nodes.push_back(loop);
    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    if(token_list[num_token].name!=";") error_alert(token_list[num_token]);

}

void Parser::if_else_handler(ExpressionNode *node, int &num_token) {
    auto* _if = new ExpressionNode;
    _if->text = "if";
    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    expression_handler(_if,num_token, {"then"});

    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);

    if(token_list[num_token].name!="{") error_alert(token_list[num_token]);
    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    block_handler(_if, num_token, "}","else");
    node->child_nodes.push_back(_if);

    auto* _else = new ExpressionNode;
    _else->text = "else";
    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    if(token_list[num_token].name!="{") error_alert(token_list[num_token]);
    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    block_handler(_else, num_token, "}","fi");
    node->child_nodes.push_back(_else);
    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    if(token_list[num_token].name!=";") error_alert(token_list[num_token]);

}

void Parser::case_handler(ExpressionNode *node, int &num_token) {
    auto* _case = new ExpressionNode;
    _case->text = "case";

    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    if(token_list[num_token].type.get_token_type()!="IDENTIFIER"&&
            token_list[num_token].type.get_token_type()!="NUMBER"&&
            token_list[num_token].type.get_token_type()!="FLOAT"&&
            token_list[num_token].type.get_token_type()!="STR") error_alert(token_list[num_token]);

    auto* _id = new ExpressionNode;
    _id->text = token_list[num_token].name;

    expression_handler(_id, num_token, {"of"});



    if(token_list[num_token].name!="of") error_alert(token_list[num_token]);
    auto* _of = new ExpressionNode;
    _of->text = "of";
    _case->child_nodes.push_back(_id);
    _case->child_nodes.push_back(_of);



    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    block_handler(_case, num_token, "esac", ";");
    node->child_nodes.push_back(_case);
}

void Parser::let_in_handler(ExpressionNode *node, int &num_token) {
    auto* let = new ExpressionNode("let");
    auto* in = new ExpressionNode("in");
    num_token++;
    while(token_list[num_token-1].name!="in"){

        variable_handler(let,num_token, {",","in"});
    }

    if(token_list[num_token].name!="{") error_alert(token_list[num_token]);
    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);
    block_handler(in, num_token, "}", ";");

    node->child_nodes.push_back(let);
    node->child_nodes.push_back(in);


    num_token++;
    if(num_token >= token_list.size()) error_alert(token_list[num_token-1]);


}

void Parser::parse(){

    auto* root = new ExpressionNode;
    root->text = "program";
    int num_token = 0;
    while(num_token < token_list.size()){
        if(token_list[num_token].name == "class")
            class_handler(root, num_token);
        num_token++;
    }
    root->print(0);
}

Parser::Parser(std::vector<Token> token_list) {
    this->token_list = token_list;
}


