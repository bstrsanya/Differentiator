#ifndef FUNC_H
#define FUNC_H

#include "Readfile.h"
#include "SizeFile.h"
#include "Struct.h"


const int SIZE_ARRAY = 100;
const int LEN_STR    = 10;
const char FILE_EXPRESSION[] = "test_example.txt";
const char FILE_LATEX[]      = "dif.tex";
const double SMALL    = 10e-8;
const int NUM_COMMAND = 12;
const int NUM_PHRASES = 3;

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
void FindCommand (char* com, type_com* com_type, int* com_value);

// calculation
void Calculation (Node_t* node, int* n);
void CalculationDeg (Node_t* node, int* n);
void CalculationAdd (Node_t* node, int* n);
void CalculationSub (Node_t* node, int* n);
void CalculationMul (Node_t* node, int* n);
int CompareDouble (double argument_1, double argument_2);

// latex
void CreateTex (Tree_t* tree);
void PutAnswerTex (Tree_t* tree);

// graph dump
void CreateDot (Node_t* node, FILE* file_dot);
void PrintDot (Node_t* node);
void CreateDotUSER (Node_t* node, FILE* file_dot);

void AddTex (Node_t* node, FILE* file);
void SubTex (Node_t* node, FILE* file);
void MulTex (Node_t* node, FILE* file);
void DivTex (Node_t* node, FILE* file);
void SinTex (Node_t* node, FILE* file);
void CosTex (Node_t* node, FILE* file);
void LnTex  (Node_t* node, FILE* file);
void DegTex (Node_t* node, FILE* file);
void NumTex (Node_t* node, FILE* file);
void MathConstTex (Node_t* node, FILE* file);
void VarTex (Node_t* node, FILE* file);

void Differentiator (Tree_t* tree);
void SingleArgFunc (Node_t* node, int* n);
void RandomStr (FILE* file);

#endif