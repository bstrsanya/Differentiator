#include <stdio.h>

#include "Differentiator_func.h"

void CreateTex (Tree_t* tree)
{
    fprintf (tree->output, "\\begin{task}{1}\n\t\\[f(x)="); 
    Print (tree->expression, tree->output); 
    fprintf (tree->output, ",f'(x)-?\\]\n\\end{task}\n\n\\begin{solution}\n");
}

void PutAnswerTex (Tree_t* tree)
{
    fprintf (tree->output, "\\text{After a few adjustments:}\n");
    fprintf (tree->output, "\\["); 
    Print (tree->expression_diff, tree->output); 
    fprintf (tree->output, "\\]\n");
    fprintf (tree->output, "\\end{solution}\n");
}