#ifndef FUNC_H
#define FUNC_H

#include "Struct.h"
#include "stack.h"
#include "Readfile.h"
#include "SizeFile.h"

#define DEF_CMD(name, str, num, code) name = num,

enum type
{
    NUM = 1,
    VAR = 2,
    OP  = 3,
};

enum operation
{
    #include "commands.h"
};

#undef DEF_CMD

struct file_t
{
    int p;
    char* s;
};

Node_t* CreateNode (int type, double value, Node_t* left, Node_t* right);
void NodeDtor (Node_t* node);
void CreateDot (Node_t* node, FILE* file_dot);
void PrintDot (Node_t* node);
void Print (Node_t* node);
void Calculation (Node_t* node, stack_t* stk);
void SkipProb (char** str);
void CreateDotUSER (Node_t* node, FILE* file_dot);
Node_t* ReadDataBase ();
Node_t* Diff (Node_t* node);
Node_t* CopyNode (Node_t* node);


Node_t* GetG (file_t* stk);
Node_t* GetN (file_t* stk);
Node_t* GetE (file_t* stk);
Node_t* GetT (file_t* stk);
Node_t* GetP (file_t* stk);
Node_t* GetS (file_t* stk);

#endif