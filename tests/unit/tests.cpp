#include <gtest/gtest.h>
#include <cstring>

// Объявляем функции и глобальную переменную из исходного C-кода.
// Предполагается, что код калькулятора размещён в файле src/calculator.c и имеет C linkage.
extern "C" {
    // Функции для целочисленного режима
    int expression_int();
    // Функции для вещественного режима
    double expression_float();
    // Функция проверки допустимых символов
    void check_allowed_chars(const char *s);
    // Глобальный указатель, используемый для разбора выражения
    extern const char *expr;
}

// Тесты для целочисленного режима
TEST(ExpressionIntTest, SimpleAddition) {
    const char *input = "1+2";
    expr = input;
    EXPECT_EQ(expression_int(), 3);
}

TEST(ExpressionIntTest, MultiplicationSubtraction) {
    const char *input = "3*4-2";
    expr = input;
    // 3*4 = 12, 12 - 2 = 10
    EXPECT_EQ(expression_int(), 10);
}

TEST(ExpressionIntTest, Parentheses) {
    const char *input = "(1+2)*3";
    expr = input;
    EXPECT_EQ(expression_int(), 9);
}

TEST(ExpressionIntTest, Division) {
    const char *input = "5/2";
    expr = input;
    // Целочисленное деление: 5/2 = 2 (остаток отбрасывается)
    EXPECT_EQ(expression_int(), 2);
}

TEST(ExpressionIntTest, ExtraSpaces) {
    const char *input = "   1 +    2 ";
    expr = input;
    EXPECT_EQ(expression_int(), 3);
}

// Тесты для вещественного режима
TEST(ExpressionFloatTest, SimpleDivision) {
    const char *input = "5/2";
    expr = input;
    EXPECT_DOUBLE_EQ(expression_float(), 2.5);
}

TEST(ExpressionFloatTest, ParenthesesAndSpaces) {
    const char *input = " ( 3 + 2 ) / 5 ";
    expr = input;
    EXPECT_DOUBLE_EQ(expression_float(), 1.0);
}

// Тесты для функции проверки допустимых символов
TEST(CheckAllowedCharsTest, ValidChars) {
    // Проверка корректной строки не должна приводить к ошибке.
    ASSERT_NO_FATAL_FAILURE(check_allowed_chars("1+2*(3-4)/5"));
}

TEST(CheckAllowedCharsTest, InvalidChars) {
    // Если в строке присутствует недопустимый символ (например, буква 'a'),
    // функция должна вызвать error() и завершить выполнение.
    EXPECT_DEATH(check_allowed_chars("1+2a*(3-4)/5"), "Syntax Error");
}

// Тесты на деление на ноль (ожидается завершение работы с ошибкой)
TEST(ExpressionIntTest, DivisionByZero) {
    const char *input = "1/0";
    expr = input;
    EXPECT_DEATH({ expression_int(); }, "Syntax Error");
}

TEST(ExpressionFloatTest, DivisionByZero) {
    const char *input = "1/0";
    expr = input;
    EXPECT_DEATH({ expression_float(); }, "Syntax Error");
}
