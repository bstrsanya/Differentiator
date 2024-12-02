#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Differentiator_func.h"

int main ()
{
    Tree_t tree = {};
    TreeCtor (&tree, "primer.txt");
    PrintDot (tree.expression);
    
    fprintf (tree.output, "\\begin{task}{1}\n\t\\[\\left("); Print (tree.expression, tree.output); fprintf (tree.output, "\\right)'\\]\n\\end{task}\n\n\\begin{solution}\n");
    tree.expression_diff = Diff (tree.expression, tree.output);

    Calculation (tree.expression_diff);
    fprintf (tree.output, "\\text{After a few adjustments:}\n");
    fprintf (tree.output, "\\["); Print (tree.expression_diff, tree.output); fprintf (tree.output, "\\]\n");
    fprintf (tree.output, "\\end{solution}\n");
    PrintDot (tree.expression_diff);

    TreeDtor (&tree);
}