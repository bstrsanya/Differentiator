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

    // printf ("ad = %p, type = [%d], value = [%lf]\n", node, node->type, node->value);
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

    else if (node->type == VAR)
        fprintf (file_dot, "node%p [shape=record; style = filled; fillcolor = pink; label = \"%c\"];\n", node, (char) node->value);

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

    if (node->left) Print (node->left);

    if (node->type == OP)
    {
        if ((int) node->value == 42)
            printf (" \\cdot ");
        else if ((int) node->value == 47)
            printf (" \\frac ");
        else
            printf (" %c ", (int) node->value);
    }
    else if (node->type == VAR)
        printf ("x");
    else 
        printf ("%.0lf", node->value);

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

#define _ADD(x,y) {CreateNode (OP, '+', Diff (x), Diff (y));}

Node_t* Diff (Node_t* node)
{
    if (node->type == NUM)
    {
        Node_t* node1 = CreateNode (NUM, 0, NULL, NULL);
        return node1;
    }
    if (node->type == VAR)
    {
        Node_t* node2 = CreateNode (NUM, 1, NULL, NULL);
        return node2;
    }
    if (node->type == OP)
    {
        switch ((int) node->value)
        {
            case '+':
            {
                Node_t* dl = Diff (node->left);
                Node_t* dr = Diff (node->right);
                Node_t* node3 = CreateNode (OP, '+', dl, dr);
                return node3;
                break;
            }
            case '-':
            {
                Node_t* dl = Diff (node->left);
                Node_t* dr = Diff (node->right);
                Node_t* node4 = CreateNode (OP, '-', dl, dr);
                return node4;
                break;
            }
            case '*':
            {
                Node_t* node5 = CreateNode (OP, '+', CreateNode (OP, '*', Diff (node->left), CopyNode (node->right)),
                                            CreateNode (OP, '*', CopyNode (node->left), Diff (node->right))); 
                return node5;
                break;
            }
            case '/':
            {
                Node_t* node6 = CreateNode (OP, '/', CreateNode (OP, '-', CreateNode (OP, '*', Diff (node->left), CopyNode(node->right)), CreateNode (OP, '*', CopyNode (node->left), Diff (node->right))), CreateNode (OP, '^', CopyNode (node->right), CreateNode (NUM, 2, NULL, NULL)));
                return node6;
                break;
            }
            case 'c':
            {
                Node_t* node7 = CreateNode (OP, '*', CreateNode (OP, '*', CreateNode (NUM, -1, NULL, NULL), CreateNode (OP, 's', NULL, CopyNode (node->right))), 
                                            Diff (node->right));
                return node7;
                break;
            }
            case 's':
            {
                Node_t* node8 = CreateNode (OP, '*', CreateNode (OP, '*', CreateNode (NUM, 1, NULL, NULL), CreateNode (OP, 'c', NULL, CopyNode (node->right))), Diff (node->right));
                return node8;
                break;
            }
            default: break;
        }
    }
    return NULL;
}

Node_t* CopyNode (Node_t* node)
{
    if (node->type == NUM || node->type == VAR)
    {
        Node_t* new_node = CreateNode (node->type, node->value, node->left, node->right);
        assert (new_node);

        return new_node;
    }
    if (node->type == OP)
    {
        switch ((int) node->value)
        {
            case '+':
            case '-':
            case '*':
            case '/':
            {
                return CreateNode (OP, node->value, CopyNode (node->left), CopyNode (node->right));
                break;
            }
            case 'c':
            case 's':
            {
                return CreateNode (OP, node->value, NULL, CopyNode (node->right));
                break;
            }
            default: break;
        }          
    }
    return NULL;
}