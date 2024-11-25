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

Node_t* ReadDataBase ()
{
    size_t size = 0;
    FILE* file = fopen ("primer.txt", "r");
    char* str = ReadFile (file, &size);
    fclose (file);

    stack_t stk = {};
    StackCtor (&stk, 10);

    while (str[0]) // != EOF
    {
        SkipProb (&str);
        if (str[0] == '(')
        {
            str += 1;
            SkipProb (&str);

            int n = 0;
            if (str[0] >= 48 && str[0] <= 57)
            {
                double num = 0;
                sscanf (str, "%lf%n", &num, &n);
                Node_t* ad = CreateNode (NUM, num, NULL, NULL);
                StackPush (&stk, ad);
                str += n;
            }
            else
            {
                int cmd = 0;
                if ((int) str[0] == '+') cmd = '+';
                if ((int) str[0] == '-') cmd = '-';
                if ((int) str[0] == '*') cmd = '*';
                if ((int) str[0] == '/') cmd = '/';
                str += 1;
                Node_t* ad = CreateNode (OP, cmd, NULL, NULL);
                StackPush (&stk, ad);
            }
        }
        if (str[0] == ')')
        {
            str += 1;
            Node_t* sun = NULL;
            StackPop (&stk, &sun);

            if (stk.size != 0) 
            {
                Node_t* parent = NULL;
                StackPop (&stk, &parent);

                if (!parent->left)
                    parent->left = sun;
                else
                    parent->right = sun;

                StackPush (&stk, parent); 
            }  
            else 
            {
                StackPush (&stk, sun);
            }          
        }
    }
    
    Node_t* node = (Node_t*) calloc (1, sizeof (Node_t));
    StackPop (&stk, &node);
    FreeStack (&stk);
    return node;
}

void SkipProb (char** str)
{
    int n = 0;
    sscanf (*str, "%*[\r \n]%n", &n);
    *str += n;
}

#define DEF_CMD(name, num, code) \
    case name: code break;

void Calculation (Node_t* node, stack_t* stk)
{
    if (!node) return;
    
    if (node->left) Calculation (node->left, stk);
    if (node->right) Calculation (node->right, stk);

    if (node->type == NUM) StackPush (stk, node);
    if (node->type == OP)
    {
        int cmd = 0;
        if ((int) node->value == '+') cmd = 101;
        if ((int) node->value == '-') cmd = 102;
        if ((int) node->value == '*') cmd = 103;
        if ((int) node->value == '/') cmd = 104;
        switch (cmd)
        {
            #include "commands.h"

            default: {
                printf ("ERROR\n");
            }
        }
    }
}

#undef DEF_CMD