//
// Created by Jared Tuck on 10/24/2021.
//

#ifndef INC_125PROJ_SYMTAB_H
#define INC_125PROJ_SYMTAB_H
#include <iostream>
#include "linked_list.h"
#include "string"
using namespace std;

class SymTab{
public:
    SymTab();
    SymTab(SymTab* T);
    SymTab* prev;
    linked_list* S;
    int SZ=0;
    void push(string ID, string Type);
    bool inTable(string I);
    std::string findType(string I);
};

#endif //INC_125PROJ_SYMTAB_H
