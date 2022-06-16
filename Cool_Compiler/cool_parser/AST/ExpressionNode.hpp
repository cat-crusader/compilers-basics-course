
#ifndef COOL_COMPILER_EXPRESSIONNODE_HPP
#define COOL_COMPILER_EXPRESSIONNODE_HPP

#include <vector>
#include <string>
#include <iostream>

struct ExpressionNode{

    std::string text;

    std::vector<ExpressionNode*> child_nodes;

    void print(int tabs){
        for(int i=0;i<tabs;i++)
            std::cout<<"\t";
        std::cout<<text;
        if(child_nodes.size())
           std::cout<<"{";
        std::cout<<"\n";
        for(auto i:child_nodes)
            i->print(tabs+1);
        if(child_nodes.size()){
            for(int i=0;i<tabs;i++)
                std::cout<<"\t";
            std::cout<<"}\n";
        }
    };

    ExpressionNode(std::string t){
        text = t;
    };

    ExpressionNode(){};
};

#endif //COOL_COMPILER_EXPRESSIONNODE_HPP
