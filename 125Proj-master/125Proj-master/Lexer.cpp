#include <iostream>
#include <fstream>
#include <string>
#include "Lexer.h"
#include "Linked_list.h"
using namespace std;

//////////////////////////////Lexer Functions////////////////////////////////////
Lexer::Lexer(){
    List = new linked_list();
    string fileName, input;
    cout<<"Please enter the .txt file you'd like to parse"<<endl;
    cin>>fileName;
    ifstream myFile;
    int line =0;
    myFile.open(fileName);
    if(myFile.is_open()) {
        line = 1;
        while (getline(myFile, input)) {
            int length = input.length();
            for (int i = 0; i < length; i++) {
                if (input[i] != 32)
                {
                    string T = "";
                    char N;//Character of Next space following potential Token
                    for (int j = i; j < length; j++)
                    {
                        if(j< length-1){N=input[j+1];}else{N=32;}
                        T = T + input[j];
                        //Check specifically for single length terminals. And Errors.
                        if (T.length() == 1) {if (isTokenL1(T,N)) {
                            List->push(T, getLabel(T), line); i=j; j=length;
                            if(List->tail->Class == "ERROR"){
                                List->tail->Class ="Error on line "+to_string(line);
                                myFile.close();
                                j=length;i=length;}
                            }}
                        else
                        if (T.length() == 2) {if (isTokenL2(T,N)) {
                                List->push(T, getLabel(T), line); i=j; j=length;
                             }}
                        else
                        if (T.length() > 2){if(isTokenL3plus(T,N)){
                                List->push(T, getLabel(T), line); i=j; j=length;
                            }}
                    }
                }
            } line++;
        }
    }else{cout<<"file "<<fileName<<" not opened"<<endl;}
    myFile.close();
    List->push("EOF","EOF", line);
    nxtTok = List->head;
}
Lexer::~Lexer() {delete List;}

Token Lexer::getNextToken() {
    Token temp("temp", "temp", 0);
    if (nxtTok) {
        temp.Class = nxtTok->get_class();
        temp.data = nxtTok->get_data();
        nxtTok = nxtTok->next;
    }
    return temp;
}
//Checks a potential token of length 1, and compares to next value following it.
bool Lexer::isTokenL1(string T, char N) {
    //If any unknown character is found isolate and token will be labeled with an error.
    if((T[0]>33 && T[0]<38) || T[0]==39 || T[0]==44||T[0]==58||T[0]==63||T[0]==64){return true;}
    if(T[0]==38 && N!=38){return true;}
    if(T[0]==91||T[0]==93||T[0]==94||T[0]==96||T[0]==126){return true;}
    //cout<<T<<endl;
    //If T is "=" can only be partial token if N is also "=", else T is token.
    if(T[0]==61 && N!=61){return true;}
    if((T[0] == 60 && N != 61)|| (T[0] == 62 && N != 61) || (T[0] == 33 && N!=61)){return true;}
    //If T is "()*+-" return true
    if(T[0]>39 && T[0]<44){return true;}
    //If T is "- / ; { }" return true
    if(T[0]==45||T[0]==47||T[0]==59||T[0]==123||T[0]==125){return true;}
    //If T is a Letter followed by a non letter except "_" return true;
    if(isLetter(T[0]) && isChar(N) && N!=95){return true;}
    //if T is a digit followed by a non digit or not "." return true
    if(isDigit(T[0]) && !isDigit(N) && N!=46){return true;}
    return false;
}

