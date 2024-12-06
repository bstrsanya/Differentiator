#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "DSLmath.h"
#include "Differentiator_func.h"

const char* const phrases[] = {
    "Очевидно, что",
    "На первой лекции было",
    "Каждый уважающий себе человек знает, что"
};

void AddTex (Node_t* node, FILE* file)
{
    RandomStr (file);
    fprintf (file, "\n\\begin{equation}\n\t\\left(");
    
    Print (node->left, file); 
    fprintf (file, " + "); 
    Print (node->right, file); 

    fprintf (file, "\\right)'=");

    fprintf (file, "\\left("); 
    Print (node->left, file); 
    fprintf (file, "\\right)'"); 

    fprintf (file, " + "); 

    fprintf (file, "\\left("); 
    Print (node->right, file); 
    fprintf (file, "\\right)'"); 

    fprintf (file, "\n\\end{equation}\n");
}

void SubTex (Node_t* node, FILE* file)
{
    RandomStr (file);
    fprintf (file, "\n\\begin{equation}\n\t\\left(");
    
    Print (node->left, file); 
    fprintf (file, " - "); 
    Print (node->right, file); 

    fprintf (file, "\\right)'=");

    fprintf (file, "\\left("); 
    Print (node->left, file); 
    fprintf (file, "\\right)'"); 

    fprintf (file, " - "); 

    fprintf (file, "\\left("); 
    Print (node->right, file); 
    fprintf (file, "\\right)'"); 
    
    fprintf (file, "\n\\end{equation}\n");
}

void MulTex (Node_t* node, FILE* file)
{
    RandomStr (file);
    fprintf (file, "\n\\begin{equation}\n\t\\left(");

    Print (node->left, file);
    fprintf (file, " \\cdot ");
    Print (node->right, file);

    fprintf (file, "\\right)'=");

    fprintf (file, "\\left(");
    Print (node->left, file);
    fprintf (file, "\\right)' \\cdot \\left(");
    Print (node->right, file);
    fprintf (file, "\\right) + \\left(");
    Print (node->left, file);
    fprintf (file, "\\right) \\cdot \\left(");
    Print (node->right, file);
    fprintf (file, "\\right)'\n\\end{equation}\n");    
}

void DivTex (Node_t* node, FILE* file)
{
    RandomStr (file);
    fprintf (file, "\n\\begin{equation}\n\t\\left(");

    fprintf (file, "\\frac{");
    Print (node->left, file);
    fprintf (file, "}{");
    Print (node->right, file);
    fprintf (file, "}\\right)'=");

    fprintf (file, "\\frac{\\left(");
    Print (node->left, file);
    fprintf (file, "\\right)' \\cdot \\left(");
    Print (node->right, file);
    fprintf (file, "\\right) - \\left(");
    Print (node->left, file);
    fprintf (file, "\\right) \\cdot \\left(");
    Print (node->right, file);
    fprintf (file, "\\right)'}{ \\left(");
    Print (node->right, file);
    fprintf (file, "\\right)^2}");
    fprintf (file, "\n\\end{equation}\n");    
}

void CosTex (Node_t* node, FILE* file)
{
    RandomStr (file);
    fprintf (file, "\n\\begin{equation}\n\t\\left(");
    
    fprintf (file, "\\cos{\\left(");
    Print (node->right, file);
    fprintf (file, "\\right)}\\right)'= (-1) \\cdot \\left(");

    Print (node->right, file);
    fprintf (file, "\\right)' \\cdot \\sin{");
    Print (node->right, file);
    fprintf (file, "}\n\\end{equation}\n");
}

void SinTex (Node_t* node, FILE* file)
{    
    RandomStr (file);
    fprintf (file, "\n\\begin{equation}\n\t\\left(");
    
    fprintf (file, "\\sin{\\left(");
    Print (node->right, file);
    fprintf (file, "\\right)}\\right)'= \\left(");

    Print (node->right, file);
    fprintf (file, "\\right)' \\cdot \\cos{");
    Print (node->right, file);
    fprintf (file, "}\n\\end{equation}\n");
}

