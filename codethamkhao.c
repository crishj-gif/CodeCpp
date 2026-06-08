#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct stack {
    char item;
    struct stack* next; 
} stack;

void init(stack** s) {
    *s = NULL;
}

int isEmpty(stack* s) {
    if (s == NULL) {
        return 1;
    } else {
        return 0;
    }
}

int push(stack** s, char value) {
    stack* a;
    a = (stack*)malloc(sizeof(stack));
    if (a == NULL) {
        return -1;
    }
    a->item = value;
    a->next = *s;
    *s = a; 
    return 0;
}

int pop(stack** s, char* out) {
    stack* cur;
    if (isEmpty(*s)) {
        return -1;
    }
    *out = (*s)->item;
    cur = *s;
    *s = cur->next;
    free(cur);
    return 0;
}


int peek(stack* s, char* out) {
    if (isEmpty(s)) {
        return -1;
    }
    *out = s->item;
    return 0;
}

int isOperator(char x) {//kiem tra toan tu
    if (x == '+' || x == '-' || x == '*' || x == '/') return 1;
    return 0;
}

int getPriority(char x) {//xac dinh uu tien toan tu
    if (x == '+' || x == '-') return 1;
    if (x == '*' || x == '/') return 2;
    return 0;
}

int checkParentheses(char* expression) {
    stack* parenStack;
    init(&parenStack);
    char trash;
    int i;
    for (i = 0; expression[i] != '\0'; i++) {
        char ch = expression[i];
        if (ch == '(') {
            push(&parenStack, '(');
        } else if (ch == ')') {
            if (isEmpty(parenStack)) return 0;
            pop(&parenStack, &trash);
        }
    }
    int result = isEmpty(parenStack);
    while (!isEmpty(parenStack)) pop(&parenStack, &trash);
    return result;
}

void infixToPostfix(char* infix, char* postfix) {
    stack* opStack;
    init(&opStack);
    char ch;
    char poppedOperator;
    char topOperator;
    int k = 0;
    int i;

    for (i = 0; infix[i] != '\0'; i++) {
        ch = infix[i];

        if (isalnum(ch)) {
            postfix[k++] = ch;
        } else if (ch == '(') {
            push(&opStack, '(');
        } else if (ch == ')') {
            while (!isEmpty(opStack) && peek(opStack, &topOperator) == 0 && topOperator != '(') {
                pop(&opStack, &poppedOperator);
                postfix[k++] = poppedOperator;
            }
            if (!isEmpty(opStack) && peek(opStack, &topOperator) == 0 && topOperator == '(') {
                pop(&opStack, &poppedOperator);
            }
        } else {
            while (!isEmpty(opStack) && peek(opStack, &topOperator) == 0 && topOperator != '(' &&
                   getPriority(topOperator) >= getPriority(ch)) {
                pop(&opStack, &poppedOperator);
                postfix[k++] = poppedOperator;
            }
            push(&opStack, ch);
        }
    }

    while (!isEmpty(opStack)) {
        pop(&opStack, &poppedOperator);
        postfix[k++] = poppedOperator;
    }
    postfix[k] = '\0';
}

int main() {
    char infixExpression[100];
    char postfixResult[100];
    printf("%d\n",'9');

    printf("Nhap bieu thuc trung to: ");
    fgets(infixExpression, 100, stdin);
    infixExpression[strcspn(infixExpression, "\n")] = '\0';

    if (strlen(infixExpression) == 0) {
        printf("Huy bo!\n");
        return 0;
    }

    if (!checkParentheses(infixExpression)) {
        printf("Loi!\n");
    } else {
        infixToPostfix(infixExpression, postfixResult);
        printf("%s\n", postfixResult);
    }
    

    return 0;
}