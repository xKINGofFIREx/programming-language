#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <map>
#include <string>

class Interpreter {

  private:
    std::string m_text;
    std::map<std::string, std::string> m_integerVars;
    std::map<std::string, std::string> m_floatVars;

    const std::string BEGIN = "начало";
    const std::string END = "конец";
    const std::string PRINT = "печать";
    const std::string LOOP = "пока";
    const std::string LOOP_END = "конецпока";
    const std::string CONDITION = "если";
    const std::string CONDITION_END = "конецесли";
    const std::string INTEGER_VAR = "целое";
    const std::string FLOAT_VAR = "вещественное";

    const std::string END_ERROR = "Язык должен оканчиваться словом - конец";
    const std::string PRINT_ERROR =
        "После печати должны находиться строка, число или "
        "переменная для печати";

    const std::string INTEGER_VAL_ERROR =
        "После переменной целого типа должны находиться целое число или "
        "переменная того же "
        "типа";
    const std::string INTEGER_VAR_ERROR =
        "После слова целое должно находиться название переменной";

    const std::string OPP_ERROR =
        "После переменной должен находиться оператор присвоения (:=)";
    const std::string NAME_ERROR =
        "Название переменной не может быть зарезервированным словом или числом";

    const std::string FLOAT_VAR_ERROR =
        "После слова вещественное должно находиться название переменной";
    const std::string FLOAT_VAL_ERROR =
        "После переменной вещественного типа должны находиться вещественное "
        "число или "
        "переменная того же типа";

    const std::string CONDITION_ERROR = "После если должно быть условие";
    const std::string CONDITION_PARAM_ERROR =
        "В условие могут находиться только переменныe, целые и вещественные "
        "числа";
    const std::string CONDITION_SIGN_ERROR =
        "В условии должен быть оператор сравнения (>, <, >=, <=, ==, <>)";
    const std::string CONDITION_END_ERROR =
        "Условие должно заканчиваться словом - конецесли";
    const std::string END_WITHOUT_CONDITION =
        "конецесли нельзя использовать без условия";
    const std::string CONDITION_IN_CONDITION_ERROR =
        "Условие не может быть вызванно внутри условия";

    const std::string INT_EXPRESSION_ERROR =
        "Операция не может быть выполнена, первый параметр целое число, второй "
        "параметр вещественное число";
    const std::string FLOAT_EXPRESSION_ERROR =
        "Операция не может быть выполнена, первый параметр вещественное число, "
        "второй параметр целое число";
    const std::string EXPRESSION_PARAM_ERROR =
        "В выражении могут находиться только переменныe, целые и вещественные "
        "числа";
    const std::string EXPRESSION_SIGN_ERROR =
        "В выражении можно использовать только операторы (+, -, *, /)";

    const std::string VARIABLE_NOT_INITIALIZED =
        "Переменная не инициализирована";
    const std::string INT_VAR_INIT_ERROR =
        "Переменной целого типа нельзя присвоить число вещественного типа";
    const std::string FLOAT_VAR_INIT_ERROR =
        "Переменной вещественного типа нельзя присвоить число целого типа";

    template <typename T>
    std::string expression_result(const std::string &sign, const T param1,
                                  const T param2) const;

  public:
    bool is_int(const std::string &word) const;
    bool is_float(const std::string &word) const;
    bool is_key_word(const std::string &word) const;
    bool is_digit(const char &c) const;

    bool is_condition_sign(const std::string &word) const;
    bool is_expression_sign(const std::string &word) const;
    bool get_codition_result(const std::string &sign,
                             const std::string &condition_param1,
                             const std::string &condition_param2) const;
    std::string
    get_expression_result(const std::string &sign,
                          const std::string &expression_param1,
                          const std::string &expression_param2) const;

    void set_text(std::string &text);
    std::string compile();
};

#endif // interpreter.h
