// DEF_CMD(ADD, "+", 43, {CreateNode (OP, '+', })

// DEF_CMD(SUB, "-", 45, {Node_t* node1 = NULL; 
//                    StackPop (stk, &node1);
//                    Node_t* node2 = NULL; 
//                    StackPop (stk, &node2);
//                    node1->value = node2->value - node1->value;
//                    StackPush (stk, node1); })

// DEF_CMD(MUL, "*", 42, {Node_t* node1 = NULL; 
//                    StackPop (stk, &node1);
//                    Node_t* node2 = NULL; 
//                    StackPop (stk, &node2);
//                    node1->value *= node2->value;
//                    StackPush (stk, node1); })

// DEF_CMD(DIV, "/", 47, {Node_t* node1 = NULL; 
//                    StackPop (stk, &node1);
//                    Node_t* node2 = NULL; 
//                    StackPop (stk, &node2);
//                    node1->value = node2->value / node1->value;
//                    StackPush (stk, node1); })


