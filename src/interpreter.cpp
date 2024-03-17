#include "interpreter.h"
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

std::string Interpreter::compile() {
    std::string error;
    std::string to_print = "";
    std::string var_value;
    std::string param1;
    std::string param2;
    std::string sign;
    bool is_param1_int;
    bool is_param2_int;
    bool is_var_int;
    bool is_in_condition = false;
    std::stringstream ss(m_text);
    std::queue<std::string> words;

    std::string word;
    while (ss >> std::ws >> word) {
        words.push(word);
    }

    if (words.empty())
        return "";

    word = words.front();
    words.pop();
    if (word != BEGIN) {
        return "Язык должен начинаться словом - начало";
    }

    while (!words.empty()) {
        word = words.front();
        words.pop();

        if (words.empty() && word != END) {
            return END_ERROR;
        } else if (word == PRINT) {
            if (words.empty()) {
                return PRINT_ERROR;
            }

            word = words.front();
            words.pop();

            if (((*word.begin() != '\'' && *(word.end() - 1) != '\'') &&
                 !is_int(word) && !is_float(word)) &&
                (m_integerVars.find(word) == m_integerVars.end() &&
                 m_floatVars.find(word) == m_floatVars.end())) {
                return PRINT_ERROR;
            }

            if ((*word.begin() == '\'' && *(word.end() - 1) == '\'')) {
                to_print += word.substr(1, word.size() - 2);
            } else if (is_int(word) || is_float(word)) {
                to_print += word;
            } else if (m_integerVars.find(word) != m_integerVars.end()) {
                to_print += m_integerVars.at(word);
            } else if (m_floatVars.find(word) != m_floatVars.end()) {
                to_print += m_floatVars.at(word);
            }
            to_print += "\n";
        } else if (word == CONDITION) {
            if (words.empty()) {
                return CONDITION_ERROR;
            }

            param1 = words.front();
            words.pop();

            if (is_in_condition) {
                return CONDITION_IN_CONDITION_ERROR;
            }

            if (!is_int(param1) && !is_float(param1) &&
                m_integerVars.find(param1) == m_integerVars.end() &&
                m_floatVars.find(param1) == m_floatVars.end()) {
                return CONDITION_PARAM_ERROR + " (" + param1 + ")";
            }

            if (words.empty()) {
                return CONDITION_SIGN_ERROR;
            }

            sign = words.front();
            words.pop();

            if (!is_condition_sign(sign)) {
                return CONDITION_SIGN_ERROR + " (" + sign + ")";
            }

            if (words.empty()) {
                return CONDITION_ERROR;
            }

            param2 = words.front();
            words.pop();

            if (!is_int(param2) && !is_float(param2) &&
                m_integerVars.find(param2) == m_integerVars.end() &&
                m_floatVars.find(param2) == m_floatVars.end()) {
                return CONDITION_PARAM_ERROR + " (" + param2 + ")";
            }

            if (!get_codition_result(sign, param1, param2)) {
                while (!words.empty() && words.front() != CONDITION_END) {
                    if (words.front() == END) {
                        return CONDITION_END_ERROR;
                    }
                    words.pop();
                }
            }

            is_in_condition = true;

        } else if (word == LOOP) {
        } else if (word == INTEGER_VAR) {
            if (words.empty()) {
                return INTEGER_VAR_ERROR;
            }

            word = words.front();
            words.pop();

            if (is_key_word(word) || is_int(word)) {
                return NAME_ERROR;
            }

            if (words.empty() || words.front() != ":=") {
                return OPP_ERROR;
            }

            words.pop();

            var_value = words.front();
            words.pop();

            if (!is_int(var_value) &&
                m_integerVars.find(var_value) == m_integerVars.end()) {
                return INTEGER_VAL_ERROR;
            }

            if (is_int(var_value)) {
                m_integerVars.insert({word, var_value});
            }
        } else if (word == FLOAT_VAR) {
            if (words.empty()) {
                return FLOAT_VAR_ERROR;
            }

            word = words.front();
            words.pop();

            if (is_key_word(word) || is_float(word)) {
                return NAME_ERROR;
            }

            if (words.empty() || words.front() != ":=") {
                return OPP_ERROR;
            }

            words.pop();

            var_value = words.front();
            words.pop();

            if (!is_float(var_value) &&
                m_floatVars.find(var_value) == m_floatVars.end()) {
                return FLOAT_VAL_ERROR;
            }

            if (is_float(var_value)) {
                m_floatVars.insert({word, var_value});
            }
        } else if (word == CONDITION_END) {
            if (is_in_condition) {
                is_in_condition = false;
            } else {
                return END_WITHOUT_CONDITION;
            }
        } else if (word == END && is_in_condition) {
            return END_ERROR;
        } else if (m_integerVars.find(word) != m_integerVars.end() ||
                   m_floatVars.find(word) != m_floatVars.end()) {
            if (words.empty()) {
                return OPP_ERROR;
            }

            if (words.empty() || words.front() != ":=") {
                return OPP_ERROR;
            }

            words.pop();

            if (m_integerVars.find(word) != m_integerVars.end()) {
                is_var_int = true;
            } else {
                is_var_int = false;
            }

            param1 = words.front();
            words.pop();

            if (!is_int(param1) && !is_float(param1) &&
                m_integerVars.find(param1) == m_integerVars.end() &&
                m_floatVars.find(param1) == m_floatVars.end()) {
                return EXPRESSION_PARAM_ERROR + " (" + param1 + ")";
            }

            if (is_int(param1) ||
                m_integerVars.find(param1) != m_integerVars.end()) {
                is_param1_int = true;
            } else {
                is_param1_int = false;
            }

            if (words.empty()) {
                return END_ERROR;
            }

            if (is_var_int && is_var_int != is_param1_int) {
                return INT_VAR_INIT_ERROR + " (" + word + " := " + param1 + ")";
            } else if (!is_var_int && is_var_int != is_param1_int) {
                return FLOAT_VAR_INIT_ERROR + " (" + word + " := " + param1 +
                       ")";
            }

            param2 = words.front();
            if (is_key_word(param2) ||
                m_integerVars.find(param2) != m_integerVars.end() ||
                m_floatVars.find(param2) != m_floatVars.end()) {
                if (is_var_int) {
                    if (is_int(param1))
                        m_integerVars.insert({word, param1});
                    else
                        m_integerVars.insert({word, m_integerVars.at(param1)});
                } else {
                    if (is_float(param1))
                        m_floatVars.insert({word, param1});
                    else
                        m_floatVars.insert({word, m_floatVars.at(param1)});
                }
                continue;
            }

            sign = words.front();
            words.pop();

            if (!is_expression_sign(sign)) {
                return EXPRESSION_SIGN_ERROR;
            }

            if (words.empty()) {
                return CONDITION_ERROR;
            }

            param2 = words.front();
            words.pop();

            if (!is_int(param2) && !is_float(param2) &&
                m_integerVars.find(param2) == m_integerVars.end() &&
                m_floatVars.find(param2) == m_floatVars.end()) {
                return EXPRESSION_PARAM_ERROR + " (" + param2 + ")";
            }

            if (is_int(param2) ||
                m_integerVars.find(param2) != m_integerVars.end()) {
                is_param2_int = true;
            } else {
                is_param2_int = false;
            }

            if (is_param1_int && is_param1_int != is_param2_int) {
                return INT_EXPRESSION_ERROR + " (" + param1 + " " + sign + " " +
                       param2 + ")";
            } else if (!is_param1_int && is_param1_int != is_param2_int) {
                return FLOAT_EXPRESSION_ERROR + " (" + param1 + " " + sign +
                       " " + param2 + ")";
            }

            if (is_var_int) {
                m_integerVars.at(word) =
                    get_expression_result(sign, param1, param2);
            } else {
                m_floatVars.at(word) =
                    get_expression_result(sign, param1, param2);
            }
        } else if (!is_key_word(word)) {
            return VARIABLE_NOT_INITIALIZED + " (" + word + ")";
        }
    }

    m_integerVars = {};
    m_floatVars = {};

    return to_print;
}

