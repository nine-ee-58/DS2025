#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <map>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

class AdvancedExpressionEvaluator {
private:
    // 运算符优先级关系表 - 扩展版本
    map<char, map<char, char>> priorityTable;
    
    // 支持的函数列表
    vector<string> supportedFunctions = {"sqrt", "ln", "log", "sin", "cos", "tan", "asin", "acos", "atan", "exp", "abs", "pow"};
    
    // 函数到名称的映射
    map<string, string> functionDescriptions;
    
public:
    AdvancedExpressionEvaluator() {
        initializePriorityTable();
        initializeFunctionDescriptions();
    }
    
private:
    void initializePriorityTable() {
        // 扩展运算符集合：+ - * / ^ ! ( ) #
        vector<char> operators = {'+', '-', '*', '/', '^', '!', '(', ')', '#'};
        
        // 重新初始化优先级表，基于经典算法设计
        // 关系: '>' 弹出计算, '<' 入栈, '=' 脱括号, ' ' 错误
        
        // + 行
        priorityTable['+'] = {
            {'+', '>'}, {'-', '>'}, {'*', '<'}, {'/', '<'}, 
            {'^', '<'}, {'!', '<'}, {'(', '<'}, {')', '>'}, {'#', '>'}
        };
        
        // - 行
        priorityTable['-'] = {
            {'+', '>'}, {'-', '>'}, {'*', '<'}, {'/', '<'}, 
            {'^', '<'}, {'!', '<'}, {'(', '<'}, {')', '>'}, {'#', '>'}
        };
        
        // * 行
        priorityTable['*'] = {
            {'+', '>'}, {'-', '>'}, {'*', '>'}, {'/', '>'}, 
            {'^', '<'}, {'!', '<'}, {'(', '<'}, {')', '>'}, {'#', '>'}
        };
        
        // / 行
        priorityTable['/'] = {
            {'+', '>'}, {'-', '>'}, {'*', '>'}, {'/', '>'}, 
            {'^', '<'}, {'!', '<'}, {'(', '<'}, {')', '>'}, {'#', '>'}
        };
        
        // ^ 行 (幂运算，右结合)
        priorityTable['^'] = {
            {'+', '>'}, {'-', '>'}, {'*', '>'}, {'/', '>'}, 
            {'^', '>'}, {'!', '<'}, {'(', '<'}, {')', '>'}, {'#', '>'}
        };
        
        // ! 行 (阶乘，单目运算符)
        priorityTable['!'] = {
            {'+', '>'}, {'-', '>'}, {'*', '>'}, {'/', '>'}, 
            {'^', '>'}, {'!', '>'}, {'(', ' '}, {')', '>'}, {'#', '>'}
        };
        
        // ( 行
        priorityTable['('] = {
            {'+', '<'}, {'-', '<'}, {'*', '<'}, {'/', '<'}, 
            {'^', '<'}, {'!', '<'}, {'(', '<'}, {')', '='}, {'#', ' '}
        };
        
        // ) 行
        priorityTable[')'] = {
            {'+', '>'}, {'-', '>'}, {'*', '>'}, {'/', '>'}, 
            {'^', '>'}, {'!', '>'}, {'(', ' '}, {')', '>'}, {'#', '>'}
        };
        
        // # 行 (结束符)
        priorityTable['#'] = {
            {'+', '<'}, {'-', '<'}, {'*', '<'}, {'/', '<'}, 
            {'^', '<'}, {'!', '<'}, {'(', '<'}, {')', ' '}, {'#', '='}
        };
    }
    
    void initializeFunctionDescriptions() {
        functionDescriptions["sqrt"] = "平方根";
        functionDescriptions["ln"] = "自然对数";
        functionDescriptions["log"] = "常用对数(底数10)";
        functionDescriptions["sin"] = "正弦函数(弧度)";
        functionDescriptions["cos"] = "余弦函数(弧度)";
        functionDescriptions["tan"] = "正切函数(弧度)";
        functionDescriptions["asin"] = "反正弦函数";
        functionDescriptions["acos"] = "反余弦函数";
        functionDescriptions["atan"] = "反正切函数";
        functionDescriptions["exp"] = "指数函数(e^x)";
        functionDescriptions["abs"] = "绝对值";
        functionDescriptions["pow"] = "幂函数(pow(x,y)=x^y)";
    }
    