bool Lexer::isTokenL2(string T, char N) {
    string tokens[6] = {"&&", "==", ">=", "<=", "!=", ""}; //tokens of size 2

    for (int i = 0; i < 2; i++) //size is defined above for tokens of size 2
    {
        if (T[1]== 61) //checking if single character is token "="
            return true;

        if (T =="&&" || T =="=="|| T ==">=" || T == "<=" || T =="!=" || T == "||") //if any defined arguments matched phrase, we have a token
            return true;

        if (isDigit(T[0]) && T[1]==46 && !isDigit(N) || T[0]==46 && isDigit(T[1]) && (!isDigit(N))) //Real Num
            return true;

        if (isDigit(T[0]) && (isDigit(T[1])) && isChar(N) && N!=46 && !isDigit(N))  //Num
            return true;

        if (T=="if" && N==40 || T=="if" && N==32) //IF, which is followed by parenthesis
            return true;

        if (T=="do" && N== 123 || T=="do" && N==32) //DO, which is followed by a bracket
            return true;

        if (isLetter(T[0]) &&  N!=95 && !isDigit(N) && !isLetter(N))
            return true;
        else
            return false;
    }

    return false;

}

bool Lexer::isTokenL3plus(string input, char N) {
    
    string contain[9] = {"for", "while", //Has to be followed by " " or "("
                        "int", "bool", "float","else",  //Has to be followed " "
                        "break", "true", "false"}; //Has to be followed by " " ";"

    //Checks for anything that can be followed by  
    //a space or open paranthesis(for, while)
    for(int i = 0; i < 9; i++){
        if(input == contain[i] && isChar(N))
            return true;
    }
    //Checks to see if token is REAL or NUM
    int dec_counter = 0;
    int ret_counter = 0;
    for(int i = 0; i < input.length(); i++){
        if (isChar(N)){
            if(N == 46)
                return false;
            if(input[i] == 46)
                dec_counter++;

            if(!isDigit(input[i]) && input[i] != 46 || dec_counter > 1){
                break;
            }
            else
                ret_counter++;
        }
    }
    if(ret_counter == input.length())
        return true;

    //Returns true if input is an ID
    if(isLetter(input[0]) && !isLetter(N) && !isDigit(N) && N != 95)
        return true;

    return false;
}
//////////////////////////////////Helper Functions////////////////////////////////////////////////////

string getLabel(string input){
    if((input[0]>33 && input[0]<38) || input[0]==39 || input[0]==44){return "ERROR";}
    if(input[0]==58||input[0]==63||input[0]==64||input[0]==91){return "ERROR";}
    if(input[0]==93||input[0]==94||input[0]==96||input[0]==126){return "ERROR";}

    string C[] = {"(",")","*","+","-","/",";","{","}","<","=",">"};

    string E[][2] = {
            {"&&","AND"},{"==","EQ"},{">=","GE"},{"<=","LE"},{"!=","NE"},{"||","OR"},
            {"if","IF"},{"do","DO"},{"for","FOR"},{"true","TRUE"},{"else","ELSE"},{"false","FALSE"},
            {"while","WHILE"},{"break","BREAK"}, {"int","BASE_TYPE"}, {"float","BASE_TYPE"},
            {"bool","BASE_TYPE"}
    };

    for(int i=0;i<12;i++) { if (input == C[i]) { return C[i]; }}

    for (int i = 0; i < 17; i++) { if(input == E[i][0]){ return E[i][1]; } }
    //If Token is
    if(isDigit(input[0])||input[0]==46){
        for(int i=0; i<input.length();i++){
            if(isChar(input[i]) && input[i] != 46){return "ERROR";}
            if(input[i]==46){return"REAL";}}
        return"NUM";}
    //If Token is comprised of only Letters, Digits and Underscore return ID
    if(isLetter(input[0])){
        for(int i=0;i<input.length();i++){if(isChar(input[i]) && input[i] != 95){return "ERROR";}}
        return "ID";}

    return "ERROR";
}
//Checks if Input is a Letter
bool isDigit(char input) {
    if(input>47 && input <58){return true;}
    return false;
}
//Checks if input is a Letter
bool isLetter(char input){
    if(input>64 && input<91){return true;}
    if(input>96 && input<123){return true;}
    return false;
};
//Checks if input is anything other than a Digit or Letter
bool isChar(char input){
    if(input>31 && input<48){return true;}
    if(input>57 && input<65){return true;}
    if(input>90 && input<97){return true;}
    if(input == 96){return true;}
    if(input>122){return true;}
    return false;
}