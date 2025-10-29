#include<iostream>
#include<stack>
#include<string>
#include<cmath>
#include<map>
#include<cctype>
#include<vector>
#include<algorithm>

using namespace std;
class Calculator {
private:
//函数到名称
  map<string,string> Fun; 
//支持的函数表
  vector<string> funs={"sqrt","ln","log","sin",
                          "cos","tan","asin","acos",
                          "atan","exp","abs"};
// 运算符优先级表
  const char prt[9][9] = {
    /*              |-------------------- 当 前 运 算 符 --------------------| */
        /*              +    -    *    /    ^    !    (    )    #  */
        /* --  + */    '>', '>', '<', '<', '<', '<', '<', '>', '>',
        /* |   - */    '>', '>', '<', '<', '<', '<', '<', '>', '>',
        /* 栈  * */    '>', '>', '>', '>', '<', '<', '<', '>', '>',
        /* 顶  / */    '>', '>', '>', '>', '<', '<', '<', '>', '>',
        /* 运  ^ */    '>', '>', '>', '>', '>', '<', '<', '>', '>',
        /* 算  ! */    '>', '>', '>', '>', '>', '>', ' ', '>', '>',
        /* 符  ( */    '<', '<', '<', '<', '<', '<', '<', '=', ' ',
        /* |   ) */    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        /* --  # */    '<', '<', '<', '<', '<', '<', '<', ' ', '='
    };
// 运算符到索引
  map<char, int> opI = {
        {'+', 0}, {'-', 1}, {'*', 2}, {'/', 3}, 
        {'^', 4}, {'!', 5}, {'(', 6}, {')', 7}, 
        {'#', 8}
    };
// 初始化函数
  void initFuns(){
      Fun["sqrt"]="计算平方根";
      Fun["ln"]="自然对数";
      Fun["log"]="常用对数";
      Fun["sin"]="正弦函数";
      Fun["cos"]="余弦函数";
      Fun["tan"]="正切函数";
      Fun["asin"]="反正弦函数";
      Fun["acos"]="反余弦函数";
      Fun["atan"]="反正切函数";
      Fun["exp"]="指数函数";
      Fun["abs"]="绝对值函数";
    }
   
public:

  Calculator() {
        initFuns();
    }

  string getNumber(const string& expr, size_t& pos) {
        string numStr;
        bool hasDecimal = false;
        bool hasExponent = false;
        
        // 处理负号
        if (expr[pos] == '-' && (pos == 0 || isO(expr[pos-1]) || expr[pos-1] == 'e' || expr[pos-1] == 'E')) {
            numStr += expr[pos++];
        }
        
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
            else {
                break;
            }
            pos++;
        }
        
