//
// Created by Jared Tuck on 10/24/2021.
//
#ifndef INC_125PROJ_STMT_H
#define INC_125PROJ_STMT_H
#include "Parser.h"
#include "Linked_list.h"
#include "MiscClass.h"
#include <iostream>
using namespace std;

class Stmt;
class Stmt1{
    //handles "id = allexpr;" (Assign)
public:
    Stmt1(linked_list* list, SymTab* T, int D);
    int Depth;
    SymTab* sTable;
    string ID;
    allexpr* allExpression; //pointer to object
    linked_list* LIST; //pointer to object
    void ScanCls1();
    void printStmt1();
    void typeCheck();
};

////////////////////////////////////////////////////////////////////
class Stmt2 {
    // handles "if (allexpr) stmt"
public:
    Stmt2(linked_list* list, SymTab* T, int D);
    int Depth;
    SymTab* sTable;
    string IF = "IF";
    allexpr* allExpression;
    Stmt* statement;
    linked_list* LIST;
    void ScanCls2();
    void printStmt2();
};

///////////////////////////////////////////////////////////////////
class Stmt3{
    //handles "if (allexpr) stmt else stmt"
public:
    Stmt3(linked_list* list, SymTab* T, int D);
    int Depth;
    SymTab* sTable;
    allexpr* allExpression;
    string IF = "IF";
    Stmt* S1;
    string ELSE = "ELSE";
    Stmt* S2;
    linked_list* LIST;
    void ScanCls3();
    void printStmt3();
};

////////////////////////////////////////////////////////////////////
class Stmt4{
    //handles "while (allexpr) stmt"
public:
    Stmt4(linked_list* list, SymTab* T, int D);
    int Depth;
    SymTab* sTable;
    string While = "WHILE";
    Stmt* statement;
    allexpr* allExpression;
    linked_list* LIST;
    void printStmt4();
    void ScanCls4();
};

class Stmt5{
    //handles "do stmt while(allexpr);"
public:
    Stmt5(linked_list* list, SymTab* T, int D);
    linked_list* LIST;
    int Depth;
    SymTab* sTable;
    string DO;
    Stmt* S1;
    string WHILE;
    allexpr* allExpression;
    void ScanCls5();
    void printStmt5();
};

//////////////////////////////////////////////////////////////////////
class Stmt6{
    //handles "for(assign allexpr; incdecexpr) stmt"
public:
    Stmt6(linked_list* list, SymTab* T, int D);
    linked_list* LIST;
    int Depth;
    SymTab* sTable;
    string FOR;
    Stmt1* A1;
    incdecexpr* IncD;
    allexpr* allExpression;
    Stmt* S1;
    void ScanCls6();
    void printStmt6();
};
#endif //INC_125PROJ_STMT_H
