#ifndef STRUCT_H
#define STRUCT_H

enum type_com
{
    NUM = 1,
    VAR = 2,
    OP  = 3,
    MATH_CONST = 4
};

struct Node_t 
{
    type_com type;
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
    Node_t** array;
};

enum command
{
    F_COS   = 99,
    F_SIN   = 115,
    F_LN    = 108,
    F_ADD   = 43,
    F_SUB   = 45,
    F_MUL   = 42,
    F_DIV   = 47,
    F_DEG   = 94,
    F_VAR   = 120,
    F_OPEN  = 40,
    F_CLOSE = 41,
    F_E     = 101
};

struct Command_t
{
    const char* name;
    command n_com;
    type_com t_com;
};




// typedef Node_t* StackElem_t;

#endif