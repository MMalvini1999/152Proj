//
// Created by Jared Tuck on 10/24/2021.
//

#ifndef INC_125PROJ_LINKED_LIST_H
#define INC_125PROJ_LINKED_LIST_H
#include <string>
using namespace std;
class Token {
public:
    Token *next;
    Token *prev;
    std::string data;
    std::string Class;
    int LN;
    Token(string data, string Class, int N) : data(data),Class(Class),LN(N), next(nullptr), prev(nullptr){};
    string get_data(){return data;}
    string get_class(){return Class;}
    int get_LN(){return LN;}
};

class linked_list {
public:
    Token *head, *tail;
    linked_list();
    ~linked_list();
    bool isEmpty();
    int listSize();
    void push(string input, string Class, int LN);
    void pop();
    void popEnd();
    linked_list* split(int pos);
    linked_list* split_set(int position_1, int position_2);
    linked_list operator=(linked_list &rhs);
    void Print();
};

#endif //INC_125PROJ_LINKED_LIST_H
