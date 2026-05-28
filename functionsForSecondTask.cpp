#include "Header.h"
#include <stack>
#include <map>
#include <cctype>
#include <iomanip>
#include <functional>

// получение приоритета оператора
int getPriority(char op) {
    switch (op) {
    case '^': return 3;
    case '*': case '/': case '%': return 2;
    case '+': case '-': return 1;
    default: return 0;
    }
}

// проверка, является ли символ оператором
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

// преобразование оператора в код (отрицательное число)
int operatorToCode(int code) {
    switch (code) {
    case '+': return -1;
    case '-': return -2;
    case '*': return -3;
    case '/': return -4;
    case '%': return -5;
    case '^': return -6;
    default: return 0;
    }
}

// преобразование кода в символ оператора
char codeToOperator(int code) {
    switch (code) {
    case -1: return '+';
    case -2: return '-';
    case -3: return '*';
    case -4: return '/';
    case -5: return '%';
    case -6: return '^';
    default: return '?';
    }
}

// Структура: токен для разбора выражения
struct Token {
    int value;
    bool isVariable;
    bool isOperator;

    Token() : value(0), isVariable(false), isOperator(false) {}
    Token(int v, bool var = false, bool op = false) : value(v), isVariable(var), isOperator(op) {}
};

// преобразование инфиксного выражения в постфиксную форму
std::vector<Token> infixToPostfix(const std::string& expression) {
    using namespace std;

    vector<Token> output;
    stack<char> operators;

    for (size_t i = 0; i < expression.length(); i++) {
        char c = expression[i];
        if (c == ' ') continue;

        if (isdigit(c)) {
            int num = 0;
            while (i < expression.length() && isdigit(expression[i])) {
                num = num * 10 + (expression[i] - '0');
                i++;
            }
            i--;
            output.push_back(Token(num, false, false));
        }
        else if (c == 'x') {
            output.push_back(Token(0, true, false));
        }
        else if (isOperator(c)) {
            while (!operators.empty() && operators.top() != '(' &&
                getPriority(operators.top()) >= getPriority(c)) {
                if (c == '^' && getPriority(operators.top()) > getPriority(c)) break;
                output.push_back(Token(operatorToCode(operators.top()), false, true));
                operators.pop();
            }
            operators.push(c);
        }
        else if (c == '(') {
            operators.push(c);
        }
        else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                output.push_back(Token(operatorToCode(operators.top()), false, true));
                operators.pop();
            }
            if (!operators.empty()) operators.pop();
        }
    }

    while (!operators.empty()) {
        output.push_back(Token(operatorToCode(operators.top()), false, true));
        operators.pop();
    }
    return output;
}

// вычисление значения поддерева и проверка наличия x
template<typename T>
int evaluateAndCheckX(Node<T>* node, bool& containsX) {
    if (!node) {
        containsX = false;
        return 0;
    }
    if (node->getKey() == 999) {
        containsX = true;
        return 0;
    }
    if (node->getKey() >= 0 && node->getKey() != 999) {
        containsX = false;
        return node->getKey();
    }
    bool hasXLeft = false, hasXRight = false;
    int leftVal = evaluateAndCheckX(node->getLeft(), hasXLeft);
    int rightVal = evaluateAndCheckX(node->getRight(), hasXRight);
    containsX = hasXLeft || hasXRight;
    if (containsX) return 0;
    switch (node->getKey()) {
    case -1: return leftVal + rightVal;
    case -2: return leftVal - rightVal;
    case -3: return leftVal * rightVal;
    case -4: return rightVal != 0 ? leftVal / rightVal : 0;
    case -5: return rightVal != 0 ? leftVal % rightVal : 0;
    case -6: return (int)pow(leftVal, rightVal);
    default: return 0;
    }
}

// замена поддеревьев без x на значения
template<typename T>
Node<T>* optimizeTree(Node<T>* node) {
    if (!node) return nullptr;
    Node<T>* newLeft = optimizeTree(node->getLeft());
    Node<T>* newRight = optimizeTree(node->getRight());
    node->setLeft(newLeft);
    node->setRight(newRight);
    if (node->getKey() >= -6 && node->getKey() <= -1) {
        bool containsX = false;
        int result = evaluateAndCheckX(node, containsX);
        if (!containsX) {
            delete node->getLeft();
            delete node->getRight();
            delete node;
            Node<T>* newNode = new Node<T>(result);
            return optimizeTree(newNode);
        }
    }
    return node;
}

// полная оптимизация дерева
template<typename T>
Node<T>* fullOptimizeTree(Node<T>* node) {
    Node<T>* current = node;
    Node<T>* previous = nullptr;
    while (current != previous) {
        previous = current;
        current = optimizeTree(current);
    }
    return current;
}

// вывод дерева на экран (повернутое на 90 градусов)
template<typename T>
void printTreeRotated(Node<T>* node, int level, std::ostream& output) {
    using namespace std;

    if (!node) return;
    printTreeRotated(node->getRight(), level + 1, output);
    for (int i = 0; i < level; i++) output << "    ";
    int key = node->getKey();
    if (key >= -6 && key <= -1) {
        output << codeToOperator(key) << endl;
    }
    else if (key == 999) {
        output << 'x' << endl;
    }
    else {
        output << key << endl;
    }
    printTreeRotated(node->getLeft(), level + 1, output);
}

