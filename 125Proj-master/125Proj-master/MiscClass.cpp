//
// Created by Jared Tuck on 10/24/2021.
//
#include <string>
#include <iostream>
#include "MiscClass.h"
using namespace std;

//allexpr Functions
allexpr::allexpr(linked_list* list, SymTab* T, int D)
{   cout<<" ALLEXPR_CALLED "<<endl;
    Token* temp = list->head;
    while(temp){cout<<temp->get_class()<<" ";temp=temp->next;}
    cout<<endl;
    sTable = T, Depth = D; LIST = list;
    allexpScan();
}

void allexpr::ErrorCheck(){
    Token* temp = LIST->head;
    string type;
    bool TypeFound=false;
    while(temp){
        if(temp->get_class()=="ID"){type=sTable->findType(temp->get_data());TypeFound=true;break;}
        if(temp->get_class()=="NUM"){type="int"; TypeFound=true;break;}
        if(temp->get_class()=="REAL"){type="float"; TypeFound=true;break;}
        if(temp->get_class()=="FALSE"||temp->get_class()=="TRUE"){type="bool"; TypeFound=true;break;}
    temp=temp->next;
    }
    string data;
    while(temp){
        data = temp->get_class();
        if(type=="int"){if(data=="REAL"||data=="float"||data=="FALSE"||data=="TRUE"){
                        cout<<"ERROR: Incompatible arguement ";
                        cout<<temp->get_data()<<" in expression on line "<<temp->get_LN();}
        }else
        if(type=="float"){if(data=="int"||data=="NUM"||data=="FALSE"||data=="TRUE"){
                        cout<<"ERROR: Incompatible arguement ";
                        cout<<temp->get_data()<<" in expression on line "<<temp->get_LN();}
        }else
        if(type=="bool"){if(data=="REAL"||data=="float"||data=="int"||data=="NUM"){
                        cout<<"ERROR: Incompatible arguement ";
                        cout<<temp->get_data()<<" in expression on line "<<temp->get_LN();}
           if(data=="GE"||data=="LE"||data=="<"||data==">"||data=="+"||data=="-"||data=="*"||data=="/"){
                        cout<<"ERROR: Incompatible arguement ";
                        cout<<temp->get_data()<<" in expression on line "<<temp->get_LN();}
        } temp=temp->next;
    }
}

void allexpr::allexpScan()
//allexpr || andexpr | andexpr
{
    Token *temp = LIST -> head;
    bool inside = false;
    int rp = 0, lp = 0, rb = 0, lb = 0, pos = 0;
    if(LIST->listSize()==1){AE2 = new andexpr(LIST, sTable, Depth);}else {
        while (temp) {
            if (temp->get_data() == "(") rp++;
            if (temp->get_data() == ")") lp++;
            if (temp->get_data() == "{") rb++;
            if (temp->get_data() == "}") lb++;

            if ((rp != lp) || (rb != lb)) inside = true;
            else inside = false;

            if (!inside && temp->get_class() == "OR") {
                linked_list *temp3 = LIST->split_set(0, pos - 1);
                linked_list *temp4 = LIST->split_set(1, LIST->listSize() - 1);
                AE1 = new allexpr(temp3, sTable, Depth );
                AE2 = new andexpr(temp4, sTable, Depth );
                break;
            }
            pos++;
            temp = temp->next;
            if (!temp){ AE2 = new andexpr(LIST, sTable, Depth);}
        }
    }
}
void allexpr::printAllexpr(){
    if(AE1){ AE1->printAllexpr();}
    AE2->printAndExpr();
}

