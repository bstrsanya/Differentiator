#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Differentiator_func.h"

int main ()
{
    Node_t* value = ReadDataBase ();
    PrintDot (value);

    Node_t* diff = Diff (value);
    PrintDot (diff);
    // Print (diff);

    NodeDtor (diff);

    NodeDtor (value);
}