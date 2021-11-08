#include <iostream>
#include "Lexer.h"
#include "Parser.h"
using namespace std;
int main() {

    Lexer* myLexer = new Lexer();
   /* while(true){
        Token tok = myLexer->getNextToken();
        if(tok.data=="EOF"){break;}
        else{cout<<tok.Class<<" "<<tok.data<<endl;}
    }*/
    Parser* myParser = new Parser(myLexer);
    myParser->PrintTree();
    delete myParser;
    delete myLexer;
    return 0;
}