    // 比较运算符优先级
    char comparePriority(char stackTop, char current) {
        if (priorityTable.count(stackTop) && priorityTable[stackTop].count(current)) {
            return priorityTable[stackTop][current];
        }
        return ' '; // 无效组合
    }
    
    // 检查是否为支持的函数
    bool isFunction(const string& token) {
        return find(supportedFunctions.begin(), supportedFunctions.end(), token) != supportedFunctions.end();
    }
    
    // 执行基本运算
    double calculate(double a, double b, char op) {
        switch(op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': 
                if (fabs(b) < 1e-10) {
                    throw runtime_error("除零错误");
                }
                return a / b;
            case '^': return pow(a, b);
            default:
                throw runtime_error("未知运算符: " + string(1, op));
        }
    }
    
    // 执行阶乘运算
    double factorial(double n) {
        if (n < 0 || n != floor(n)) {
            throw runtime_error("阶乘运算要求非负整数");
        }
        double result = 1;
        for (int i = 2; i <= n; i++) {
            result *= i;
        }
        return result;
    }
    
    // 执行函数计算
    double calculateFunction(const string& func, const vector<double>& args) {
        if (func == "sqrt") {
            if (args[0] < 0) {
                throw runtime_error("平方根函数参数不能为负");
            }
            return sqrt(args[0]);
        }
        else if (func == "ln") {
            if (args[0] <= 0) {
                throw runtime_error("自然对数函数参数必须为正");
            }
            return log(args[0]);
        }
        else if (func == "log") {
            if (args[0] <= 0) {
                throw runtime_error("常用对数函数参数必须为正");
            }
            return log10(args[0]);
        }
        else if (func == "sin") {
            return sin(args[0]);
        }
        else if (func == "cos") {
            return cos(args[0]);
        }
        else if (func == "tan") {
            return tan(args[0]);
        }
        else if (func == "asin") {
            if (args[0] < -1 || args[0] > 1) {
                throw runtime_error("反正弦函数参数必须在[-1,1]范围内");
            }
            return asin(args[0]);
        }
        else if (func == "acos") {
            if (args[0] < -1 || args[0] > 1) {
                throw runtime_error("反余弦函数参数必须在[-1,1]范围内");
            }
            return acos(args[0]);
        }
        else if (func == "atan") {
            return atan(args[0]);
        }
        else if (func == "exp") {
            return exp(args[0]);
        }
        else if (func == "abs") {
            return fabs(args[0]);
        }
        else if (func == "pow") {
            if (args.size() < 2) throw runtime_error("pow函数需要两个参数");
            return pow(args[0], args[1]);
        }
        else {
            throw runtime_error("未知函数: " + func);
        }
    }
    
    // 检查字符是否为运算符
    bool isOperator(char c) {
        return string("+-*/^!()#").find(c) != string::npos;
    }
    
    // 提取函数名
    string extractFunction(const string& expr, size_t& pos) {
        string func;
        while (pos < expr.length() && isalpha(expr[pos])) {
            func += expr[pos];
            pos++;
        }
        return func;
    }
    
    // 提取数字
    string extractNumber(const string& expr, size_t& pos) {
        string numStr;
        bool hasDecimal = false;
        bool hasExponent = false;
        
        while (pos < expr.length()) {
            char c = expr[pos];
            
            if (isdigit(c)) {
                numStr += c;
            }
            else if (c == '.' && !hasDecimal) {
                numStr += c;
                hasDecimal = true;
            }
            else if ((c == 'e' || c == 'E') && !hasExponent) {
                numStr += c;
                hasExponent = true;
                // 处理指数符号
                if (pos + 1 < expr.length() && (expr[pos + 1] == '+' || expr[pos + 1] == '-')) {
                    numStr += expr[++pos];
                }
            }
            else if ((c == '+' || c == '-') && (pos == 0 || isOperator(expr[pos - 1]) || expr[pos - 1] == 'e' || expr[pos - 1] == 'E')) {
                // 处理正负号（位于开头或运算符后或指数符号后）
                numStr += c;
            }
            else {
                break;
            }
            pos++;
        }
        
        return numStr;
    }
    
