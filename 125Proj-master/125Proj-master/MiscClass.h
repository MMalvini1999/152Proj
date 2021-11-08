//
// Created by Jared Tuck on 10/24/2021.
//
#ifndef INC_125PROJ_MISCCLASS_H
#define INC_125PROJ_MISCCLASS_H
#include "linked_list.h"
#include "SymTab.h"
class andexpr;

class allexpr   {
    //allexpr || andexpr | andexpr
public:
    allexpr(linked_list* list, SymTab* T, int D);
    linked_list* LIST;
    int Depth;
    SymTab* sTable;
    allexpr* AE1= nullptr;
    andexpr* AE2= nullptr;
    void ErrorCheck();
    void allexpScan();  //scans linkedlist to find allexp seg
    void printAllexpr();
};


class incdecexpr{
    // id++ | id--
public:
    incdecexpr(linked_list* list, SymTab* T, int D);
    linked_list* LIST;
    int Depth;
    SymTab* sTable;
    string ID;
    string s1, s2;
    void printIncDecEx();
};


class factor{
public:
    //(allexpr) | incdecexpr | id | num | real | true | false
    factor(linked_list* list, SymTab* T, int D);
    linked_list* LIST;
    int Depth;
    SymTab* sTable;
    allexpr* allExpression= nullptr;
    incdecexpr* IncDec= nullptr;
    string tok;
    void scan();
    void printFact();
};

class term{
    //term * factor | term / factor | factor
public:
    term(linked_list* list, SymTab* T, int D);
    linked_list* LIST;
    int Depth;
    SymTab* sTable;
    term* T1= nullptr;
    string tok;
    factor* F1= nullptr;
    void termScan();
    void printTerm();
};

class expr      {
    // expr + term | expr - term | term
public:
    expr(linked_list* list, SymTab* T, int D);
    linked_list* LIST;
    int Depth;
    SymTab* sTable;
    expr* E1= nullptr;
    string tok;
    term* T1= nullptr;
    void exprScan();
    void printExpr();
};

class rel       {
    //expr<expr | expr<=expr | expr>expr | expr>=expr | expr
public:
    rel(linked_list* list, SymTab* T, int D);
    linked_list* LIST;
    int Depth;
    SymTab* sTable;
    expr* E1= nullptr;
    string tok;
    expr* E2= nullptr;
    void printRel();
    void relScan();
};

class EQ     {
    // equal==rel | equal != rel | rel
public:
    EQ(linked_list* list, SymTab* T, int D);
    linked_list* LIST;
    int Depth;
    SymTab* sTable;
    EQ* E1= nullptr;
    string Sym;
    rel* R1= nullptr;
    void typeCheck();
    void printEq();
    void EqScan();
};

class andexpr   {
    // andexpr && equal | equal
public:
    andexpr(linked_list* list, SymTab* T, int D);
    linked_list* LIST;
    int Depth;
    SymTab* sTable;
    andexpr* AE1= nullptr;
    string* AND;
    EQ* E1= nullptr;
    void printAndExpr();
    void andScan();
};
/*
class assign{
    //id = allepr
public:
    assign(linked_list* list, SymTab* T, int D);
    linked_list* LIST;
    int Depth;
    SymTab* sTable;
    allexpr* allExpression;
    string ID;
    void printAssign();
    void assignScan();
};

*/

#endif //INC_125PROJ_MISCCLASS_H
