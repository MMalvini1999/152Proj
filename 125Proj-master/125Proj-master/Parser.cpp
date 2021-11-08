//
// Created by Jared Tuck on 10/24/2021.
//
#include "Parser.h"
#include <vector>
Parser::Parser(Lexer* L){Lex=L;  Data = Lex->List; P = new Prog(Data);};

void Parser::PrintTree(){P->printProg();};

Prog::Prog(linked_list* LIST){
List = LIST;
List->Print();
ErrorCheck1();
ErrorCheck2();
if(List->tail->get_class() == "EOF"){List->popEnd();}
B= new Block(1, List);
}

void Prog::printProg(){std::cout<<"+--PROGRAM"<<endl; B->printBlock();}

void Prog::ErrorOut(Token* temp){
    cout<<"Error: no applicable grammar for STMT on line ";
    cout<<temp->get_LN()<<" using Token "<<temp->get_data();
    exit(1);
}

void Prog::ErrorCheck1() {
    int LB=0, RB=0, LP=0, RP = 0;
    int LBL=0, RBL=0, LPL=0, RPL = 0;
    Token* temp = List->head;
    while(temp){

        if(temp->get_data()=="{"){if(LB==0){LBL = temp->get_LN();} LB++;}

        if(temp->get_data()=="}"){
            RB++;
            if(LB==RB){LB=0, RB=0;}
            RBL = temp->get_LN();}
        if(temp->get_data()=="("){if(LP==0){LPL = temp->get_LN();} LP++;}
        if(temp->get_data()==")"){
            RP++;
            if(LP==RP){LP=0, RP=0;}
            RPL = temp->get_LN();}
        temp=temp->next;}
    if(LB != RB){
        if(LB>RB){cout<<"ERROR:Non-Matching Bracket Found on Line "<<LBL<<endl;cout<<"flag";exit(1);}
        else{cout<<"ERROR:Non-Matching Bracket Found on Line "<<RBL<<endl;cout<<"flag0";exit(1);}
    }
    if(LP != RP){
        if(LP>RP){cout<<"ERROR:Non-Matching Parenthesis Found on Line "<<LPL<<endl;cout<<"flag1";exit(1);}
        else{cout<<"ERROR:Non-Matching Parenthesis Found on Line "<<RPL<<endl;cout<<"flag2";exit(1);}
    }
}
void Prog::ErrorCheck2() {
    Token* temp = List->head;
    string data;
    while(temp){
        data = temp->get_class();
        if(data=="BASE_TYPE"){
            if(!temp->next){ErrorOut(temp);}
            if(temp->next->get_class() != "ID"){ErrorOut(temp);}
        }
        if(data=="ID" && temp->next->get_class()=="ID"){ErrorOut(temp->next);}

        if(data=="WHILE" || data=="IF"){
            if(!temp->next || temp->next->get_class() != "("){ErrorOut(temp);}
        }
        if(data=="ELSE"||data=="WHILE"||data=="DO"||data=="IF"||data=="FOR"||data=="BASE_TYPE"){
            if(temp->prev->get_class()!="}"&&temp->prev->get_class()!=";"&&temp->prev->get_class()!="{"){ErrorOut(temp);}
        }
        if(data == "ID"|| data == "NUM"||data=="float"){
            if(temp->next->get_class()=="WHILE"){ErrorOut(temp);}
        }
    temp=temp->next;
    }
}
Block::Block(int D, linked_list* List){
    Depth = D; LIST=List; Table = new SymTab();
    Scan4Stmt(LIST->split_set(1, LIST->listSize()-2));
}

Block::Block(int D, linked_list* List, SymTab* T){
    cout<<"Block_Called"<<endl;
    Depth = D;
    LIST=List->split_set(1, List->listSize()-2);
    Table = new SymTab(T);
    Table->prev = T;
    Scan4Stmt(LIST);
}

