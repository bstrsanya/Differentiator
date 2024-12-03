#ifndef FUNC_H
#define FUNC_H

#include "Struct.h"
#include "Readfile.h"
#include "SizeFile.h"

enum type
{
    NUM = 1,
    VAR = 2,
    OP  = 3,
    MATH_CONST = 4
};

struct file_t
{
    int p;
    char* s;
};

Node_t* CreateNode (int type, double value, Node_t* left, Node_t* right);
void NodeDtor (Node_t* node);
void CreateDot (Node_t* node, FILE* file_dot);
void PrintDot (Node_t* node);
void Print (Node_t* node, FILE* file);
void Calculation (Node_t* node);
void SkipProb (char** str);
void CreateDotUSER (Node_t* node, FILE* file_dot);
void ReadDataBase (Tree_t* tree);
Node_t* Diff (Node_t* node, FILE* file);
Node_t* CopyNode (Node_t* node);
void TreeCtor (Tree_t* tree, const char* name_file);
void TreeDtor (Tree_t* tree);

Node_t* GetG (file_t* stk);
Node_t* GetN (file_t* stk);
Node_t* GetE (file_t* stk);
Node_t* GetT (file_t* stk);
Node_t* GetP (file_t* stk);
Node_t* GetS (file_t* stk);

#endif