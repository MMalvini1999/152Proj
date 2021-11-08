#ifndef INC_125PROJ_PARSER_H
#define INC_125PROJ_PARSER_H
#include <vector>
#include <iostream>
#include "Lexer.h"
#include "linked_list.h"
#include "Statement.h"
#include "SymTab.h"
#include "Decl.h"

//Block object should pass a complete statement into stmt class
//Class will determine what type of stmt it is, create a new stmt depending on type.

class Block{
public:
    Block(int D, linked_list* List);
    Block(int D, linked_list* List, SymTab* T);
    std::vector<Stmt*> St;
    SymTab* Table;
    linked_list* LIST;
    int Depth;
    void Scan4Stmt(linked_list* List);
    Token* Scan4Decl(linked_list* List);
    void StmtFound(linked_list* list, int POS);
    void printBlock();
};

class Prog{
public:
    Prog(linked_list* LIST);
    Block* B;
    linked_list* List;
    int Depth = 0;
    void printProg();
    void ErrorOut(Token* temp);
    void ErrorCheck1();
    void ErrorCheck2();
};

class Parser{
public:
    Parser(Lexer* L);
    Lexer* Lex;
    Prog* P;
    linked_list* Data;
    void PrintTree();

};


#endif //INC_125PROJ_PARSER_H

