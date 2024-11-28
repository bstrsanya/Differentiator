#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Differentiator_func.h"

int main ()
{
    Node_t* value = GetG ();
    PrintDot (value);

    stack_t stk = {};
    StackCtor (&stk, 10);
    Calculation (value, &stk);
    Node_t* res = NULL;
    StackPop (&stk, &res);
    printf ("res = %lf\n", res->value);

    FreeStack (&stk);

    NodeDtor (value);
}