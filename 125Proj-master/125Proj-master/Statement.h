//
// Created by Jared Tuck on 10/24/2021.
//

#ifndef INC_125PROJ_STATEMENT_H
#define INC_125PROJ_STATEMENT_H
#include "linked_list.h"
#include "SymTab.h"
#include "Stmt.h"
#include "Parser.h"

class Stmt1; //declaring all classes to handle different grammar variations of mini-lang
class Stmt2;
class Stmt3;
class Stmt4;
class Stmt5;
class Stmt6;
class Block;

class Stmt{
public:
    Stmt(linked_list* list, SymTab* T, int D);
    linked_list* LIST;
    SymTab* sTable;
    int Depth;
    Stmt1* S1= nullptr;
    Stmt2* S2= nullptr;
    Stmt3* S3= nullptr;
    Stmt4* S4= nullptr;
    Stmt5* S5= nullptr;
    Stmt6* S6= nullptr;
    Block* B= nullptr;
    string BREAK=" ";
    void printStmt();
    void makeNewStmt();//Determine type of statement and locate its endpoint.
};


#endif //INC_125PROJ_STATEMENT_H
