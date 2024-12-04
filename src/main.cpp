#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Differentiator_func.h"

int main ()
{
    Tree_t tree = {};
    TreeCtor (&tree, file_expression);

    tree.expression_diff = Diff (tree.expression, tree.output);
    CreateTex (&tree);
    Calculation (tree.expression_diff);
    PutAnswerTex (&tree);

    TreeDtor (&tree);
}