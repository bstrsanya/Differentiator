#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <DSLmath.h>

#include "Differentiator_func.h"

Node_t* NodeCtor (int type, double value, Node_t* left, Node_t* right)
{
    Node_t* new_node = (Node_t*) calloc (1, sizeof (Node_t));
    assert (new_node);

    new_node->type = (type_com) type;
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

void TreeCtor (Tree_t* tree, const char* name_file)
{
    assert (tree);
    assert (name_file);

    system ("mkdir -p " DIR);

    tree->input = fopen (name_file, "rb");
    ReadDataBase (tree);
    fclose (tree->input);

    tree->output = fopen (FILE_LATEX, "wb");
}

void TreeDtor (Tree_t* tree)
{
    assert (tree);

    NodeDtor (tree->expression);
    // for (int y = SIZE_ARRAY - 1; y != 0; y--)
    // {
    //     if ((int) tree->array[y]->value != '$')
    //         free (tree->array[y]);
    //     else
    //     {
    //         free (tree->array[y]);
    //         break;
    //     }
    // } 
    // for (int i = 0; i < SIZE_ARRAY; i++)
    // {
    //     if (tree->array[i])
    //         free (tree->array[i]);
    // }

    free (tree->array);
    NodeDtor (tree->expression_diff);

    if (tree->output)
        fclose (tree->output);
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

    if (node->type == VAR) 
        fprintf (file, "x");

    if (node->type == MATH_CONST) 
        fprintf (file, "%c", (int) node->value);

    if (node->type == OP)
    {
        switch ((int) node->value)
        {
            case F_ADD:
            {
                // fprintf (file, "\\left(");
                Print (node->left, file);
                fprintf (file, "+");
                Print (node->right, file);
                // fprintf (file, "\\right)");
                break;
            }
            case F_SUB:
            {
                // fprintf (file, "\\left(");
                Print (node->left, file);
                fprintf (file, "-");
                Print (node->right, file);
                // fprintf (file, "\\right)");
                break;
            }
            case F_MUL:
            {
                // printf ("\\left(");
                if ((int) node->left->value == F_ADD || (int) node->left->value == F_SUB)
                {
                    fprintf (file, "\\left(");
                    Print (node->left, file);
                    fprintf (file, "\\right)");
                }
                else
                {
                    Print (node->left, file);
                }
                fprintf (file, " \\cdot ");
                if ((int) node->right->value == F_ADD || (int) node->right->value == F_SUB)
                {
                    fprintf (file, "\\left(");
                    Print (node->right, file);
                    fprintf (file, "\\right)");
                }
                else
                {
                    Print (node->right, file);
                }                
                break;
            }
            case F_DIV:
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
            case F_COS:
            {
                fprintf (file, "\\cos\\left(");
                Print (node->right, file);
                fprintf (file, "\\right)");
                break;
            }
            case F_SIN:
            {
                fprintf (file, "\\sin\\left(");
                Print (node->right, file);
                fprintf (file, "\\right)");
                break;
            }
            case F_TAN:
            {
                fprintf (file, "\\tg\\left(");
                Print (node->right, file);
                fprintf (file, "\\right)");
                break;
            }
            case F_CTG:
            {
                fprintf (file, "\\ctg\\left(");
                Print (node->right, file);
                fprintf (file, "\\right)");
                break;
            }
            case F_LN:
            {
                fprintf (file, "\\ln\\left(");
                Print (node->right, file);
                fprintf (file, "\\right)");
                break;
            }
            case F_DEG:
            {
                if (node->left->type == NUM || node->left->type == VAR)
                {
                    Print (node->left, file);
                    fprintf (file, "^{");
                    Print (node->right, file);
                    fprintf (file, "}");
                }
                else
                {
                    fprintf (file, "\\left(");
                    Print (node->left, file);
                    fprintf (file, "\\right)^{");
                    Print (node->right, file);
                    fprintf (file, "}");
                }
                break;
            }
            default:
            {
                // printf ("%g \t %d\n", node->value, node->type);
                printf ("ERERR\n");
                break;
            }
        }
    }
}

void Differentiator (Tree_t* tree)
{
    int n = 0;
    Calculation (tree->expression, &n);

    if (n)
    {
        fprintf (tree->output, "\\text{Для начала упростим функцию}\n");
        fprintf (tree->output, "\\[f(x) = ");
        Print (tree->expression, tree->output);
        fprintf (tree->output, "\\]\n");
    }

    tree->expression_diff = Diff (tree->expression, tree->output);
    Calculation (tree->expression_diff, &n);
}

Node_t* Diff (Node_t* node, FILE* file)
{
    if (node->type == NUM)
    {
        Node_t* node1 = CONST (0);
        NumTex (node, file);
        return node1;
    }
    if (node->type == MATH_CONST)
    {
        Node_t* node10 = CONST (0);
        MathConstTex (node, file);
        return node10;
    }
    if (node->type == VAR)
    {
        Node_t* node2 = CONST (1);
        VarTex (node, file);  
        return node2;
    }
    if (node->type == OP)
    {
        switch ((int) node->value)
        {
            case F_ADD:
            {
                AddTex (node, file);
                return ADD (dl, dr);
                break;
            }
            case F_SUB:
            {
                SubTex (node, file);
                return SUB (dl, dr);
                break;
            }
            case F_MUL:
            {
                MulTex (node, file);
                return ADD (MUL (dl, cr), MUL (cl, dr));
                break;
            }
            case F_DIV:
            {
                DivTex (node, file);
                return DIV (SUB(MUL (dl, cr), MUL (cl, dr)), ST (cr, CONST (2)));
                break;
            }
            case F_COS:
            {
                CosTex (node, file);
                return MUL (MUL (CONST (-1), SIN (cr)), dr);
                break;
            }
            case F_SIN:
            {
                SinTex (node, file);
                return MUL (dr, COS (cr));
                break;
            }
            case F_LN:
            {
                LnTex (node, file);
                return MUL (DIV (CONST (1), cr), dr);
                break;
            }
            case F_DEG:
            {
                DegTex (node, file);

                if (node->right->type == NUM)
                {
                    double poc = node->right->value;
                    return MUL (MUL (CONST (poc), ST (cl, CONST (poc - 1))), dl);
                }
                else if (node->left->type == NUM)
                {
                    return MUL (MUL (ST (cl, cr), LN (cl)), dr);
                }
                else if (node->left->type == MATH_CONST && (int) node->left->value == F_E)
                {
                    return MUL (EXP (cr), dr);
                }
                else 
                {
                    return MUL (ST (cl, cr), ADD (MUL (dr, LN (cl)), MUL (cr, MUL (DIV (CONST (1), cl), dl))));
                }
                break;
            }
            case F_TAN:
            {
                TanTex (node, file);
                return MUL (DIV (CONST (1), ST (COS (cr), CONST (2))), dr);
                break;
            }
            case F_CTG:
            {
                CtgTex (node, file);
                return MUL (DIV (CONST (-1), ST (SIN (cr), CONST (2))), dr);
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
        Node_t* new_node = NodeCtor (node->type, node->value, node->left, node->right);
        assert (new_node);

        return new_node;
    }
    if (node->type == OP)
    {
        switch ((int) node->value)
        {
            case F_ADD:
            case F_SUB:
            case F_MUL:
            case F_DIV:
            case F_DEG:
            {
                return NodeCtor (OP, node->value, CopyNode (node->left), CopyNode (node->right));
                break;
            }
            case F_COS:
            case F_SIN:
            case F_LN:
            case F_CTG:
            case F_TAN:
            {
                return NodeCtor (OP, node->value, NULL, CopyNode (node->right));
                break;
            }
            default: break;
        }          
    }
    return NULL;
}