// вывод дерева в файл 
template<typename T>
void saveTreeFormatted(Node<T>* node, int level, std::ostream& output) {
    using namespace std;

    if (!node) return;
    saveTreeFormatted(node->getRight(), level + 1, output);
    for (int i = 0; i < level; i++) output << "        ";
    int key = node->getKey();
    if (key >= -6 && key <= -1) {
        output << right << setw(8) << codeToOperator(key) << endl;
    }
    else if (key == 999) {
        output << right << setw(8) << 'x' << endl;
    }
    else {
        output << right << setw(8) << key << endl;
    }
    saveTreeFormatted(node->getLeft(), level + 1, output);
}

// построение дерева из постфиксной записи
template<typename T>
Node<T>* buildTreeFromPostfix(const std::vector<Token>& postfix, int xValue) {
    using namespace std;

    stack<Node<T>*> st;
    for (const auto& token : postfix) {
        if (token.isOperator) {
            Node<T>* right = st.top(); st.pop();
            Node<T>* left = st.top(); st.pop();
            Node<T>* node = new Node<T>(token.value);
            node->setLeft(left);
            node->setRight(right);
            st.push(node);
        }
        else {
            if (token.isVariable) st.push(new Node<T>(999));
            else st.push(new Node<T>(token.value));
        }
    }
    return st.empty() ? nullptr : st.top();
}

// получение выражения от пользователя
std::string getExpressionFromUser() {
    using namespace std;

    string expression;
    cout << "Введите арифметическое выражение: ";
    cin.ignore();
    getline(cin, expression);
    return expression;
}

// получение выражения из файла
std::string getExpressionFromFile(const std::string& filename) {
    using namespace std;

    ifstream file(filename.c_str());
    if (!file.is_open()) {
        cout << "Ошибка открытия файла " << filename << endl;
        return "";
    }
    string expression;
    getline(file, expression);
    file.close();
    cout << "Прочитано из файла: " << expression << endl;
    return expression;
}

// получение значения переменной x
int getXValue() {
    using namespace std;

    int xValue;
    cout << "Введите значение переменной x: ";
    cin >> xValue;
    return xValue;
}

// Основная функция решения задачи
void solve() {
    using namespace std;

    int choice;
    cout << "=== Построение дерева из ификсного выражения ===" << endl;
    cout << "Способ ввода:" << endl;
    cout << " 1 - Ввод с консоли" << endl;
    cout << " 2 - Ввод из файла" << endl;
    choice = getNumber();

    while (choice != 1 && choice != 2) {
        cout << "Ошибка! Введите 1 или 2!!!\n";
        choice = getNumber();
    }

    string expression;
    int xValue;

    if (choice == 1) {
        expression = getExpressionFromUser();
        xValue = getXValue();
    }
    else if (choice == 2) {
        string filename;
        cout << "Введите имя файла: ";
        getline(cin, filename);
        expression = getExpressionFromFile(filename);
        if (expression.empty()) return;
        xValue = getXValue();
    }
    else {
        cout << "Неверный выбор!" << endl;
        return;
    }

    if (expression.empty()) {
        cout << "Выражение не введено!" << endl;
        return;
    }

    cout << "\nВыражение: " << expression << endl;
    cout << "Значение x = " << xValue << endl;

    vector<Token> postfix = infixToPostfix(expression);
    Node<int>* root = buildTreeFromPostfix<int>(postfix, xValue);

    if (!root) {
        cout << "Ошибка построения дерева!" << endl;
        return;
    }

    function<int(Node<int>*)> evaluateWithX = [&](Node<int>* node) -> int {
        if (!node) return 0;
        if (node->getKey() == 999) return xValue;
        if (node->getKey() >= 0) return node->getKey();

        int leftVal = evaluateWithX(node->getLeft());
        int rightVal = evaluateWithX(node->getRight());

        switch (node->getKey()) {
        case -1: return leftVal + rightVal;
        case -2: return leftVal - rightVal;
        case -3: return leftVal * rightVal;
        case -4: return rightVal != 0 ? leftVal / rightVal : 0;
        case -5: return rightVal != 0 ? leftVal % rightVal : 0;
        case -6: return (int)pow(leftVal, rightVal);
        default: return 0;
        }
        };

    int result = evaluateWithX(root);
    cout << "Результат вычисления: " << result << endl;

    cout << "\nВыберите способ вывода:" << endl;
    cout << "1. Вывод в консоль" << endl;
    cout << "2. Вывод в файл" << endl;
    cout << "Ваш выбор: ";
    cin >> choice;

    if (choice == 1) {
        cout << "\n=== ИСХОДНОЕ ДЕРЕВО ===" << endl;
        printTreeRotated(root, 0, cout);

        root = fullOptimizeTree(root);

        cout << "\n=== ПРЕОБРАЗОВАННОЕ ДЕРЕВО ===" << endl;
        printTreeRotated(root, 0, cout);
        cout << "\nРезультат: " << result << endl;
    }
    else if (choice == 2) {
        string filename;
        cout << "Введите имя файла для вывода: ";
        cin >> filename;

        ofstream outFile(filename.c_str());
        if (!outFile.is_open()) {
            cout << "Ошибка открытия файла!" << endl;
            return;
        }

        outFile << "Выражение: " << expression << endl;
        outFile << "x = " << xValue << endl;
        outFile << "Результат: " << result << endl;

        outFile << "\n=== ИСХОДНОЕ ДЕРЕВО ===" << endl;
        saveTreeFormatted(root, 0, outFile);

        root = fullOptimizeTree(root);

        outFile << "\n=== ПРЕОБРАЗОВАННОЕ ДЕРЕВО ===" << endl;
        saveTreeFormatted(root, 0, outFile);
        outFile.close();
        cout << "Сохранено в файл: " << filename << endl;
    }
}
