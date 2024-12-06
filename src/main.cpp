#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Differentiator_func.h"

int main ()
{
    Tree_t tree = {};
    TreeCtor (&tree, FILE_EXPRESSION);

    CreateTex (&tree);
    Differentiator (&tree);
    PutAnswerTex (&tree);

    TreeDtor (&tree);
}