void Interpreter::set_text(std::string &text) { m_text = text; }

bool Interpreter::is_int(const std::string &word) const {
    auto it = word.begin();
    while (it < word.end() && is_digit(*it)) {
        ++it;
    }
    return !word.empty() && it == word.end();
}

bool Interpreter::is_float(const std::string &word) const {
    auto it = word.begin();
    if (*it == '.')
        return false;
    while (it < word.end() && (is_digit(*it) || *it == '.')) {
        ++it;
    }
    return !word.empty() && it == word.end() && *(it - 1) != '.';
}

bool Interpreter::is_key_word(const std::string &word) const {
    if (word == BEGIN || word == END || word == PRINT || word == LOOP ||
        word == LOOP_END || word == CONDITION || word == CONDITION_END ||
        word == INTEGER_VAR || word == FLOAT_VAR) {
        return true;
    }
    return false;
}

bool Interpreter::is_digit(const char &c) const {
    switch (c) {
    case '0' ... '9':
        return true;
    default:
        return false;
    }
}

bool Interpreter::is_condition_sign(const std::string &word) const {
    return word == ">" || word == "<" || word == ">=" || word == "<=" ||
           word == "==" || word == "<>";
}

bool Interpreter::get_codition_result(
    const std::string &sign, const std::string &condition_param1,
    const std::string &condition_param2) const {
    float param1;
    float param2;

    if (m_integerVars.find(condition_param1) != m_integerVars.end()) {
        param1 = std::stof(m_integerVars.at(condition_param1));
    } else if (m_floatVars.find(condition_param1) != m_floatVars.end()) {
        param1 = std::stof(m_floatVars.at(condition_param1));
    } else {
        param1 = std::stof(condition_param1);
    }

    if (m_integerVars.find(condition_param2) != m_integerVars.end()) {
        param2 = std::stof(m_integerVars.at(condition_param2));
    } else if (m_floatVars.find(condition_param2) != m_floatVars.end()) {
        param2 = std::stof(m_floatVars.at(condition_param2));
    } else {
        param2 = std::stof(condition_param2);
    }

    if (sign == ">")
        return param1 > param2;
    if (sign == "<")
        return param1 < param2;
    if (sign == ">=")
        return param1 >= param2;
    if (sign == "<=")
        return param1 <= param2;
    if (sign == "==")
        return param1 == param2;
    if (sign == "<>")
        return param1 != param2;
    return false;
}

