DEF_CMD(ADD, 101, {Node_t* node1 = NULL; 
                   StackPop (stk, &node1);
                   Node_t* node2 = NULL; 
                   StackPop (stk, &node2);
                   node1->value += node2->value;
                   StackPush (stk, node1); })

DEF_CMD(SUB, 102, {Node_t* node1 = NULL; 
                   StackPop (stk, &node1);
                   Node_t* node2 = NULL; 
                   StackPop (stk, &node2);
                   node1->value = node2->value - node1->value;
                   StackPush (stk, node1); })

DEF_CMD(MUL, 103, {Node_t* node1 = NULL; 
                   StackPop (stk, &node1);
                   Node_t* node2 = NULL; 
                   StackPop (stk, &node2);
                   node1->value *= node2->value;
                   StackPush (stk, node1); })

DEF_CMD(DIV, 104, {Node_t* node1 = NULL; 
                   StackPop (stk, &node1);
                   Node_t* node2 = NULL; 
                   StackPop (stk, &node2);
                   node1->value = node2->value / node1->value;
                   StackPush (stk, node1); })
