#ifndef FUNC_H
#define FUNC_H

#include "Struct.h"
#include "Readfile.h"
#include "SizeFile.h"

const int SIZE_ARRAY = 100;
const int LEN_STR    = 10;
const char file_expression[] = "primer.txt";
const char file_latex[]      = "dif.tex";

enum type
{
    NUM = 1,
    VAR = 2,
    OP  = 3,
    MATH_CONST = 4
};

#define DEF_CMD(name, str, type, value) name = value,

enum command
{
    #include "commands.h"
};

#undef DEF_CMD

// differentiator
Node_t* NodeCtor (int type, double value, Node_t* left, Node_t* right);
void NodeDtor (Node_t* node);
void TreeCtor (Tree_t* tree, const char* name_file);
void TreeDtor (Tree_t* tree);

void Print (Node_t* node, FILE* file);
Node_t* Diff (Node_t* node, FILE* file);
Node_t* CopyNode (Node_t* node);

// parser
void ReadDataBase (Tree_t* tree);
Node_t** CreateTokens (char* str);
Node_t* GetG (int* pointer, Node_t** array);
Node_t* GetN (int* pointer, Node_t** array);
Node_t* GetE (int* pointer, Node_t** array);
Node_t* GetT (int* pointer, Node_t** array);
Node_t* GetP (int* pointer, Node_t** array);
Node_t* GetS (int* pointer, Node_t** array);
void FindCommand (char* com, int* com_type, int* com_value);

// calculation
void Calculation (Node_t* node);
void CalculationDeg (Node_t* node);
void CalculationAdd (Node_t* node);
void CalculationSub (Node_t* node);
void CalculationMul (Node_t* node);

// latex
void CreateTex (Tree_t* tree);
void PutAnswerTex (Tree_t* tree);

// graph dump
void CreateDot (Node_t* node, FILE* file_dot);
void PrintDot (Node_t* node);
void CreateDotUSER (Node_t* node, FILE* file_dot);

#endif