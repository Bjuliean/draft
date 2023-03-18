#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

typedef struct s_stack {
    char action[255];
    struct s_stack *next;
} t_stack;

void postfix(char *label);
void push_front_stack(t_stack **stack, char *set_action);
void killstack(t_stack *stack);
void del_stack_element(t_stack **stack);
bool is_digit(char c);
void add_digit(char *label, char *str, int *i, int *counter);
void get_action(char *action_buffer, char *label, int *i, int *current_block);
int get_priority_value(char *action_buffer);
bool is_simple_action(char c);
bool is_less_or_equal_priority(t_stack **stack, char *action_buffer);
void translate_action_to_exit(t_stack **stack, char *exit, int *arr_counter_exit);

t_stack *create_node(char *set_action) {
    t_stack *node = (t_stack *)malloc(sizeof(t_stack));
    strcpy(node->action, set_action);
    node->next = NULL;
    return node;
}

int main(void) {

    char label[255] = "(sin(cos(2*2))*24/sqrt(10))/(5+125)";
    // 2 2 * cos sin 24 * 10 sqrt / 5 125 + /
    // 2 2 * cos sin 24 * 10 sqrt /
    // 2 4 7 * + 12 - 34 6 ^ +
    // 2 1124 314 - + 12 2 * +
    // 0.35 12 + 4 4 * - 3 45 ^ + 444 132 / +
    postfix(label);
    //printf("CC %c\n", label[15]);
    // if(is_digit(label[7]) == true) {
    //     printf("+ %c\n", label[7]);
    // }

    // char a = '(';

    // if(is_digit(a) == true) {
    //     printf("TRUE");
    // } else {
    //     printf("FALSE");
    // }

    return 0;
}

void push_front_stack(t_stack **stack, char *set_action) {
    t_stack *new_element = create_node(set_action);
    new_element->next = *stack;
    *stack = new_element;
}

void del_stack_element(t_stack **stack) {
    t_stack *temp = *stack;
    *stack = (*stack)->next;
    free(temp);
}

void killstack(t_stack *stack) {
    t_stack *temp;
    while(stack) {
        temp = stack;
        stack = stack->next;
        free(temp);
    }
}

bool is_digit(char c) {
    return (c >= '0' && c <= '9') || c == '.' ? true : false;
}

void postfix(char *label) {
    t_stack *stack = create_node("end");
    char exit[255] = {0};
    char action_buffer[255] = {0};
    int arr_counter_exit = 0, current_block = 0;

    // for(int i = 0; label[i] != '\0'; i++) {
    //     if(is_digit(label[i]) == true) {
    //         add_digit(label, exit, &i, &arr_counter_exit);
    //     } else {
    //         get_action(action_buffer, label, &i);
    //         if(is_less_or_equal_priority(&stack, action_buffer) == false) {
    //             push_front_stack(&stack, action_buffer);
    //         } else {
    //             translate_action_to_exit(&stack, exit, &arr_counter_exit);
    //             push_front_stack(&stack, action_buffer);
    //         }
    //     }
    // }

    for(int i = 0; i < strlen(label); i++) {
        if(is_digit(label[i]) == true) {
            add_digit(label, exit, &i, &arr_counter_exit);
        } else {
            get_action(action_buffer, label, &i, &current_block);
            if(action_buffer[strlen(action_buffer) - 1] == ' ') {
                action_buffer[strlen(action_buffer) - 1] = 0;
                if(is_less_or_equal_priority(&stack, action_buffer) == true) {
                        while(strcmp(stack->action, "end") != 0) {
                            if(strcmp(stack->action, "(") == 0)
                                break;
                            translate_action_to_exit(&stack, exit, &arr_counter_exit);
                            del_stack_element(&stack);
                        }
                }
                    if(strcmp(action_buffer, ")") == 0) {
                        while(strcmp(stack->action, "(") != 0) {
                            translate_action_to_exit(&stack, exit, &arr_counter_exit);
                            del_stack_element(&stack);
                        }
                        del_stack_element(&stack);
                        current_block = 0;
                        memset(action_buffer, 0, 255);
                    } else {
                        push_front_stack(&stack, action_buffer);
                        current_block = 0;
                        memset(action_buffer, 0, 255);
                    }
            }
        }
    }

    while(stack != NULL) {
        if(strcmp(stack->action, "end") != 0 /*&& strcmp(stack->action, "(") != 0 && strcmp(stack->action, ")") != 0*/) {
            strcat(exit, stack->action);
            strcat(exit, " ");
        }
        del_stack_element(&stack);
    }

    printf("exit %s\n", exit);
    printf("act %s\n", action_buffer);

    // while(stack != NULL) {
    //     printf("stack %s\n", stack->action);
    //     stack = stack->next;
    // }
    // // killstack(stack);
}