    // 处理函数参数
    vector<double> evaluateFunctionParameters(const string& expr, size_t& pos) {
        // 跳过函数名后的左括号
        if (pos >= expr.length() || expr[pos] != '(') {
            throw runtime_error("函数后必须跟左括号");
        }
        pos++; // 跳过 '('
        
        vector<double> parameters;
        string currentParam;
        int bracketCount = 1;
        
        while (pos < expr.length() && bracketCount > 0) {
            char c = expr[pos];
            
            if (c == '(') {
                bracketCount++;
                currentParam += c;
            }
            else if (c == ')') {
                bracketCount--;
                if (bracketCount > 0) {
                    currentParam += c;
                }
            }
            else if (c == ',' && bracketCount == 1) {
                // 参数分隔符
                if (!currentParam.empty()) {
                    parameters.push_back(evaluate(currentParam));
                    currentParam.clear();
                }
            }
            else {
                currentParam += c;
            }
            pos++;
        }
        
        if (bracketCount != 0) {
            throw runtime_error("函数参数括号不匹配");
        }
        
        // 处理最后一个参数
        if (!currentParam.empty()) {
            parameters.push_back(evaluate(currentParam));
        }
        
        return parameters;
    }

public:
    // 表达式求值主函数
    double evaluate(const string& expression) {
        stack<char> optrStack;    // 运算符栈
        stack<double> opndStack;   // 操作数栈
        
        // 添加结束符
        string expr = expression + "#";
        
        optrStack.push('#');
        
        size_t i = 0;
        while (i < expr.length()) {
            // 跳过空白字符
            if (isspace(expr[i])) {
                i++;
                continue;
            }
            
            // 处理函数
            if (isalpha(expr[i])) {
                size_t start = i;
                string func = extractFunction(expr, i);
                
                if (isFunction(func)) {
                    cout << "读取函数: " << func << " [" << functionDescriptions[func] << "]" << endl;
                    vector<double> params = evaluateFunctionParameters(expr, i);
                    double result = calculateFunction(func, params);
                    opndStack.push(result);
                    cout << "计算函数: " << func << "(";
                    for (size_t j = 0; j < params.size(); j++) {
                        if (j > 0) cout << ", ";
                        cout << params[j];
                    }
                    cout << ") = " << result << endl;
                    continue;
                } else {
                    // 如果不是函数，回退并尝试作为变量处理（这里简化为错误）
                    i = start;
                    throw runtime_error("未知函数或变量: " + func);
                }
            }
            
            // 处理数字
            if (isdigit(expr[i]) || expr[i] == '.' || 
               ((expr[i] == '+' || expr[i] == '-') && 
                (i == 0 || isOperator(expr[i-1]) || expr[i-1] == 'e' || expr[i-1] == 'E'))) {
                string numStr = extractNumber(expr, i);
                try {
                    double num = stod(numStr);
                    opndStack.push(num);
                    cout << "读取数字: " << num << endl;
                } catch (const exception& e) {
                    throw runtime_error("数字格式错误: " + numStr);
                }
            } 
            // 处理运算符
            else if (isOperator(expr[i])) {
                char currentOp = expr[i];
                char stackTop = optrStack.top();
                char relation = comparePriority(stackTop, currentOp);
                
                cout << "栈顶: " << stackTop << ", 当前: " << currentOp 
                     << ", 关系: " << relation << endl;
                
                switch(relation) {
                    case '<':
                        // 栈顶优先级低，当前运算符入栈
                        optrStack.push(currentOp);
                        i++;
                        break;
                    case '=':
                        // 优先级相等，脱括号
                        optrStack.pop(); // 弹出栈顶的'('
                        i++;            // 跳过当前的')'
                        break;
                    case '>': {
                        // 栈顶优先级高，执行运算
                        char op = optrStack.top();
                        optrStack.pop();
                        
                        if (op == '!') {
                            // 阶乘运算（单目运算符）
                            if (opndStack.empty()) {
                                throw runtime_error("阶乘运算缺少操作数");
                            }
                            double a = opndStack.top();
                            opndStack.pop();
                            double result = factorial(a);
                            opndStack.push(result);
                            cout << "计算: " << a << "! = " << result << endl;
                        } else {
                            // 二元运算符
                            if (opndStack.size() < 2) {
                                throw runtime_error("操作数不足");
                            }
                            
                            double b = opndStack.top();
                            opndStack.pop();
                            double a = opndStack.top();
                            opndStack.pop();
                            
                            double result = calculate(a, b, op);
                            opndStack.push(result);
                            
                            cout << "计算: " << a << " " << op << " " << b 
                                 << " = " << result << endl;
                        }
                        break;
                    }
                    case ' ':
                    default:
                        throw runtime_error("表达式语法错误: 无效的运算符组合 " + 
                                          string(1, stackTop) + " 和 " + string(1, currentOp));
                }
            } else {
                throw runtime_error("未知字符: " + string(1, expr[i]));
            }
        }
        
        if (opndStack.size() != 1) {
            throw runtime_error("表达式不完整或格式错误");
        }
        
        return opndStack.top();
    }
    