bool Interpreter::is_expression_sign(const std::string &word) const {
    return word == "/" || word == "*" || word == "+" || word == "-";
}

std::string
Interpreter::get_expression_result(const std::string &sign,
                                   const std::string &expression_param1,
                                   const std::string &expression_param2) const {
    if (m_integerVars.find(expression_param1) != m_integerVars.end()) {
        return expression_result(
            sign,
            stoi(is_int(expression_param1)
                     ? expression_param1
                     : m_integerVars.at(expression_param1)),
            stoi(is_int(expression_param2)
                     ? expression_param2
                     : m_integerVars.at(expression_param2)));
    } else {
        return expression_result(sign,
                                 stoi(is_float(expression_param1)
                                          ? expression_param1
                                          : m_floatVars.at(expression_param1)),
                                 stoi(is_float(expression_param2)
                                          ? expression_param2
                                          : m_floatVars.at(expression_param2)));
    }
}

template <typename T>
std::string Interpreter::expression_result(const std::string &sign,
                                           const T param1,
                                           const T param2) const {
    if (sign == "+")
        return std::to_string(param1 + param2);
    if (sign == "-")
        return std::to_string(param1 - param2);
    if (sign == "/")
        return std::to_string(param1 / param2);
    if (sign == "*")
        return std::to_string(param1 * param2);
    return "";
}