incdecexpr::incdecexpr(linked_list* list, SymTab* T, int D)
{   cout<<" INCDEC_EXPRES_CALLED "<<endl;
    Token* temp = list->head;
    while(temp){cout<<temp->get_class()<<" ";temp=temp->next;}
    cout<<endl;
    sTable = T; Depth = D;LIST = list;
    temp = LIST -> head;
    ID = temp ->get_data();
    if(!sTable->inTable(ID)){cout<<"ERROR on line "<<temp->get_LN()<<endl;}
    temp = temp -> next;
    s1 = temp->get_data();
    s2 = temp ->next ->get_data();
}
void incdecexpr::printIncDecEx() {
    for(int i=0; i<=Depth; i++){cout<<"| "; }
    cout<<"+--Token "<<ID<<endl;
    for(int i=0; i<=Depth; i++){cout<<"| "; }
    cout<<"+--Token "<<s1<<endl;
    for(int i=0; i<=Depth; i++){cout<<"| "; }
    cout<<"+--Token "<<s2<<endl;
}
//factor functions
factor::factor(linked_list* list, SymTab* T, int D)
{   cout<<" FACTOR CALLED "<<endl;
    Token* temp = list->head;
    while(temp){cout<<temp->get_class()<<" ";temp=temp->next;}
    cout<<endl;
    sTable = T, Depth = D;LIST = list;
    scan();
}
void factor::scan()
//(allexpr) | incdecexpr | id | num | real | true | false
{
    Token* temp = LIST -> head;
    if(temp -> get_data() == "(")
    {    linked_list* temp2 = LIST -> split_set(1, LIST->listSize()-2);
        allExpression = new allexpr(temp2, sTable, Depth);
    }
    else if(LIST ->listSize()>1){IncDec = new incdecexpr(LIST, sTable, Depth); }
    else{
        tok = temp->get_data();
        if(temp->get_class()=="ID"){
            if(!sTable->inTable(tok)){
                cout<<"SCOPE ERROR on line "<<temp->get_LN()<<", ID '"<<temp->get_data()<<"' is UN-INITILIZED."<<endl; exit(1);}}
        cout<<"Found Terminal "<<tok<<endl;
    }
}
void factor::printFact(){
       if(allExpression){allExpression->allexpScan();}else
       if(IncDec){IncDec->printIncDecEx();}else
       {for(int i=0; i<=Depth; i++){cout<<"| "; }
           cout<<"+--Token "<<tok<<endl;}
}
//Term Function
term::term(linked_list* list, SymTab* T, int D)
{   cout<<" TERM_CALLED "<<endl;
    Token* temp = list->head;
    while(temp){cout<<temp->get_class()<<" ";temp=temp->next;}
    cout<<endl;
    sTable = T, Depth = D; LIST = list;
    termScan();
}
void term::termScan(){
//term * factor | term / factor | factor
    Token* temp = LIST -> head;
    bool inside;
    int rp = 0, lp = 0, rb = 0, lb = 0, pos = 0;
    while(temp){
        if (temp -> get_data() == "(")   rp++;
        if (temp ->get_data() == ")")   lp++;
        if (temp ->get_data() == "{")   rb++;
        if (temp ->get_data() == "}")   lb++;

        if( (rp != lp) || (rb != lb))   inside = true;
        else inside = false;
        tok = temp ->get_data();
        bool symfound = false;
        if(tok == "*") symfound = true;
        if(tok == "/") symfound = true;

        if(!inside && symfound){
            linked_list* temp3 = LIST -> split_set(0, pos - 1);
            linked_list* temp4 = LIST -> split_set(1, LIST->listSize() - 1);
            T1 = new term(temp3, sTable, Depth + 1);
            F1 = new factor(temp4, sTable, Depth + 1);
            break;
        }
        pos++;temp=temp->next;
        if(!temp){F1 = new factor(LIST, sTable, Depth);}
    }
}
void term::printTerm() {
    if(T1) {
        for (int i = 0; i <= Depth; i++) { cout << "| "; }
        cout << "+--Token " << tok << endl;
        T1->printTerm();
    }
    F1->printFact();
}