void Block::StmtFound(linked_list* List, int POS){
    linked_list* B;
    int b =List->listSize();
    if(POS != List->listSize()-1){B = List->split(POS);}else{B=List;}
    B->Print();
    Token* temp= B->head;
    cout<<"STMT passed from Scan4STMT"<<endl;
    while(temp){cout<<temp->get_data()<<" ";temp=temp->next;}
    Stmt* S = new Stmt(B, Table, Depth+1+St.size());
    St.push_back(S);

}
Token* Block::Scan4Decl(linked_list* List){
    Token* temp = List->head;
    bool Ty=false, I=false, S=false;//Type, ID , Semicolon
    string ty, ID, Semi;
    for(int i=0;i<3; i++){
        if(i==0 && temp->get_class()=="BASE_TYPE"){ Ty=true;ty=temp->get_data();}
        if(i==1 && temp->get_class()=="ID"){ I=true;ID=temp->get_data();}
        if(i==2 && temp->get_class()==";"){ S=true;};
        temp = temp->next;}
    if(Ty && I && S){
        cout<<"Decl added "<<ty<<" "<<ID<< endl;
        List->pop();List->pop();List->pop();
        Table->push(ID,ty);
    }
    if(Ty && !I){
        cout<<"Error: No matching Grammar for STMT on Line ";
        cout<<List->head->get_LN()<<" beginning with Token ";
        cout<<List->head->get_data();exit(1);}
    return List->head;
}
void Block::Scan4Stmt(linked_list* List){
    Token* temp = List->head;
    int RB=0, LB=0, RP=0, LP=0,pos=0,SZ=0;
    bool B_EQ = true, P_EQ =true,INSIDE=false;
    string Data, HEAD_Data, LOOK;
    cout<<"Passed to Scan4STMT ";
    Token* temp2=List->head;
    while(temp2){cout<<temp2->get_data()<<" ";temp2=temp2->next;}
    while(temp) {
    if(temp->next){LOOK=temp->next->get_class();}else{LOOK=" ";}
    SZ = List->listSize();
    if(pos==0){HEAD_Data=temp->get_class(); if(HEAD_Data=="DO"){LP=0;RP=0;}}
    Data=temp->get_class();
    if(Data=="{"){LB++;} if(Data=="}"){RB++;}
    if(Data=="("){LP++;} if(Data==")"){RP++;}
    if(LB==RB){B_EQ=true;}else{B_EQ=false;}
    if(LP==RP){P_EQ=true;}else{P_EQ=false;}
    if(!B_EQ || !P_EQ){INSIDE =true;}else{INSIDE=false;}
    if(HEAD_Data=="BASE_TYPE"){temp = Scan4Decl(List);pos=0;}else
    if(HEAD_Data=="ID"){if(Data==";"&& !INSIDE){temp=temp->next;StmtFound(List, pos);if(pos==SZ-1){break;}pos=0;}else{temp=temp->next;pos++;}}else
    if(HEAD_Data=="IF"){if((Data=="}"||Data==";")&& LOOK != "ELSE"&&!INSIDE){temp=temp->next;StmtFound(List, pos);if(pos==SZ-1){break;}pos=0;}else{temp=temp->next;pos++;}}else
    if(HEAD_Data=="WHILE"){if((Data==";"&&!INSIDE)||(Data=="}"&&!INSIDE)){temp=temp->next;StmtFound(List, pos);if(pos==SZ-1){break;}pos=0;}else{temp=temp->next;pos++;}}else
    if(HEAD_Data=="DO"){if(Data==";" && P_EQ && temp->prev->get_data()==")"){temp=temp->next;StmtFound(List, pos);if(pos==SZ-1){break;}pos=0;}else{temp=temp->next;pos++;}}else
    if(HEAD_Data=="FOR"){if((Data==";"&&!INSIDE)||(Data=="}"&&!INSIDE)){temp=temp->next;StmtFound(List, pos);if(pos==SZ-1){break;}pos=0;}else{temp=temp->next;pos++;}}else
    if(HEAD_Data=="BREAK"){if(Data==";'"){temp=temp->next;StmtFound(List, pos);if(pos==SZ-1){break;}pos=0;}else{temp=temp->next;pos++;}}else
    if(HEAD_Data=="{"){if(Data=="}"&&!INSIDE){temp=temp->next;StmtFound(List, pos);if(pos==SZ-1){break;}pos=0;}else{temp=temp->next;pos++;}}else
    {pos++;temp=temp->next;}
    if(!temp){
        cout<<"ERROR: No Applicable Grammar for STMT on Line ";
        cout<<List->head->get_LN()<<" Beginning with Token ";
        cout<<List->head->get_data()<<endl;
    }
    }

    /*    Token* temp = List->head;
    //   x assign | if (allexpr) stmt | if (allexpr) stmt else stmt | while (allexpr) stmt
    //    do stmt while (allexpr) ; | for (assign allexpr ; incdecexpr)  stmt  | x break ; | block
    int BR=0, BL = 0;
    bool B_EQ = true;
    int pos = 0;
    cout<<endl;
    while(temp){cout<<temp->get_class()<<" ";temp=temp->next;}
    cout<<endl;
    temp=List->head;
    string LOOK;
    int count=0;
    while(temp){//scan until end of Stmt is found, check for decl at beginning of new search.
        int SZ = List->listSize();
        if(temp->next){LOOK = temp->next->get_class();}else{LOOK="";}
        while(pos==0 && temp->get_class()=="BASE_TYPE"){
            temp = Scan4Decl(List);
            while(temp){cout<<temp->get_class()<<" ";temp=temp->next;}
            cout<<endl;
            temp=List->head;
        }
        //cout<<temp->get_class()<<"  ";
        if(temp->get_class()=="BREAK" && pos==0){
            temp=temp->next->next;//Next tk should be ";" after break, need to error check for.
            linked_list* B = List->split(0);
            Stmt* S = new Stmt(B, Table, Depth);
            St.push_back(S);
            List->pop();//discard ";" after Break.
            if(List->listSize()==0){break;}
        }
        if(temp->get_data()=="}"){BR++;}
        if(temp->get_data()=="{"){BL++;}
        if(BR == BL){B_EQ = true;}else{B_EQ = false;}
        //^^^Determine if inside or outside of Brackets
        if(B_EQ && temp->get_data()=="}" && !temp->next){cout<<"*";temp=temp->next;StmtFound(List, pos);if(pos==SZ-1){break;} pos =0;}
        else//^^^If Tok is a R.Bracket and Brackets match its the end of stmt unless followed by else.
        if(B_EQ && temp->get_data()=="}" && LOOK!="ELSE"){cout<<"**";temp=temp->next;StmtFound(List, pos);if(pos==SZ-1){break;}pos =0;}
        else//^^^If Tok is a R.Bracket and Brackets match its the end of stmt unless followed by else.
        if(B_EQ && temp->get_data()==";" ){cout<<"***";temp=temp->next;StmtFound(List, pos);if(pos==SZ-1){break;}pos=0;}
        else//^^^If Brackets are equal then semi colon signifies end of Stmt
        if(B_EQ && LOOK=="if"){cout<<"****";temp=temp->next;StmtFound(List, pos);if(pos==SZ-1){break;}pos=0;}
        else//^^^If next next is a "if" then it signifies beginning of new stnt  therefore end of stmt found.
        if(B_EQ && LOOK=="do"){cout<<"*****";temp=temp->next;StmtFound(List, pos);if(pos==SZ-1){break;}pos=0;}
        else//^^^If next next is a "do" then it signifies beginning of new stnt  therefore end of stmt found.
        if(B_EQ && LOOK=="for"){cout<<"******";temp=temp->next;StmtFound(List, pos);if(pos==SZ-1){break;}pos=0;}
        else//^^^If next next is a "for" then it signifies beginning of new stmt therefore end of stmt found.
        {pos++;temp=temp->next;}
        //^^^ Nothing Found keep scanning
        }
        //if(List->head && B_EQ){StmtFound(List, List->listSize()-1);}else{/*error state*/}

void Block::printBlock(){
    for(int i=0; i<Depth; i++){cout<<"| "; }
    cout<<"+--Block"<<endl;
    int SZ=St.size();
    for(int i=0;i<SZ;i++){
       // int j=0;while(j<i){cout<<"| ";j++;}
        St[i]->printStmt();
    }
};