#include <stdio.h>
#include <stdlib.h>

#include "Differentiator_func.h"

int main ()
{
    Node_t* node = ReadDataBase ();

    PrintDot (node);
    
    // Print (node);
    // putchar ('\n');

    stack_t stk = {};
    StackCtor (&stk, 10);
    Calculation (node, &stk);
    Node_t* a = NULL;
    StackPop (&stk, &a);
    printf ("res = %lf\n", a->value);
    FreeStack (&stk);

    NodeDtor (node);
}