//Expr Function
expr::expr(linked_list* list, SymTab* T, int D)
{   cout<<" EXPR_CALLED "<<endl;
    Token* temp = list->head;
    while(temp){cout<<temp->get_class()<<" ";temp=temp->next;}
    cout<<endl;
    sTable = T, Depth = D;LIST = list;
    exprScan();}

void expr::exprScan(){
// expr + term | expr - term | term
    Token *temp = LIST -> head;
    bool inside;
    int rp = 0, lp = 0, rb = 0, lb = 0, pos = 0;
    while(temp){
        if (temp -> get_data() == "(")   rp++;
        if (temp ->get_data() == ")")   lp++;
        if (temp ->get_data() == "{")   rb++;
        if (temp ->get_data() == "}")   lb++;

        if( (rp != lp) || (rb != lb))   inside = true;
        else inside = false;
        tok = temp ->get_data();
        bool symfound = false;
        if(tok == "+") symfound = true;
        if(tok == "-") symfound = true;

        if(!inside && symfound) {
            linked_list* temp3 = LIST -> split_set(0, pos - 1);
            linked_list* temp4 = LIST -> split_set(1, LIST->listSize() - 1);
            E1 = new expr(temp3, sTable, Depth+1);
            T1 = new term(temp4, sTable, Depth+1);
            break;        }
        pos++;
        temp=temp->next;
        if(!temp){T1 = new term(LIST, sTable, Depth);}
    }
}
void expr::printExpr(){
    if(E1){

        for(int i=0; i<=Depth; i++){cout<<"| "; }
        cout<<"+--Token "<<tok<<endl;
        E1->printExpr();
        T1->printTerm();
    }else{T1->printTerm();}

}

//rel Function
rel::rel(linked_list* list, SymTab* T, int D)
{   cout<<" REL_CALLED "<<endl;
    Token* temp = list->head;
    while(temp){cout<<temp->get_class()<<" ";temp=temp->next;}
    cout<<endl;
    sTable = T, Depth = D; LIST = list;
    relScan();
}
void rel::relScan(){
//expr<expr | expr<=expr | expr>expr | expr>=expr | expr
    Token *temp = LIST -> head;
    bool inside;
    int rp = 0, lp = 0, rb = 0, lb = 0, pos = 0;
    while(temp){
        if (temp -> get_data() == "("){rp++;}
        if (temp ->get_data() == ")") {lp++;}
        if (temp ->get_data() == "{") {rb++;}
        if (temp ->get_data() == "}") {lb++;}

        if( (rp != lp) || (rb != lb))   inside = true;
        else inside = false;
        tok = temp ->get_data();
        bool symfound = false;
        if(tok == "<") symfound = true;
        if(tok == ">") symfound = true;
        if(tok == "<=") symfound = true;
        if(tok == ">=") symfound = true;

        if(!inside && symfound){
            linked_list* temp3 = LIST -> split_set(0, pos - 1);
            linked_list* temp4 = LIST -> split_set(1, LIST->listSize() - 1);
            E1 = new expr(temp3, sTable, Depth+1);
            E2 = new expr(temp4, sTable, Depth+1);
            break;
        }
        pos++;
        temp=temp->next;
        if(!temp){ E2 = new expr(LIST, sTable, Depth);}
    }
}
void rel::printRel() {
    if(E1){
        for(int i=0; i<=Depth; i++){cout<<"| "; }
        cout<<"+--Token "<<tok<<endl;
        E1->printExpr();
        E2->printExpr();
    }else{E2->printExpr();}
}

