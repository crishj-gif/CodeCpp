#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

typedef struct stack {
    char item;
    struct stack *next; 
} stack;

void init(stack **s){
    (*s) = NULL;
}

int isEmpty(stack*s){
    if(s == NULL) return 1;
    return 0;
}

int push(stack**s, char value){
    stack *a = (stack*)malloc(sizeof(stack));
    if(a == NULL) return -1;
    a->item = value;
    a->next = (*s);
    (*s) = a;
    return 0;
}

int pop(stack **s, char *out){
    stack* cur;
    if(isEmpty(*s)) return -1;
    *out = (*s)->item;
    cur = (*s);
    (*s) = cur->next;
    free(cur);
    return 0;
}

int peek(stack* s, char* out) {
    if (isEmpty(s)) return -1;
    *out = s->item;
    return 0;
}

int isOperator(char x){
    if(x == '+' || x == '-' || x == '*' || x == '/') return 1;
    return 0;
}

int isnum(char x){

    if (x >= '0' && x <= '9'){
        return 1;
    }
    return 0;
}

int getPriority(char x){
    if(x == '+' || x == '-') return 1;
    else if(x == '*' || x == '/') return 2;
    return 0;
}

int checkParenthese(char* expression){
    stack *parenstack;
    init(&parenstack);
    char trash;
    
    for(int i=0; expression[i] != '\0'; i++){
        char ch = expression[i];
        if(ch == '('){
            push(&parenstack, ch);
        }
        else if(ch == ')'){
            if(isEmpty(parenstack)) return 0;
            pop(&parenstack, &trash);
        }
    } 
    int a=isEmpty(parenstack);
    while(!isEmpty(parenstack)) pop(&parenstack, &trash);
    return a;
}

void infixtopostfix(char *infix, char* postfix){
    stack* opstack;
    init(&opstack);
    char ch;
    char popped;
    char topoperator;
    int k = 0;
    
    for(int i=0; infix[i] != '\0'; i++){
        ch = infix[i];
        if(isnum(ch)) {
            postfix[k++] = ch;
        }
        else if(ch == '(') {
            push(&opstack, ch);
            postfix[k++] = ' ';
        }
        else if(ch == ')'){
            while(!isEmpty(opstack) && peek(opstack, &topoperator) == 0 && topoperator != '('){
                pop(&opstack, &popped);
                postfix[k++] = popped;
            }
            if (!isEmpty(opstack)) {
                pop(&opstack, &popped);
            }
        }
        else if(isOperator(ch)){
            while(!isEmpty(opstack) && peek(opstack, &topoperator) == 0 && topoperator != '(' && getPriority(topoperator) >= getPriority(ch)){
                pop(&opstack, &popped);
                postfix[k++] = popped;
                postfix[k++] = ' ';
            }
            push(&opstack, ch);
        }
    }
    
    while (!isEmpty(opstack)){
        pop(&opstack, &popped);
        postfix[k++] = popped;
        postfix[k++] = ' ';
    }
    postfix[k++] = '\0'; 
}
int evaluatePostfix(char* postfix) {
    int arrStack[100]; 
    int top = -1;      
    
    for (int i = 0; postfix[i] != '\0'; i++) {
        char ch = postfix[i]; 
        
        if (isnum(ch)) {
            top++;
            arrStack[top] = ch - '0'; 
        } 
        else if (isOperator(ch)) {
            int val2 = arrStack[top--]; 
            int val1 = arrStack[top--]; 
            
            int result = 0;
            if (ch == '+') result = val1 + val2;
            else if (ch == '-') result = val1 - val2;
            else if (ch == '*') result = val1 * val2;
            else if (ch == '/') result = val1 / val2;
            
            top++; 
            arrStack[top] = result; 
        }
    }
    
    return arrStack[top]; 
}

int main() {
    char infixExpression[100];
    char postfixResult[100]; 
    printf("Nhap bieu thuc trung to: ");
    fgets(infixExpression, 100, stdin);
    infixExpression[strcspn(infixExpression, "\n")] = 0; 
    if (strlen(infixExpression) == 0) {
        printf("Loi!\n");
        return 0;
    }
    if (!checkParenthese(infixExpression)) {
        printf("Loi!\n");
    } else {
        infixtopostfix(infixExpression, postfixResult);
        int kq=evaluatePostfix(postfixResult);
        printf("Bieu thuc hau to: %s\n", postfixResult);
        printf("Ket qua phep tinh: %d\n", kq);
    }
    return 0;
}