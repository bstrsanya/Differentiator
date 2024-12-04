#define ADD(x,y) NodeCtor (OP, F_ADD, x,    y)
#define SUB(x,y) NodeCtor (OP, F_SUB, x,    y)
#define MUL(x,y) NodeCtor (OP, F_MUL, x,    y)
#define DIV(x,y) NodeCtor (OP, F_DIV, x,    y)
#define COS(x)   NodeCtor (OP, F_COS, NULL, x)
#define SIN(x)   NodeCtor (OP, F_SIN, NULL, x)
#define ST(x, y) NodeCtor (OP, F_DEG, x,    y)
#define LN(x)    NodeCtor (OP, F_LN,  NULL, x)
#define CONST(c) NodeCtor (NUM, c, NULL, NULL)

#define EXP(x) NodeCtor (OP, '^', NodeCtor (MATH_CONST, 'e', NULL, NULL), x)

#define dl Diff     (node->left, file)
#define dr Diff     (node->right, file)
#define cl CopyNode (node->left)
#define cr CopyNode (node->right)

#define dl_tex general_node = CopyNode (node->left); fprintf (file, "\\left("); Print (general_node, file); fprintf (file, "\\right)'"); NodeDtor (general_node)
#define dr_tex general_node = CopyNode (node->right); fprintf (file, "\\left("); Print (general_node, file); fprintf (file, "\\right)'"); NodeDtor (general_node)
#define cl_tex general_node = CopyNode (node->left); fprintf (file, "\\left("); Print (general_node, file); fprintf (file, "\\right)"); NodeDtor (general_node)
#define cr_tex general_node = CopyNode (node->right); fprintf (file, "\\left("); Print (general_node, file); fprintf (file, "\\right)"); NodeDtor (general_node)

#define ADD_TEX(x,y) {fprintf (file, "\\left("); x; fprintf (file, " + "); y; fprintf (file, "\\right)");}
#define SUB_TEX(x,y) {fprintf (file, "\\left("); x; fprintf (file, " - "); y; fprintf (file, "\\right)");}
#define MUL_TEX(x,y) {x; fprintf (file, " \\cdot "); y;}
#define DIV_TEX(x,y) {fprintf (file, "\\frac{"); x; fprintf (file, "}{"); y; fprintf (file, "}");}
#define ST_TEX(x, y) {x; fprintf (file, "^"); fprintf (file, "{"); y; fprintf (file, "}");}
#define SIN_TEX(x)   {fprintf (file, "\\sin"); x;}
#define COS_TEX(x)   {fprintf (file, "\\cos"); x;}
#define LN_TEX(x)    {fprintf (file, "\\ln{"); x; fprintf (file, "}");}
#define EXP_TEX(x)   {fprintf (file, "e^{"); x; fprintf (file, "}");}

#define CONST_TEX(x)  fprintf (file, "%g' = %g", node->value, (double) x)
#define NUMBER_TEX(x) fprintf (file, "%g", (double) x)
#define VAR_TEX       fprintf (file, "\%c' = 1", (int) node->value)

#define BEGIN fprintf (file, "\\text{Understand that,}\n\\begin{equation}\n\t")
#define OPEN fprintf (file, "\\left(")
#define CLOSE fprintf (file, "\\right)")
#define PRO fprintf (file, "'")
#define EQUALLY fprintf (file, "=")
#define END fprintf (file, "\n\\end{equation}\n")