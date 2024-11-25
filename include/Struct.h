#ifndef STRUCT_H
#define STRUCT_H

struct Node_t 
{
    int type;
    double value;
    Node_t* left;
    Node_t* right;
};

typedef Node_t* StackElem_t;

#endif