        return numStr;
    }

  bool isO(char c) {
         return string("+-*/^!()#").find(c) != string::npos;
    }

  char ComparePrt(char StackTop, char Current) {
      if(opI.find(StackTop) != opI.end() && opI.find(Current) != opI.end())
      { 
        int row = opI[StackTop];
        int col = opI[Current];
        return prt[row][col];
      }
      throw runtime_error("未知运算符");

      return ' ';
    }

  double calculateSimple(double a, char op, double b) {
      switch (op) {
          case '+': return a + b;
          case '-': return a - b;
          case '*': return a * b;
          case '/': 
              if (b == 0) {
                  throw runtime_error("除数不能为零");
              }
              return a / b;
          case '^': return pow(a, b);
          default:
              throw runtime_error("未知运算符");
      } 
    }

  double Factorial(double n) {
      if (n < 0) {
          throw runtime_error("阶乘函数参数不能为负");
      }
      if (n == 0 || n == 1) {
          return 1;
      }
      double result = 1;
      for (int i = 2; i <= n; ++i) {
          result *= i;
      }
      return result;
    }

  string getFunc(const string& expr, size_t& pos) {
        string func;
        while (pos < expr.length() && isalpha(expr[pos])) {
            func += expr[pos];
            pos++;
        }
        return func;
    }

  bool isF(const string& func) {
      return find(funs.begin(),funs.end(),func)!=funs.end();
    }

  double calculateFun(const string& func, double value) {
      if (func == "sqrt") {
          if (value < 0) {
              throw runtime_error("平方根函数参数不能为负");
          }
          return sqrt(value);
      }
      else if (func == "ln") {
          if (value <= 0) {
              throw runtime_error("自然对数函数参数必须为正");
          }
          return log(value);
      }
      else if (func == "log") {
          if (value <= 0) {
              throw runtime_error("常用对数函数参数必须为正");
          }
          return log10(value);
      }
      else if (func == "sin") {
          return sin(value);
      }
      else if (func == "cos") {
          return cos(value);
      }
      else if (func == "tan") {
          return tan(value);
      }
      else if (func == "asin") {
          if (value < -1 || value > 1) {
              throw runtime_error("反正弦函数参数必须在[-1,1]范围内");
          }
          return asin(value);
      }
      else if (func == "acos") {
          if (value < -1 || value > 1) {
              throw runtime_error("反余弦函数参数必须在[-1,1]范围内");
          }
          return acos(value);
      }
      else if (func == "atan") {
          return atan(value);
      }
      else if (func == "exp") {
          return exp(value);
      }
      else if (func == "abs") {
          return fabs(value);
      }
      else {
          throw runtime_error("未知函数: " + func);
      }
    }

  double DealFunc(const string& expr, size_t& pos) {
      if(pos >= expr.length() || expr[pos] != '(') {
          throw runtime_error("函数后必须跟左括号");
      }
      pos++;
      string paramExpr;
      int bcont=1;
      while(pos < expr.length() && bcont > 0) {
          if(expr[pos] == '(') bcont++;
          else if(expr[pos] == ')') bcont--;
          
          if(bcont > 0) {
              paramExpr += expr[pos];
          }
          pos++;
      }
      if(bcont != 0) {
          throw runtime_error("函数参数括号不匹配");
    }
      return Evaluate(paramExpr+"#");
  }

  double Evaluate(const string& expression) {
        stack<char> opStack;    // 运算栈
        stack<double> numStack;   // 数栈
        stack<string> funcStack;  // 函数栈
        
        string expr = expression + "#";
        
        opStack.push('#');
        
        size_t i = 0;
        while (i < expr.length()&&(expr[i]!='#'||opStack.top()!='#' )) {
            if (isspace(expr[i])) {
                i++;
                continue;
            }
            // 函数处理
            if (isalpha(expr[i])) {
                size_t start = i;
                string func = getFunc(expr, i);
                
                if (isF(func)) {
                    //cout << "读取函数: " << func << " [" << Fun[func] << "]" << endl;
                    double param = DealFunc(expr, i);
                    double result = calculateFun(func, param);
                    numStack.push(result);
                    //cout << "计算函数: " << func << "(" << param << ") = " << result << endl;
                    continue;
                } else {
                    // 如果不是函数，回退报错
                    i = start;
                    throw runtime_error("未知函数: " + func);
                }
            }
            
            // 数字处理
            if (isdigit(expr[i]) || expr[i] == '.' || (expr[i] == '-' && (i == 0 || isO(expr[i-1])))) {
                string numStr = getNumber(expr, i);
                try {
                    double num = stod(numStr);
                    numStack.push(num);
                    //cout << "读取数字: " << num << endl;
                } catch (const exception& e) {
                    throw runtime_error("数字格式错误: " + numStr);
                }
                continue;
            }
            
            // 处理运算符
            if (isO(expr[i])) {
                char currentOp = expr[i];
                char stackTop = opStack.top();
                char relation = ComparePrt(stackTop, currentOp);  
                //cout  << "栈顶: " << stackTop << ", 当前: " << currentOp << ", 关系: " << relation << endl;
                switch(relation) {
                    case '<':
                        opStack.push(currentOp);
                        i++;
                        break;
                    case '=':
                        opStack.pop();
                        i++;
                        break;
                    case '>': {
                        char op = opStack.top();
                        opStack.pop();
                        
                        if(op == '!') {
                            // 阶乘运算（单目运算符）
                            if (numStack.empty()) {
                                throw runtime_error("阶乘运算缺少操作数");
                            }
                            double a = numStack.top();
                            numStack.pop();
                            double result = Factorial(a);
                            numStack.push(result);
                            
                           // cout << "计算: " << a << "! = " << result << endl;
                        } 
                          else {

                            if(numStack.size() < 2) {
                                throw runtime_error("操作数不足");
                            }

                          double b = numStack.top(); numStack.pop();
                          double a = numStack.top(); numStack.pop();
                          double result = calculateSimple(a, op, b);  
              
                          numStack.push(result);
                          //cout << "计算: " << a << " " << op << " " << b << " = " << result << endl;
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

        if (numStack.size() != 1 || opStack.top() != '#') {
            throw runtime_error("表达式不完整或格式错误");
        
        }
      
        return numStack.top();

    }

  void ActivateEvaluator() {
        string input;
        while (true) {
            cout << "请输入表达式 (输入 'quit' 退出): ";
            getline(cin, input);
            if (input == "quit" || input == "exit") {
                break;
            }
            if (input.empty()) {
                continue;
            }
            try {
                double result = Evaluate(input);
                cout << ">>> 结果: " << result << endl << endl;
            } catch (const exception& e) {
                cout << ">>> 错误: " << e.what() << endl << endl;
            }
        }
      }
};

int main() {
    Calculator calculator;
    //calculator.ActivateEvaluator();
    string ins[5]={
        "3+5*2",
        "sqrt(16)+log(100)",
        "5! + 3^2",
        "sin(3.14159/2) + cos(0)",
        "ln(exp(1)) + abs(-5)"
    };
    string input="3+5*2";
    double result;
    for(int i=0;i<5;i++){
        input=ins[i];
        result=calculator.Evaluate(input);
        cout<<"表达式: "<<input<<" = "<<result<<endl;
    }
    return 0;
}