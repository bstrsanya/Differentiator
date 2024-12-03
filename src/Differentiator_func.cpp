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

    // printf ("ad = %p, type = [%d], value = [%g]\n", node, node->type, node->value);
    free (node);
}

void TreeCtor (Tree_t* tree, const char* name_file)
{
    assert (tree);
    assert (name_file);

    tree->input = fopen (name_file, "rb");
    ReadDataBase (tree);
    fclose (tree->input);

    tree->output = fopen ("dif.tex", "wb");
}

void TreeDtor (Tree_t* tree)
{
    assert (tree);

    NodeDtor (tree->expression);
    NodeDtor (tree->expression_diff);
    fclose (tree->output);
}


void CreateDot (Node_t* node, FILE* file_dot)
{
    if (!node) return;

    if (node->type == NUM)
        fprintf (file_dot, "node%p [shape=record; style = filled; fillcolor = \"#a2ff00\"; color = \"#800000\"; label = \"{type = NUM | value = %g | address = %p | { <f0> left = %p | <f1> right = %p}}\"];\n", node, node->value, node, node->left, node->right);

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
        fprintf (file_dot, "node%p [shape=record; style = filled; fillcolor = \"#a2ff00\"; label = \"%.3g\"];\n", node, node->value);

    else if (node->type == OP)
        fprintf (file_dot, "node%p [shape=circle; style = filled; fillcolor = \"#ffe4c4\"; label = \"%c\"];\n", node, (char) node->value);

    else if (node->type == VAR)
        fprintf (file_dot, "node%p [shape=record; style = filled; fillcolor = pink; label = \"%c\"];\n", node, (char) node->value);

    else if (node->type == MATH_CONST)
        fprintf (file_dot, "node%p [shape=record; style = filled; fillcolor = \"#0000ff8f\"; label = \"%c\"];\n", node, (char) node->value);

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

void SkipProb (char** str)
{
    int n = 0;
    sscanf (*str, "%*[\r \n]%n", &n);
    *str += n;
}

void Print (Node_t* node, FILE* file)
{
    if (node->type == NUM) 
    {
        if (node->value < 0)
            fprintf (file, "(%g)", node->value);
        else
            fprintf (file, "%g", node->value);
    }
    if (node->type == VAR) fprintf (file, "x");
    if (node->type == MATH_CONST) 
        fprintf (file, "%c", (int) node->value);
    if (node->type == OP)
    {
        switch ((int) node->value)
        {
            case '+':
            {
                fprintf (file, "\\left(");
                Print (node->left, file);
                fprintf (file, "+");
                Print (node->right, file);
                fprintf (file, "\\right)");
                break;
            }
            case '-':
            {
                fprintf (file, "\\left(");
                Print (node->left, file);
                fprintf (file, "-");
                Print (node->right, file);
                fprintf (file, "\\right)");
                break;
            }
            case '*':
            {
                // printf ("\\left(");
                Print (node->left, file);
                fprintf (file, " \\cdot ");
                Print (node->right, file);
                // printf ("\\right)");
                break;
            }
            case '/':
            {
                // if (node->left->type == OP) printf ("\\left(");
                fprintf (file, "\\frac{");
                Print (node->left, file);
                fprintf (file, "}{");
                Print (node->right, file);
                fprintf (file, "}");
                // printf ("\\right)");
                break;
            }
            case 'c':
            {
                fprintf (file, "\\cos\\left(");
                Print (node->right, file);
                fprintf (file, "\\right)");
                break;
            }
            case 's':
            {
                fprintf (file, "\\sin\\left(");
                Print (node->right, file);
                fprintf (file, "\\right)");
                break;
            }
            case 'l':
            {
                fprintf (file, "\\ln\\left(");
                Print (node->right, file);
                fprintf (file, "\\right)");
                break;
            }
            case '^':
            {
                fprintf (file, "\\left(");
                Print (node->left, file);
                fprintf (file, "\\right)^{");
                Print (node->right, file);
                fprintf (file, "}");
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
    if (node->type == OP && (int) node->value == '*')
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
    if (node->type == OP && (int) node->value == '+')
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
    if (node->type == OP && (int) node->value == '-')
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
    if (node->type == OP && (int) node->value == 'l')
    {
        if (node->right->type == MATH_CONST && (int) node->right->value == 'e')
        {
            node->type = NUM;
            node->value = 1;
            NodeDtor (node->right);
            node->right = NULL; 
        }
    }
}

#define dl Diff(node->left, file)
#define dr Diff(node->right, file)
#define cl CopyNode(node->left)
#define cr CopyNode(node->right)
#define CONST(c) CreateNode (NUM, c, NULL, NULL)
#define ADD(x,y) CreateNode (OP, '+', x, y)
#define SUB(x,y) CreateNode (OP, '-', x, y)
#define MUL(x,y) CreateNode (OP, '*', x, y)
#define DIV(x,y) CreateNode (OP, '/', x, y)
#define COS(x)   CreateNode (OP, 'c', NULL, x)
#define SIN(x)   CreateNode (OP, 's', NULL, x)
#define ST(x, y) CreateNode (OP, '^', x, y)
#define LN(x) CreateNode (OP, 'l', NULL, x)
#define EXP(x) CreateNode (OP, '^', CreateNode (MATH_CONST, 'e', NULL, NULL), x)

Node_t* a = NULL;
#define dl_ a = CopyNode (node->left); fprintf (file, "\\left("); Print (a, file); fprintf (file, "\\right)'"); NodeDtor (a)
#define dr_ a = CopyNode (node->right); fprintf (file, "\\left("); Print (a, file); fprintf (file, "\\right)'"); NodeDtor (a)
#define cl_ a = CopyNode (node->left); fprintf (file, "\\left("); Print (a, file); fprintf (file, "\\right)"); NodeDtor (a)
#define cr_ a = CopyNode (node->right); fprintf (file, "\\left("); Print (a, file); fprintf (file, "\\right)"); NodeDtor (a)
#define ADD_(x,y) {fprintf (file, "\\left("); x; fprintf (file, " + "); y; fprintf (file, "\\right)");}
#define SUB_(x,y) {x; fprintf (file, " - "); y;}
#define MUL_(x,y) {x; fprintf (file, " \\cdot "); y;}
#define DIV_(x,y) {fprintf (file, "\\frac{"); x; fprintf (file, "}{"); y; fprintf (file, "}");}
#define SIN_(x) {fprintf (file, "\\sin"); x;}
#define COS_(x) {fprintf (file, "\\cos"); x;}
#define ST_(x, y) {x; fprintf (file, "^"); fprintf (file, "{"); y; fprintf (file, "}");}
#define LN_(x) {fprintf (file, "\\ln{"); x; fprintf (file, "}");}

#define CONST_(x) fprintf (file, "%.1g' = %d", node->value, x);
#define M_(x) fprintf (file, "%d", x);
#define VAR_ fprintf (file, "\%c' = 1", (int) node->value);

#define BEGIN fprintf (file, "\\text{Очень очевидно, что}\n\\begin{equation}\n\t")
#define OPEN fprintf (file, "\\left(")
#define CLOSE fprintf (file, "\\right)")
#define PRO fprintf (file, "'")
#define EQUALLY fprintf (file, "=")
#define END fprintf (file, "\n\\end{equation}\n")

Node_t* Diff (Node_t* node, FILE* file)
{
    if (node->type == NUM)
    {
        Node_t* node1 = CONST (0);
        BEGIN; CONST_(0); END;
        return node1;
    }
    if (node->type == MATH_CONST)
    {
        Node_t* node10 = CONST (0);
        BEGIN; fprintf (file, "%c' = 0", (int) node->value); END;
        return node10;
    }
    if (node->type == VAR)
    {
        Node_t* node2 = CONST (1);
        BEGIN; VAR_; END;       
        return node2;
    }
    if (node->type == OP)
    {
        switch ((int) node->value)
        {
            case '+':
            {
                BEGIN; OPEN; ADD_ (cl_, cr_); CLOSE; PRO; 
                EQUALLY; ADD_ (dl_, dr_); END;
                return ADD (dl, dr);
                break;
            }
            case '-':
            {
                BEGIN; OPEN; SUB_ (cl_, cr_); CLOSE; PRO;
                EQUALLY; SUB_ (dl_, dr_); END;
                return SUB (dl, dr);
                break;
            }
            case '*':
            {
                BEGIN; OPEN; MUL_ (cl_, cr_); CLOSE; PRO;
                EQUALLY; ADD_ (MUL_ (dl_, cr_), MUL_ (cl_, dr_)); END;
                return ADD (MUL (dl, cr), MUL (cl, dr));
                break;
            }
            case '/':
            {
                BEGIN; OPEN; DIV_ (cl_, cr_); CLOSE; PRO;
                EQUALLY; DIV_ (SUB_(MUL_ (dl_, cr_), MUL_ (cl_, dr_)), ST_ (cr_, M_(2))); END;
                return DIV (SUB(MUL (dl, cr), MUL (cl, dr)), ST (cr, CONST (2)));
                break;
            }
            case 'c':
            {
                BEGIN; OPEN; COS_ (cr_); CLOSE; PRO;
                EQUALLY; MUL_ (MUL_ (M_(-1) , SIN_ (cr_)), dr_); END;
                return MUL (MUL (CONST (-1), SIN (cr)), dr);
                break;
            }
            case 's':
            {
                BEGIN; OPEN; SIN_ (cr_); CLOSE; PRO;
                EQUALLY; MUL_ (COS_ (cr_), dr_); END;
                return MUL (COS (cr), dr);
                break;
            }
            case 'l':
            {
                BEGIN; OPEN; LN_ (cr_); CLOSE; PRO;
                EQUALLY; MUL_ (DIV_ (M_ (1), cr_), dr_); END;
                return MUL (DIV (CONST (1), cr), dr);
                break;
            }
            case '^':
            {
                BEGIN; OPEN; ST_ (cl_, cr_); CLOSE; PRO;
                EQUALLY; MUL_ (ST_ (cl_, cr_), ADD_ (MUL_ (dr_, LN_(cl_)), MUL_(cr_, MUL_ (DIV_ (M_(1), cl_), dl_)))); END;
                return MUL (ST (cl, cr), ADD (MUL (dr, LN (cl)), MUL (cr, MUL (DIV (CONST (1), cl), dl))));
                break;
            }
            default: break;
        }
    }
    return NULL;
}

Node_t* CopyNode (Node_t* node)
{
    if (node->type == NUM || node->type == VAR || node->type == MATH_CONST)
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
            case '^':
            {
                return CreateNode (OP, node->value, CopyNode (node->left), CopyNode (node->right));
                break;
            }
            case 'c':
            case 's':
            case 'l':
            {
                return CreateNode (OP, node->value, NULL, CopyNode (node->right));
                break;
            }
            default: break;
        }          
    }
    return NULL;
}