    // 显示支持的函数列表
    void showSupportedFunctions() {
        cout << "=== 支持的函数列表 ===" << endl;
        for (const auto& func : supportedFunctions) {
            cout << func << " - " << functionDescriptions[func] << endl;
        }
        cout << "支持的运算符: +, -, *, /, ^, !, (, )" << endl;
        cout << endl;
    }
    
    // 交互式求值
    void interactiveEvaluation() {
        cout << "=== 高级表达式求值器 ===" << endl;
        
        showSupportedFunctions();
        
        cout << "输入 'quit' 退出" << endl << endl;
        
        string input;
        while (true) {
            cout << "请输入表达式: ";
            getline(cin, input);
            
            if (input == "quit" || input == "exit") {
                break;
            }
            
            if (input.empty()) {
                continue;
            }
            
            try {
                double result = evaluate(input);
                cout << ">>> 结果: " << result << endl << endl;
            } catch (const exception& e) {
                cout << ">>> 错误: " << e.what() << endl << endl;
            }
        }
    }
};

// 测试函数
void runAdvancedTests() {
    AdvancedExpressionEvaluator evaluator;
    
    cout << "=== 高级数学函数测试 ===" << endl;
    
    vector<pair<string, double>> testCases = {
        {"sqrt(16)", 4.0},
        {"sin(0)", 0.0},
        {"cos(0)", 1.0},
        {"log(100)", 2.0},
        {"ln(exp(1))", 1.0},
        {"abs(-5)", 5.0},
        {"5!", 120.0},
        {"2^3", 8.0},
        {"sqrt(9)+2*3", 9.0},
        {"sin(3.14159/2)", 1.0},  // 近似值
        {"log(1000)*2", 6.0},
        {"pow(2,3)", 8.0},
        {"3! + 2^3", 14.0}
    };
    
    for (const auto& test : testCases) {
        cout << "测试表达式: " << test.first << endl;
        try {
            double result = evaluator.evaluate(test.first);
            double expected = test.second;
            cout << "结果: " << result << ", 期望: " << expected;
            if (fabs(result - expected) < 0.0001) {
                cout << " ✓ 通过" << endl;
            } else {
                cout << " ✗ 失败" << endl;
            }
        } catch (const exception& e) {
            cout << "错误: " << e.what() << endl;
        }
        cout << endl;
    }
}

// 演示函数
void demonstrateFunctions() {
    AdvancedExpressionEvaluator evaluator;
    
    cout << "=== 高级数学函数演示 ===" << endl;
    
    vector<string> demonstrations = {
        "sqrt(25)",
        "sin(0.5236)",  // 30度角的正弦值
        "cos(1.0472)",  // 60度角的余弦值
        "log(100)",
        "ln(2.71828)",
        "exp(2)",
        "abs(-7.5)",
        "5!",
        "2^8",
        "sqrt(2+3*4)",
        "sin(3.14159/6) + cos(3.14159/3)",
        "pow(2, 10)"
    };
    
    for (const auto& demo : demonstrations) {
        cout << "计算: " << demo << endl;
        try {
            double result = evaluator.evaluate(demo);
            cout << "结果: " << result << endl << endl;
        } catch (const exception& e) {
            cout << "错误: " << e.what() << endl << endl;
        }
    }
}

int main() {
    AdvancedExpressionEvaluator evaluator;
    
    // 运行测试
    runAdvancedTests();
    
    // 函数演示
    demonstrateFunctions();
    
    // 交互式求值
    evaluator.interactiveEvaluation();
    
    return 0;
}