#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Differentiator_func.h"

int main ()
{
    Tree_t tree = {};

    TreeCtor (&tree, FILE_EXPRESSION);
    PrintDot (tree.expression, "initial_express");

    CreateTex (&tree);
    Differentiator (&tree);
    PrintDot (tree.expression_diff, "diff_express");
    PutAnswerTex (&tree);
    TreeDtor (&tree);
}