EQ::EQ(linked_list* list, SymTab* T, int D)
{   cout<<" EQ_CALLED "<<endl;
    Token* temp = list->head;
    while(temp){cout<<temp->get_class()<<" ";temp=temp->next;}
    cout<<endl;
    sTable = T, Depth = D; LIST = list;
    EqScan();
}
void EQ::EqScan(){
// equal==rel | equal != rel | rel
    typeCheck();
    Token *temp = LIST -> head;
    bool inside;
    int rp = 0, lp = 0, rb = 0, lb = 0, pos = 0;
   /* cout<<endl;
    while(temp){cout<<temp->get_class()<<" ";temp=temp->next;}
    cout<<endl;
    temp=LIST->head;*/
    while(temp){
        if (temp ->get_data() == "(")   rp++;
        if (temp ->get_data() == ")")   lp++;
        if (temp ->get_data() == "{")   rb++;
        if (temp ->get_data() == "}")   lb++;

        if( (rp != lp) || (rb != lb))   inside = true;
        else inside = false;
        string tok = temp ->get_class();
        bool symfound = false;
        if(tok == "EQ") symfound = true;
        if(tok == "NE") symfound = true;

        if(!inside && symfound)
        {   Sym = temp->get_data();
            linked_list* temp3 = LIST -> split_set(0, pos - 1);
            linked_list* temp4 = LIST -> split_set(1, LIST->listSize() - 1);
            E1 = new EQ(temp3, sTable, Depth );
            R1 = new rel(temp4, sTable, Depth);
            break;
        }
        pos++;
        temp=temp->next;
        if(!temp){ R1 = new rel(LIST, sTable, Depth);}
    }
}
void EQ::typeCheck() {
    Token* temp = LIST->head;
    string TYPE;
    bool TypeFound = false;
    while(temp){
        if(temp->get_class()=="ID" && !sTable->inTable(temp->get_data())){
        cout<<"SCOPE ERROR on line "<<temp->get_LN()<<", ID '"<<temp->get_data()<<"' is UN-INITILIZED."<<endl; exit(1);}
        if(!TypeFound && temp->get_class() == "ID") {
            TypeFound=true;
            TYPE = sTable->findType(temp->get_data());
        }else
        if(temp->get_class() == "ID"){
            if(sTable->findType(temp->get_data()) != TYPE){
                cout<<"TYPE ERROR on line "<<temp->get_LN()<<", ID '"<<temp->get_data()<<"' is not compatible with type "<<TYPE<<endl; exit(1);
            }
        }
        temp=temp->next;
    }

}

void EQ::printEq(){
    if(E1){
        for(int i=0; i<=Depth; i++){cout<<"| "; }
        cout<<"+--Token "<<Sym<<endl;
        E1->printEq();
    }
    R1->printRel();
}
andexpr::andexpr(linked_list* list, SymTab* T, int D)
{   cout<<" ANDEXPR_CALLED "<<endl;
    Token* temp = list->head;
    while(temp){cout<<temp->get_class()<<" ";temp=temp->next;}
    cout<<endl;
    sTable = T, Depth = D;LIST = list;
    andScan();
}

void andexpr::andScan()
// andexpr && equal | equal
{
    Token *temp = LIST -> head;
    bool inside;
    int rp = 0, lp = 0, rb = 0, lb = 0, pos = 0;
    while(temp)
    {
        if (temp -> get_data() == "(")   rp++;
        if (temp ->get_data() == ")")   lp++;
        if (temp ->get_data() == "{")   rb++;
        if (temp ->get_data() == "}")   lb++;

        if( (rp != lp) || (rb != lb))   inside = true;
        else inside = false;
        string tok = temp ->get_class();

        if(!inside && tok == "AND")
        {
            linked_list* temp3 = LIST -> split_set(0, pos - 1);
            linked_list* temp4 = LIST -> split_set(1, LIST->listSize() - 1);
            AE1 = new andexpr(temp3, sTable, Depth);
            E1 = new EQ(temp4, sTable, Depth+1);
            break;
        }
        temp=temp->next;
        pos++;
        if(!temp){ E1 = new EQ(LIST, sTable, Depth+1);}
    }
}
void andexpr::printAndExpr() {
    if(AE1){AE1->printAndExpr();}
    E1->printEq();
}
