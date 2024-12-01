#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Differentiator_func.h"

int main ()
{
    Node_t* value = ReadDataBase ();
    PrintDot (value);

    printf ("\\[\\left("); Print (value); printf ("\\right)'\\]\n");
    Node_t* diff = Diff (value);

    Calculation (diff);
    printf ("\\text{After a few adjustments:}\n");
    printf ("\\["); Print (diff); printf ("\\]\n");
    PrintDot (diff);

    NodeDtor (diff);

    NodeDtor (value);
}