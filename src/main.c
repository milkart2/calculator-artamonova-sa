#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXPR 1024

const char* expr;

void error()
{
    fprintf(stderr, "Syntax Error\n");
    exit(1);
}

void skip_spaces()
{
    while (*expr && isspace(*expr))
        expr++;
}

void check_allowed_chars(const char* s)
{
    while (*s) {
        if (!isdigit(*s) && !isspace(*s) && *s != '(' && *s != ')' &&
            *s != '*' && *s != '+' && *s != '-' && *s != '/')
            error();
        s++;
    }
}

int number_int();
int factor_int();
int term_int();
int expression_int();

int number_int()
{
    skip_spaces();
    int num = 0;
    if (!isdigit(*expr))
        error();
    while (isdigit(*expr)) {
        int digit = *expr - '0';
        num = num * 10 + digit;
        if (num > 2000000000)
            error();
        expr++;
    }
    return num;
}

int factor_int()
{
    skip_spaces();
    if (*expr == '(') {
        expr++;
        int result = expression_int();
        skip_spaces();
        if (*expr != ')')
            error();
        expr++;
        return result;
    }
    return number_int();
}

int term_int()
{
    int result = factor_int();
    while (1) {
        skip_spaces();
        if (*expr == '*') {
            expr++;
            result *= factor_int();
        } else if (*expr == '/') {
            expr++;
            int divisor = factor_int();
            if (divisor == 0)
                error();
            result /= divisor;
        } else {
            break;
        }
    }
    return result;
}

int expression_int()
{
    int result = term_int();
    while (1) {
        skip_spaces();
        if (*expr == '+') {
            expr++;
            result += term_int();
        } else if (*expr == '-') {
            expr++;
            result -= term_int();
        } else {
            break;
        }
    }
    return result;
}

double number_float();
double factor_float();
double term_float();
double expression_float();

double number_float()
{
    skip_spaces();
    double num = 0.0;
    if (!isdigit(*expr))
        error();
    while (isdigit(*expr)) {
        int digit = *expr - '0';
        num = num * 10 + digit;
        if (num > 2000000000.0)
            error();
        expr++;
    }
    return num;
}

double factor_float()
{
    skip_spaces();
    if (*expr == '(') {
        expr++;
        double result = expression_float();
        skip_spaces();
        if (*expr != ')')
            error();
        expr++;
        return result;
    }
    return number_float();
}

double term_float()
{
    double result = factor_float();
    while (1) {
        skip_spaces();
        if (*expr == '*') {
            expr++;
            result *= factor_float();
        } else if (*expr == '/') {
            expr++;
            double divisor = factor_float();
            double abs_divisor = divisor < 0 ? -divisor : divisor;
            if (abs_divisor < 1e-9)
                error();
            result /= divisor;
        } else {
            break;
        }
    }
    return result;
}

double expression_float()
{
    double result = term_float();
    while (1) {
        skip_spaces();
        if (*expr == '+') {
            expr++;
            result += term_float();
        } else if (*expr == '-') {
            expr++;
            result -= term_float();
        } else {
            break;
        }
    }
    return result;
}

#ifndef UNIT_TEST
int main(int argc, char* argv[])
{
    char input[MAX_EXPR];

    if (fgets(input, MAX_EXPR, stdin) == NULL)
        error();


    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n')
        input[len - 1] = '\0';


    check_allowed_chars(input);

    expr = input;
    skip_spaces();

    int is_float = 0;
    if (argc > 1 && strcmp(argv[1], "--float") == 0)
        is_float = 1;

    if (is_float) {
        double result = expression_float();
        skip_spaces();
        if (*expr != '\0')
            error();

        printf("%.4f\n", result);
    } else {
        int result = expression_int();
        skip_spaces();
        if (*expr != '\0')
            error();
        printf("%d\n", result);
    }
    return 0;
}
#endif
