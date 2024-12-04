#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Differentiator_func.h"

void ReadDataBase (Tree_t* tree)
{
    size_t size = 0;

    char* s = ReadFile (tree->input, &size);

    Node_t** array = CreateTokens (s);

    int pointer = 0;
    Node_t* value = GetG (&pointer, array);
    
    tree->expression = value;
    tree->array = array;
    free (s);
}

Node_t** CreateTokens (char* s)
{
    int t = 0;
    Node_t** array = (Node_t**) calloc (SIZE_ARRAY, sizeof (Node_t*));
    for (int i = 0; i < SIZE_ARRAY; i++)
    {
        array[i] = NodeCtor (0, 0, NULL, NULL);
    }

    int y = 0;

    while (s[t] != '$')
    {
        if (('0' <= s[t]) && (s[t] <= '9'))
        {            
            double d = 0;
            int n = 0;
            sscanf (s + t, "%lf%n", &d, &n);
            t += n;
            array[y]->type = NUM;
            array[y]->value = d;
            y++;
        }
        else if (('a' <= s[t] && s[t] <= 'z') || ('A' <= s[t] && s[t] <= 'Z'))
        {
            char com[LEN_STR] = "";
            int n = 0;
            sscanf (s + t, "%[a-zA-Z]%n", com, &n);
            
            int com_type = 0;
            int com_value = 0;
            FindCommand (com, &com_type, &com_value);

            array[y]->type = com_type;
            array[y]->value = com_value;
            y++;
            t += n;
        }
        else if (s[t] == '(' || s[t] == ')')
        {
            array[y]->type = OP;
            array[y]->value = s[t];
            t++;
            y++;
        }
        else if (s[t] == '+' || s[t] == '-' || s[t] == '*' || s[t] == '/' || s[t] == '^')
        {
            array[y]->type = OP;
            array[y]->value = s[t];
            t++;
            y++;
        }
        else
        {
            printf ("ER\n");
        }

    }
    array[y]->value = '$';
    return array;
}

#define DEF_CMD(name, str, type, value) if (!strcmp(str, com)) {*com_type = type; *com_value = value;}
void FindCommand (char* com, int* com_type, int* com_value)
{
    #include "commands.h"
}
#undef DEF_CMD

Node_t* GetG (int* pointer, Node_t** array)
{
    Node_t* value = GetE (pointer, array);
    if ((int) array[*pointer]->value != '$')
        assert (0);
    (*pointer)++;
    return value;
}

Node_t* GetN (int* pointer, Node_t** array)
{
    (*pointer)++;
    return array[(*pointer)-1];
}

Node_t* GetE (int* pointer, Node_t** array)
{
    Node_t* value = GetT (pointer, array);

    while (array[*pointer]->type == OP && ((int) array[*pointer]->value == F_ADD || (int) array[*pointer]->value == F_SUB))
    {
        int num = *pointer;
        (*pointer)++;
        Node_t* value2 = GetT (pointer, array);
        array[num]->left = value;
        array[num]->right = value2;
        value = array[num];
    }
    return value;
}

Node_t* GetT (int* pointer, Node_t** array)
{
    Node_t* value = GetS (pointer, array);

    while (array[*pointer]->type == OP && ((int) array[*pointer]->value == F_MUL || (int) array[*pointer]->value == F_DIV))
    {
        int num = *pointer;
        (*pointer)++;
        Node_t* value2 = GetS (pointer, array);
        array[num]->left = value;
        array[num]->right = value2;
        value = array[num];
    }

    
    return value;
}

Node_t* GetS (int* pointer, Node_t** array)
{
    Node_t* value = GetP (pointer, array);

    if (array[*pointer]->type == OP && (int) array[*pointer]->value == F_DEG)
    {
        int num = *pointer;
        (*pointer)++;
        Node_t* value2 = GetP (pointer, array);
        array[num]->left = value;
        array[num]->right = value2;
        value = array[num];
    }

    return value;
}

Node_t* GetP (int* pointer, Node_t** array)
{
    if (array[*pointer]->type == OP && (int) array[*pointer]->value == F_OPEN)
    {
        (*pointer)++;
        Node_t* value = GetE (pointer, array);
        if ((int) array[*pointer]->value != F_CLOSE)
            assert (0);
        (*pointer)++;
        return value;
    }
    else if (array[*pointer]->type == OP && (int) array[*pointer]->value == F_COS)
    {
        int num = *pointer;
        (*pointer)++;
        Node_t* value = GetP (pointer, array);
        array[num]->right = value;
        return array[num];
    }
    else if (array[*pointer]->type == OP && (int) array[*pointer]->value == F_SIN)
    {
        int num = *pointer;
        (*pointer)++;
        Node_t* value = GetP (pointer, array);
        array[num]->right = value;
        return array[num];
    }
    else if (array[*pointer]->type == OP && (int) array[*pointer]->value == F_LN)
    {
        int num = *pointer;
        (*pointer)++;
        Node_t* value = GetP (pointer, array);
        array[num]->right = value;
        return array[num];
    }
    else
        return GetN (pointer, array);
}