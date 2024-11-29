#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Differentiator_func.h"

Node_t* ReadDataBase ()
{
    FILE* file = fopen ("primer.txt", "rb");
    size_t size = 0;

    file_t* stk = (file_t*) calloc (1, sizeof (file_t));
    assert (stk);
    stk->s = ReadFile (file, &size);
    stk->p = 0;
    fclose (file);
    
    Node_t* value = GetG (stk);
    
    free (stk->s);
    free (stk);
    
    return value;
}

Node_t* GetG (file_t* stk)
{
    assert (stk);

    Node_t* value = GetE (stk);
    if (stk->s[stk->p] != '$')
        assert (0);
    stk->p++;
    return value;
}

Node_t* GetN (file_t* stk)
{
    assert (stk);

    int value = 0;
    if (stk->s[stk->p] == 'x')
    {
        stk->p++;
        return CreateNode (VAR, 120, NULL, NULL);
    }

    while (('0' <= stk->s[stk->p]) && (stk->s[stk->p] <= '9'))
    {
        value = value * 10 + stk->s[stk->p] - '0';
        stk->p++;
    }

    return CreateNode (NUM, value, NULL, NULL);
}

Node_t* GetE (file_t* stk)
{
    assert (stk);

    Node_t* value = GetT (stk);
    while (stk->s[stk->p] == '+' || stk->s[stk->p] == '-')
    {
        int op = stk->s[stk->p];
        stk->p++;
        Node_t* value2 = GetT(stk);
        if (op == '+')
            value = CreateNode (OP, 43, value, value2);
        else
            value = CreateNode (OP, 45, value, value2);
    }
    return value;
}

Node_t* GetT (file_t* stk)
{
    assert (stk);

    Node_t* value = GetS (stk);
    while (stk->s[stk->p] == '*' || stk->s[stk->p] == '/')
    {
        int op = stk->s[stk->p];
        stk->p++;
        Node_t* value2 = GetS(stk);
        if (op == '*')
            value = CreateNode (OP, 42, value, value2);
        else
            value = CreateNode (OP, 47, value, value2);
    }
    return value;
}

Node_t* GetS (file_t* stk)
{
    assert (stk);

    Node_t* value = GetP (stk);

    if (stk->s[stk->p] == '^')
    {
        stk->p++;
        Node_t* value2 = GetP (stk);
        value = CreateNode (OP, 94, value, value2);
    }

    return value;
}

Node_t* GetP (file_t* stk)
{
    assert (stk);

    if (stk->s[stk->p] == '(')
    {
        stk->p++;
        Node_t* value = GetE (stk);
        if (stk->s[stk->p] != ')')
            assert (0);
        stk->p++;
        return value;
    }
    else if (stk->s[stk->p] == 'c')
    {
        stk->p++;
        Node_t* value = GetP (stk);
        return CreateNode (OP, 99, NULL, value);
    }
    else if (stk->s[stk->p] == 's')
    {
        stk->p++;
        Node_t* value = GetP (stk);
        return CreateNode (OP, 115, NULL, value);
    }
    else
        return GetN (stk);
}