void LnTex (Node_t* node, FILE* file)
{
    RandomStr (file);
    fprintf (file, "\n\\begin{equation}\n\t\\left(");

    fprintf (file, "\\ln{");
    Print (node->right, file);
    fprintf (file, "}\\right)' = \\frac{1}{");

    Print (node->right, file);
    fprintf (file, "} \\cdot \\left(");
    Print (node->right, file);
    fprintf (file, "\\right)'");
    fprintf (file, "\n\\end{equation}\n"); 
}

void DegTex (Node_t* node, FILE* file)
{
    if (node->right->type == NUM)
    {
        RandomStr (file);
        fprintf (file, "\n\\begin{equation}\n\t\\left(");

        fprintf (file, "\\left(");
        Print (node->left, file);
        fprintf (file, "\\right)^{");
        Print (node->right, file);
        fprintf (file, "}\\right)' = %g \\cdot \\left(", node->right->value);

        Print (node->left, file);
        fprintf (file, "\\right)^{%g} \\cdot \\left(", node->right->value - 1);
        Print (node->left, file);
        fprintf (file, "\\right)'");
        fprintf (file, "\n\\end{equation}\n"); 
    }
    else if (node->left->type == NUM)
    {
        RandomStr (file);
        fprintf (file, "\n\\begin{equation}\n\t\\left(");

        fprintf (file, "\\left(");
        Print (node->left, file);
        fprintf (file, "\\right)^{");
        Print (node->right, file);
        fprintf (file, "}\\right)' = ");

        Print (node->left, file);
        fprintf (file, "^{");
        Print (node->right, file);
        fprintf (file, "}\\cdot \\ln{%g} \\cdot \\left(", node->left->value);
        Print (node->right, file);
        fprintf (file, "\\right)'");
        fprintf (file, "\n\\end{equation}\n"); 
    }
    else
    {
        RandomStr (file);
        fprintf (file, "\n\\begin{equation}\n\t\\left(");

        fprintf (file, "\\left(");
        Print (node->left, file);
        fprintf (file, "\\right)^{");
        Print (node->right, file);
        fprintf (file, "}\\right)' = ");  

        Print (node->left, file);
        fprintf (file, "^{");
        Print (node->right, file);
        fprintf (file, "} \\cdot \\left(\\left(");
        Print (node->right, file);
        fprintf (file, "\\right)' \\cdot \\ln{");
        Print (node->left, file);
        fprintf (file, "} + \\frac{");
        Print (node->right, file);
        fprintf (file, "}{");
        Print (node->left, file);
        fprintf (file, "} \\cdot \\left(");
        Print (node->left, file);
        fprintf (file, "\\right)'\\right)");
        fprintf (file, "\n\\end{equation}\n"); 
    }
}

void NumTex (Node_t* node, FILE* file)
{
    RandomStr (file);
    fprintf (file, "\n\\begin{equation}\n\t");
    fprintf (file, "%g' = %g", node->value, (double) 0);
    fprintf (file, "\n\\end{equation}\n");
}

void MathConstTex (Node_t* node, FILE* file)
{
    RandomStr (file);
    fprintf (file, "\n\\begin{equation}\n\t");
    fprintf (file, "%c' = 0", (int) node->value); 
    fprintf (file, "\n\\end{equation}\n");
}

void VarTex (Node_t* node, FILE* file)
{
    RandomStr (file);
    fprintf (file, "\n\\begin{equation}\n\t");
    fprintf (file, "\%c' = 1", (int) node->value);
    fprintf (file, "\n\\end{equation}\n");  
}

void RandomStr (FILE* file)
{
    int n_str = rand() % NUM_PHRASES;
    fprintf (file, "\\text{%s}", phrases[n_str]);
}