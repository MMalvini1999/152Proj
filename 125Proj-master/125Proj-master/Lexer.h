
#ifndef INC_125PROJ_LEXER_H
#define INC_125PROJ_LEXER_H
#include <string>
#include "Linked_list.h"

using namespace std;

class Lexer {
public:
    linked_list* List;
    Lexer();
    ~Lexer();
    Token* nxtTok;
    Token getNextToken();
    bool isTokenL1(string T, char N);
    bool isTokenL2(string input, char N);
    bool isTokenL3plus(string input, char N);
};

string getLabel(string input);
bool isDigit(char input);
bool isLetter(char input);
bool isChar(char input);

#endif //INC_125PROJ_LEXER_H