#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Differentiator_func.h"

Node_t* CreateNode (int type, double value, Node_t* left, Node_t* right)
{
    Node_t* new_node = (Node_t*) calloc (1, sizeof (Node_t));
    assert (new_node);

    new_node->type = type;
    new_node->value = value;

    new_node->left = left;
    new_node->right = right;

    return new_node;
}

void NodeDtor (Node_t* node)
{
    if (!node) return;
    
    if (node->left) NodeDtor (node->left);
    if (node->right) NodeDtor (node->right);

    free (node);
}

void CreateDot (Node_t* node, FILE* file_dot)
{
    if (!node) return;

    if (node->type == NUM)
        fprintf (file_dot, "node%p [shape=record; style = filled; fillcolor = \"#a2ff00\"; color = \"#800000\"; label = \"{type = NUM | value = %lf | address = %p | { <f0> left = %p | <f1> right = %p}}\"];\n", node, node->value, node, node->left, node->right);

    else if (node->type == OP)
        fprintf (file_dot, "node%p [shape=record; style = filled; fillcolor = \"#ffe4c4\"; color = \"#800000\"; label = \"{type = OP | value = %c | address = %p | { <f0> left = %p | <f1> right = %p}}\"];\n", node, (char) node->value, node, node->left, node->right);

    if (node->left) 
    {
        fprintf (file_dot, "node%p: <f0> -> node%p [color = red, style = bold, arrowhead = vee];\n", node, node->left);
        CreateDot (node->left, file_dot);
    }
    if (node->right) 
    {
        fprintf (file_dot, "node%p: <f1> -> node%p [color = red, style = bold, arrowhead = vee];\n", node, node->right);
        CreateDot (node->right, file_dot);
    }
}

void CreateDotUSER (Node_t* node, FILE* file_dot)
{
    if (!node) return;

    if (node->type == NUM)
        fprintf (file_dot, "node%p [shape=record; style = filled; fillcolor = \"#a2ff00\"; label = \"%.3lf\"];\n", node, node->value);

    else if (node->type == OP)
        fprintf (file_dot, "node%p [shape=circle; style = filled; fillcolor = \"#ffe4c4\"; label = \"%c\"];\n", node, (char) node->value);

    if (node->left) 
    {
        fprintf (file_dot, "node%p -> node%p [color = red, style = bold, arrowhead = vee];\n", node, node->left);
        CreateDotUSER (node->left, file_dot);
    }
    if (node->right) 
    {
        fprintf (file_dot, "node%p -> node%p [color = red, style = bold, arrowhead = vee];\n", node, node->right);
        CreateDotUSER (node->right, file_dot);
    }
}

void PrintDot (Node_t* node)
{
    FILE* file_dot = fopen ("./aaa.dot", "w");
    assert (file_dot != NULL);
    // fprintf (file_dot, "digraph{\nsplines=\"ortho\";\n");
    fprintf (file_dot, "digraph{\n");
    CreateDotUSER (node, file_dot);
    fprintf (file_dot, "}");
    fclose (file_dot);
    system ("dot ./aaa.dot -Tpng -o ./aaa.png");
}

void Print (Node_t* node)
{
    if (!node) return;
    
    printf ("(");
    if (node->type == OP)
        printf ("%c", (int) node->value);
    else if (node->type == VAR)
        printf ("x");
    else 
        printf ("%.0lf", node->value);
    if (node->left) Print (node->left);
    if (node->right) Print (node->right);

    printf (")");
}

void SkipProb (char** str)
{
    int n = 0;
    sscanf (*str, "%*[\r \n]%n", &n);
    *str += n;
}

#define DEF_CMD(name, str, num, code) \
    case name: code break;

void Calculation (Node_t* node, stack_t* stk)
{
    if (!node) return;
    
    if (node->left) Calculation (node->left, stk);
    if (node->right) Calculation (node->right, stk);

    if (node->type == NUM) StackPush (stk, node);
    if (node->type == OP)
    {
        switch ((int) node->value)
        {
            #include "commands.h"

            default: {
                printf ("ERROR\n");
            }
        }
    }
}

#undef DEF_CMD

int p = 0;
const char* s = "25*10*(3*(25-10*2)+1)$";

Node_t* GetG ()
{
    Node_t* value = GetE ();
    if (s[p] != '$')
        assert (0);
    p++;
    return value;
}

Node_t* GetN ()
{
    int value = 0;
    while (('0' <= s[p]) && (s[p] <= '9'))
    {
        value = value * 10 + s[p] - '0';
        p++;
    }

    return CreateNode (NUM, value, NULL, NULL);
}

Node_t* GetE ()
{
    Node_t* value = GetT ();
    while (s[p] == '+' || s[p] == '-')
    {
        int op = s[p];
        p++;
        Node_t* value2 = GetT();
        if (op == '+')
            value = CreateNode (OP, 43, value, value2);
        else
            value = CreateNode (OP, 45, value, value2);
    }
    return value;
}

Node_t* GetT ()
{
    Node_t* value = GetP ();
    while (s[p] == '*' || s[p] == '/')
    {
        int op = s[p];
        p++;
        Node_t* value2 = GetP();
        if (op == '*')
            value = CreateNode (OP, 42, value, value2);
        else
            value = CreateNode (OP, 47, value, value2);
    }
    return value;
}

Node_t* GetP ()
{
    if (s[p] == '(')
    {
        p++;
        Node_t* value = GetE ();
        if (s[p] != ')')
            assert (0);
        p++;
        return value;
    }
    else
        return GetN ();
}