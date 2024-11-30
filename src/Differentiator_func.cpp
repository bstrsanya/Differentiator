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

// void Print (Node_t* node)
// {
//     if (!node) return;
    
//     if (node->left) 
//     {
//         printf ("("); 
//         Print (node->left);
//     }

//     if (node->type == OP)
//     {
//         if ((int) node->value == 42)
//             printf (" \\cdot ");
//         else if ((int) node->value == 47)
//             printf (" \\frac ");
//         else
//             printf (" %c ", (int) node->value);
//     }
//     else if (node->type == VAR)
//         printf ("x");
//     else 
//         printf ("%.0lf", node->value);

//     if (node->right) {Print (node->right); printf (")");}


// }

void SkipProb (char** str)
{
    int n = 0;
    sscanf (*str, "%*[\r \n]%n", &n);
    *str += n;
}

void Print (Node_t* node) // Попробовать написать также как Diff идя сверху
{
    if (node->type == NUM) printf ("%.1lf", node->value);
    if (node->type == VAR) printf ("x");
    if (node->type == OP)
    {
        switch ((int) node->value)
        {
            case '+':
            {
                printf ("\\left(");
                Print (node->left);
                printf ("+");
                Print (node->right);
                printf ("\\right)");
                break;
            }
            case '-':
            {
                printf ("\\left(");
                Print (node->left);
                printf ("-");
                Print (node->right);
                printf ("\\right)");
                break;
            }
            case '*':
            {
                printf ("\\left(");
                Print (node->left);
                printf (" \\cdot ");
                Print (node->right);
                printf ("\\right)");
                break;
            }
            case '/':
            {
                printf ("\\left(");
                printf ("\\frac{");
                Print (node->left);
                printf ("}{");
                Print (node->right);
                putchar ('}');
                printf ("\\right)");
                break;
            }
            default:
            {
                printf ("ERERR\n");
                break;
            }
        }
    }
}

void Calculation (Node_t* node)
{
    if (!node) return;
    
    if (node->left->left) Calculation (node->left);
    if (node->right->left) Calculation (node->right);

    if (node->type == OP && node->left->type == NUM && node->right->type == NUM)
    {
        switch ((int) node->value) 
        {
            case '+':
            {
                node->value = node->left->value + node->right->value;
                break;
            }
            case '-':
            {
                node->value = node->left->value - node->right->value;
                break;
            }
            case '*':
            {
                node->value = node->left->value * node->right->value;
                break;
            }
            case '/':
            {
                node->value = node->left->value / node->right->value;
                break;
            }
            default: printf ("er\n");
        }
        node->type = NUM;
        NodeDtor (node->left);
        NodeDtor (node->right);
        node->left = NULL;
        node->right = NULL;
    }
    else if (node->type == OP && (int) node->value == '*')
    {
        if ((node->left->type == NUM && (int) node->left->value == 0) || 
            (node->right->type == NUM && (int) node->right->value == 0)) // *0 0*
        {
            node->type = NUM;
            node->value = 0;
            NodeDtor (node->left);
            NodeDtor (node->right);
            node->left = NULL;
            node->right = NULL;
        }
        else if (node->left->type == NUM && (int) node->left->value == 1) // 1*
        {
            node->type = node->right->type;
            node->value = node->right->value;

            Node_t* new_left = NULL;
            Node_t* new_right = NULL;
            if (node->right->right) 
                new_right = CopyNode (node->right->right);
            if (node->right->left)
                new_left = CopyNode (node->right->left);
            
            NodeDtor (node->left);
            NodeDtor (node->right);
            node->left = new_left;
            node->right = new_right; 
        }
        else if (node->right->type == NUM && (int) node->right->value == 1) // *1
        {
            node->type = node->left->type;
            node->value = node->left->value;

            Node_t* new_left = NULL;
            Node_t* new_right = NULL;
            if (node->left->right) 
                new_right = CopyNode (node->left->right);
            if (node->left->left)
                new_left = CopyNode (node->left->left);
            
            NodeDtor (node->left);
            NodeDtor (node->right);
            node->left = new_left;
            node->right = new_right;         
        }
    }
    else if (node->type == OP && (int) node->value == '+')
    {
        if (node->left->type == NUM && (int) node->left->value == 0) // 0+
        {
            node->type = node->right->type;
            node->value = node->right->value;

            Node_t* new_left = NULL;
            Node_t* new_right = NULL;
            if (node->right->right) 
                new_right = CopyNode (node->right->right);
            if (node->right->left)
                new_left = CopyNode (node->right->left);
            
            NodeDtor (node->left);
            NodeDtor (node->right);
            node->left = new_left;
            node->right = new_right; 
        }
        else if (node->right->type == NUM && (int) node->right->value == 0) // +0
        {
            node->type = node->left->type;
            node->value = node->left->value;

            Node_t* new_left = NULL;
            Node_t* new_right = NULL;
            if (node->left->right) 
                new_right = CopyNode (node->left->right);
            if (node->left->left)
                new_left = CopyNode (node->left->left);
            
            NodeDtor (node->left);
            NodeDtor (node->right);
            node->left = new_left;
            node->right = new_right; 
        }
    }
    else if (node->type == OP && (int) node->value == '-')
    {
        if (node->right->type == NUM && (int) node->right->value == 0) // -0
        {
            node->type = node->left->type;
            node->value = node->left->value;

            Node_t* new_left = NULL;
            Node_t* new_right = NULL;
            if (node->left->right) 
                new_right = CopyNode (node->left->right);
            if (node->left->left)
                new_left = CopyNode (node->left->left);
            
            NodeDtor (node->left);
            NodeDtor (node->right);
            node->left = new_left;
            node->right = new_right; 
        }
    }
}