void translate_action_to_exit(t_stack **stack, char *exit, int *arr_counter_exit) {
    //printf("CHECK %s\n", (*stack)->action);
    strcat(exit, (*stack)->action);
    strcat(exit, " ");
    *arr_counter_exit = strlen(exit);
}

bool is_less_or_equal_priority(t_stack **stack, char *action_buffer) {
    bool res = false;
    if(strcmp((*stack)->action, "end") == 0)
        res = false;
    else {
        if(get_priority_value((*stack)->action) < get_priority_value(action_buffer))
            res = false;
        else if(get_priority_value((*stack)->action) >= get_priority_value(action_buffer))
            res = true;
    }
    if(strcmp((*stack)->action, "(") == 0 || strcmp(action_buffer, "(") == 0)
        res = false;
    if(strcmp((*stack)->action, ")") == 0 || strcmp(action_buffer, ")") == 0)
        res = false;
    // if(res == true) {
    //     printf("CHECK ACT |%s| && ST |%s| == TRUE\n", action_buffer, (*stack)->action);
    //     printf("ACT V %d | ST V %d\n\n", get_priority_value(action_buffer), get_priority_value((*stack)->action));
    // } else {
    //     printf("CHECK ACT |%s| && ST |%s| == FALSE\n", action_buffer, (*stack)->action);
    //     printf("ACT V %d | ST V %d\n\n", get_priority_value(action_buffer), get_priority_value((*stack)->action));
    // }
    return res;
}

int get_priority_value(char *action_buffer) {
    int res = 0;
    if(strcmp(action_buffer, "+") == 0)
        res = 1;
    if(strcmp(action_buffer, "-") == 0)
        res = 1;
    if(strcmp(action_buffer, "*") == 0)
        res = 2;
    if(strcmp(action_buffer, "/") == 0)
        res = 2;
    if(strcmp(action_buffer, "^") == 0)
        res = 3;
    if(strcmp(action_buffer, "sin") == 0)
        res = 4;
    if(strcmp(action_buffer, "cos") == 0)
        res = 4;
    if(strcmp(action_buffer, "tan") == 0)
        res = 4;
    if(strcmp(action_buffer, "asin") == 0)
        res = 4;
    if(strcmp(action_buffer, "acos") == 0)
        res = 4;
    if(strcmp(action_buffer, "atan") == 0)
        res = 4;
    if(strcmp(action_buffer, "sqrt") == 0)
        res = 4;
    if(strcmp(action_buffer, "ln") == 0)
        res = 4;
    if(strcmp(action_buffer, "log") == 0)
        res = 4;
    if(strcmp(action_buffer, "(") == 0)
        res = 5;
    if(strcmp(action_buffer, ")") == 0)
        res = 5;
    return res;
}

bool is_simple_action(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%' || c == '(' || c == ')' ? true : false;
}

void get_action(char *action_buffer, char *label, int *i, int *current_block) {
    // char temp[255] = {0};
    // int temp_counter = 0, copy_i = *i;
    // memset(action_buffer, 0, 255);
    // if(is_simple_action(label[*i]) == true) {
    //     temp[0] = label[*i];
    // } else {
    //     while(is_digit(label[copy_i]) == false) {
    //         if(label[copy_i] == '(' || label[copy_i] == ')')
    //             break;
    //         temp[temp_counter] = label[copy_i];
    //         temp_counter++;
    //         copy_i++;
    //     }
    // }
    // //*i = *i + temp_counter + 1;
    // strcpy(action_buffer, temp);
    // memset(action_buffer, 0, 255);
    // for(int z = 0; is_digit(label[*i]) == false; *i = *i + 1, z++) {
    //     action_buffer[z] = label[*i];
    // }
    // //*i = *i - 1;

            action_buffer[*current_block] = label[*i];
            *current_block = * current_block + 1;
            if(is_simple_action(label[*i]) == true || is_simple_action(label[*i + 1]) == true) {
                action_buffer[*current_block] = ' ';
                *current_block = * current_block + 1;
            }
}

void add_digit(char *label, char *str, int *i, int *counter) {
    // for(; is_digit(label[*i]) == true; *i = *i + 1, *counter = *counter + 1) {
    //     str[*counter] = label[*i];
    // }
    // str[*counter] = ' ';
    // *counter = *counter + 1;
    // //*i = *i - 1;

            str[*counter] = label[*i];
            *counter = *counter + 1;
            if(is_digit(label[*i + 1]) == false) {
                str[*counter] = ' ';
                *counter = *counter + 1;
            }
}