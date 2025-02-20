#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_EXPR 1024

const char *expr;  


void error() {
    fprintf(stderr, "Syntax Error\n");
    exit(1);
}


void skip_spaces() {
    while (*expr == ' ') expr++;
}


int number() {
    skip_spaces();
    int num = 0;
    if (!isdigit(*expr)) error();
    while (isdigit(*expr)) {
        num = num * 10 + (*expr - '0');
        expr++;
    }
    return num;
}


int factor() {
    skip_spaces();
    if (*expr == '(') {
        expr++;  
        int result = expression();
        if (*expr != ')') error();
        expr++;  
        return result;
    }
    return number();
}


int term() {
    int result = factor();
    while (1) {
        skip_spaces();
        if (*expr == '*') {
            expr++;
            result *= factor();
        } else if (*expr == '/') {
            expr++;
            int divisor = factor();
            if (divisor == 0) error();  
            result /= divisor;
        } else {
            break;
        }
    }
    return result;
}


int expression() {
    int result = term();
    while (1) {
        skip_spaces();
        if (*expr == '+') {
            expr++;
            result += term();
        } else if (*expr == '-') {
            expr++;
            result -= term();
        } else {
            break;
        }
    }
    return result;
}

int main() {
    static char input[MAX_EXPR];
    if (!fgets(input, MAX_EXPR, stdin)) return 1;  
    expr = input;
    printf("%d\n", expression());
    return 0;
}