#define dl Diff(node->left)
#define dr Diff(node->right)
#define cl CopyNode(node->left)
#define cr CopyNode(node->right)
#define CONST(c) CreateNode (NUM, c, NULL, NULL)
#define ADD(x,y) CreateNode (OP, '+', x, y)
#define SUB(x,y) CreateNode (OP, '-', x, y)
#define MUL(x,y) CreateNode (OP, '*', x, y)
#define DIV(x,y) CreateNode (OP, '/', x, y)
#define COS(x)   CreateNode (OP, 'c', NULL, x)
#define SIN(x)   CreateNode (OP, 's', NULL, x)

Node_t* a = NULL;
#define dl_ a = CopyNode (node->left); printf ("\\left("); Print (a); printf ("\\right)'"); NodeDtor (a)
#define dr_ a = CopyNode (node->right); printf ("\\left("); Print (a); printf ("\\right)'"); NodeDtor (a)
#define cl_ a = CopyNode (node->left); printf ("\\left("); Print (a); printf ("\\right)"); NodeDtor (a)
#define cr_ a = CopyNode (node->right); printf ("\\left("); Print (a); printf ("\\right)"); NodeDtor (a)
#define ADD_(x,y) {x; printf (" + "); y;}
#define SUB_(x,y) {x; printf (" - "); y;}
#define MUL_(x,y) {x; printf (" \\cdot "); y;}
#define DIV_(x,y) {printf ("\\frac{"); x; printf ("}{"); y; putchar ('}');}

Node_t* Diff (Node_t* node)
{
    if (node->type == NUM)
    {
        Node_t* node1 = CONST (0);
        printf ("\\[(%.1lf)' = 0\\]\n", node->value);
        return node1;
    }
    if (node->type == VAR)
    {
        Node_t* node2 = CONST (1);
        printf ("\\[(%c)' = 1\\]\n", (int) node->value);
        return node2;
    }
    if (node->type == OP)
    {
        switch ((int) node->value)
        {
            case '+':
            {
                printf ("\\[");
                // ADD_ (cl_, cr_); printf ("' = ");
                ADD_ (dl_, dr_);
                printf ("\\]\n");
                return ADD (dl, dr);
                break;
            }
            case '-':
            {
                printf ("\\[");
                SUB_ (dl_, dr_);
                printf ("\\]\n");
                return SUB (dl, dr);
                break;
            }
            case '*':
            {
                printf ("\\[");
                ADD_ (MUL_ (dl_, cr_), MUL_ (cl_, dr_));
                printf ("\\]\n");
                return ADD (MUL (dl, cr), MUL (cl, dr));
                break;
            }
            case '/':
            {
                printf ("\\[");
                DIV_ (SUB_(MUL_ (dl_, cr_), MUL_ (cl_, dr_)), MUL_ (cr_, cr_));
                printf ("\\]\n");
                return DIV (SUB(MUL (dl, cr), MUL (cl, dr)), MUL (cr, cr));
                break;
            }
            case 'c':
            {
                return MUL (MUL (CONST (-1), SIN (cr)), dr);
                break;
            }
            case 's':
            {
                return MUL (COS (cr), dr);
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