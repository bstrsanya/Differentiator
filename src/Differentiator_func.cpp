#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <DSLmath.h>

#include "Differentiator_func.h"

Node_t* NodeCtor (int type, double value, Node_t* left, Node_t* right)
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

void TreeCtor (Tree_t* tree, const char* name_file)
{
    assert (tree);
    assert (name_file);

    tree->input = fopen (name_file, "rb");
    ReadDataBase (tree);
    fclose (tree->input);

    tree->output = fopen (file_latex, "wb");
}

void TreeDtor (Tree_t* tree)
{
    assert (tree);

    for (int i = 0; i < SIZE_ARRAY; i++)
        free (tree->array[i]);

    free (tree->array);
    NodeDtor (tree->expression_diff);
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
                fprintf (file, "\\left(");
                Print (node->left, file);
                fprintf (file, "+");
                Print (node->right, file);
                fprintf (file, "\\right)");
                break;
            }
            case F_SUB:
            {
                fprintf (file, "\\left(");
                Print (node->left, file);
                fprintf (file, "-");
                Print (node->right, file);
                fprintf (file, "\\right)");
                break;
            }
            case F_MUL:
            {
                // printf ("\\left(");
                Print (node->left, file);
                fprintf (file, " \\cdot ");
                Print (node->right, file);
                // printf ("\\right)");
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
            case F_LN:
            {
                fprintf (file, "\\ln\\left(");
                Print (node->right, file);
                fprintf (file, "\\right)");
                break;
            }
            case F_DEG:
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

Node_t* Diff (Node_t* node, FILE* file)
{
    Node_t* general_node = NULL;
    if (node->type == NUM)
    {
        Node_t* node1 = CONST (0);
        BEGIN; CONST_TEX(0); END;
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
        BEGIN; VAR_TEX; END;       
        return node2;
    }
    if (node->type == OP)
    {
        switch ((int) node->value)
        {
            case F_ADD:
            {
                BEGIN; OPEN; ADD_TEX (cl_tex, cr_tex); CLOSE; PRO; 
                EQUALLY; ADD_TEX (dl_tex, dr_tex); END;
                return ADD (dl, dr);
                break;
            }
            case F_SUB:
            {
                BEGIN; OPEN; SUB_TEX (cl_tex, cr_tex); CLOSE; PRO;
                EQUALLY; SUB_TEX (dl_tex, dr_tex); END;
                return SUB (dl, dr);
                break;
            }
            case F_MUL:
            {
                BEGIN; OPEN; MUL_TEX (cl_tex, cr_tex); CLOSE; PRO;
                EQUALLY; ADD_TEX (MUL_TEX (dl_tex, cr_tex), MUL_TEX (cl_tex, dr_tex)); END;
                return ADD (MUL (dl, cr), MUL (cl, dr));
                break;
            }
            case F_DIV:
            {
                BEGIN; OPEN; DIV_TEX (cl_tex, cr_tex); CLOSE; PRO;
                EQUALLY; DIV_TEX (SUB_TEX (MUL_TEX (dl_tex, cr_tex), MUL_TEX (cl_tex, dr_tex)), ST_TEX (cr_tex, NUMBER_TEX(2))); END;
                return DIV (SUB(MUL (dl, cr), MUL (cl, dr)), ST (cr, CONST (2)));
                break;
            }
            case F_COS:
            {
                BEGIN; OPEN; COS_TEX (cr_tex); CLOSE; PRO;
                EQUALLY; MUL_TEX (MUL_TEX (NUMBER_TEX(-1) , SIN_TEX (cr_tex)), dr_tex); END;
                return MUL (MUL (CONST (-1), SIN (cr)), dr);
                break;
            }
            case F_SIN:
            {
                BEGIN; OPEN; SIN_TEX (cr_tex); CLOSE; PRO;
                EQUALLY; MUL_TEX (dr_tex, COS_TEX (cr_tex)); END;
                return MUL (dr, COS (cr));
                break;
            }
            case F_LN:
            {
                BEGIN; OPEN; LN_TEX (cr_tex); CLOSE; PRO;
                EQUALLY; MUL_TEX (DIV_TEX (NUMBER_TEX (1), cr_tex), dr_tex); END;
                return MUL (DIV (CONST (1), cr), dr);
                break;
            }
            case F_DEG:
            {
                if (node->right->type == NUM)
                {
                    double poc = node->right->value;
                    BEGIN; OPEN; ST_TEX (cl_tex, cr_tex); CLOSE; PRO;
                    EQUALLY; MUL_TEX (MUL_TEX (NUMBER_TEX (poc), ST_TEX (cl_tex, NUMBER_TEX (poc-1))), dl_tex); END;
                    return MUL (MUL (CONST (poc), ST (cl, CONST (poc - 1))), dl);
                }
                else if (node->left->type == MATH_CONST && (int) node->left->value == F_E)
                {
                    BEGIN; OPEN; EXP_TEX (cr_tex); CLOSE; PRO;
                    EQUALLY; MUL_TEX (EXP_TEX (cr_tex), dr_tex); END;
                    return MUL (EXP (cr), dr);
                }
                else 
                {
                    BEGIN; OPEN; ST_TEX (cl_tex, cr_tex); CLOSE; PRO;
                    EQUALLY; MUL_TEX (ST_TEX (cl_tex, cr_tex), ADD_TEX (MUL_TEX (dr_tex, LN_TEX(cl_tex)), MUL_TEX(cr_tex, MUL_TEX (DIV_TEX (NUMBER_TEX(1), cl_tex), dl_tex)))); END;
                    return MUL (ST (cl, cr), ADD (MUL (dr, LN (cl)), MUL (cr, MUL (DIV (CONST (1), cl), dl))));
                }
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
            {
                return NodeCtor (OP, node->value, NULL, CopyNode (node->right));
                break;
            }
            default: break;
        }          
    }
    return NULL;
}