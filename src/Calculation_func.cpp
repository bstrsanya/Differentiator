#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "Differentiator_func.h"

void Calculation (Node_t* node, int* n)
{
    if (!node) return;
    
    if (node->left) Calculation (node->left, n);
    if (node->right) Calculation (node->right, n);

    if (node->left && node->right && node->type == OP && node->left->type == NUM && node->right->type == NUM)
    {
        (*n)++;
        switch ((int) node->value) 
        {
            case F_ADD:
            {
                node->value = node->left->value + node->right->value;
                break;
            }
            case F_SUB:
            {
                node->value = node->left->value - node->right->value;
                break;
            }
            case F_MUL:
            {
                node->value = node->left->value * node->right->value;
                break;
            }
            case F_DIV:
            {
                node->value = node->left->value / node->right->value;
                break;
            }
            case F_DEG:
            {
                node->value = pow (node->left->value, node->right->value);
                break;
            }
            default: printf ("Invalid operation with two arguments\n");
        }

        node->type = NUM;
        NodeDtor (node->left);
        NodeDtor (node->right);
        node->left = NULL;
        node->right = NULL;
    }
    CalculationAdd (node, n);  
    CalculationSub (node, n);  
    CalculationDeg (node, n);  
    CalculationMul (node, n);  

    if (node->type == OP && (int) node->value == F_DIV && node->right->type == NUM && CompareDouble (node->right->value, 0))
    {
        printf ("div on zero - illegal operation\n");
        assert (0);
    }
    
    if (!node->left && node->type == OP)
        SingleArgFunc (node, n);
}

void SingleArgFunc (Node_t* node, int* n)
{
    if (node->right->type == NUM)
    {
        (*n)++;
        double new_value = 0;
        switch ((int) node->value)
        {
            case F_COS:
            {
                new_value = cos (node->right->value);
                break;
            }
            case F_SIN:
            {
                new_value = sin (node->right->value);
                break;
            }
            case F_TAN:
            {
                new_value = tan (node->right->value);
                break;
            }
            case F_CTG:
            {
                new_value = 1 / tan (node->right->value);
                break;
            }
            case F_LN:
            {
                if (node->right->value < 0)
                {
                    printf ("ln(%g) - illegal operation\n", node->right->value);
                    assert (0);
                }
                new_value = log (node->right->value);
                break;
            }
            default:
            {
                printf ("%d %g ERROR\n", node->type, node->value);
                break;
            }
        }
        node->type = NUM;
        node->value = new_value;
        NodeDtor (node->right);
        node->right = NULL;
    }    
}

void CalculationMul (Node_t* node, int* n)
{
    if (node->type == OP && (int) node->value == F_MUL)
    {
        if ((node->left->type == NUM && CompareDouble (node->left->value, 0)) || 
            (node->right->type == NUM && CompareDouble (node->right->value, 0))) // *0 0*
        {
            (*n)++;

            node->type = NUM;
            node->value = 0;
            NodeDtor (node->left);
            NodeDtor (node->right);
            node->left = NULL;
            node->right = NULL;
        }
        else if (node->left->type == NUM && CompareDouble (node->left->value, 1)) // 1*
        {
            (*n)++;

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
        else if (node->right->type == NUM && CompareDouble (node->right->value, 1)) // *1
        {
            (*n)++;

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

void CalculationAdd (Node_t* node, int* n)
{
    if (node->type == OP && (int) node->value == F_ADD)
    {
        if (node->left->type == NUM && CompareDouble (node->left->value, 0)) // 0+
        {
            (*n)++;

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
        else if (node->right->type == NUM && CompareDouble (node->right->value, 0)) // +0
        {
            (*n)++;

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

void CalculationSub (Node_t* node, int* n)
{
    if (node->type == OP && (int) node->value == F_SUB)
    {
        if (node->right->type == NUM && CompareDouble (node->right->value, 0)) // -0
        {
            (*n)++;

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

void CalculationDeg (Node_t* node, int* n)
{
    if (node->type == OP && (int) node->value == F_DEG)
    {
        if (node->right->type == NUM && CompareDouble (node->right->value, 1))
        {
            (*n)++;

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
        else if (node->left->type == NUM && CompareDouble (node->left->value, 1))
        {
            (*n)++;
            
            node->type = NUM;
            node->value = 1;
            NodeDtor (node->left);
            NodeDtor (node->right);
            node->left = NULL;
            node->right = NULL;
        }
    }
}

int CompareDouble (double argument_1, double argument_2)
{
    if (fabs (argument_1 - argument_2) < SMALL)
        return true;
         
    return false;
}