#ifndef STRUCT_H
#define STRUCT_H

struct Node_t 
{
    int type;
    double value;
    Node_t* left;
    Node_t* right;
};

struct Tree_t
{
    Node_t* expression;
    Node_t* expression_diff;
    FILE* output;
    FILE* input;
};

typedef Node_t* StackElem_t;

#endif