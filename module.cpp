#include "header.h"

// Чтение выражения из файла
std::vector<int> readExpressionFromFile(const std::string& filename) {
    using namespace std;

    vector<int> expr;
    ifstream file(filename.c_str());

    if (!file.is_open()) {
        cout << "Ошибка открытия файла: " << filename << endl;
        return expr;
    }

    string token;
    while (file >> token) {
        if (token == "+") expr.push_back(-1);
        else if (token == "-") expr.push_back(-2);
        else if (token == "*") expr.push_back(-3);
        else if (token == "/") expr.push_back(-4);
        else if (token == "%") expr.push_back(-5);
        else if (token == "^") expr.push_back(-6);
        else {
            int num = stoi(token);
            expr.push_back(num);
        }
    }

    file.close();
    return expr;
}

// Ввод выражения с консоли
std::vector<int> readExpressionFromConsole() {
    using namespace std;

    vector<int> expr;
    string line;

    cout << "Введите префиксное выражение:" << endl;
    cout << "Операции: + - * / % ^" << endl;

    getline(cin, line);

    stringstream ss(line);
    string token;

    while (ss >> token) {
        if (token == "+") expr.push_back(-1);
        else if (token == "-") expr.push_back(-2);
        else if (token == "*") expr.push_back(-3);
        else if (token == "/") expr.push_back(-4);
        else if (token == "%") expr.push_back(-5);
        else if (token == "^") expr.push_back(-6);
        else {
            int num = stoi(token);
            expr.push_back(num);
        }
    }

    return expr;
}

// Вывод дерева (консоль или файл)
void outputTree(binaryTree<int>& tree) {
    using namespace std;

    int choice;
    cout << "\n=== Вывод дерева ===" << endl;
    cout << "1 - вывести в консоль" << endl;
    cout << "2 - сохранить в файл" << endl;
    cout << "Ваш выбор: ";
    choice = getNumber();

    while (choice != 1 && choice != 2) {
        cout << "Ошибка! Введите 1 или 2!!!!\n";
        choice = getNumber();
    }

    if (choice == 1) {
        tree.printTree();
    }
    else {
        string filename = getText();
        tree.saveTreeToFile(filename);
    }
}

void task_1() {
    using namespace std;

    binaryTree<int> tree;
    vector<int> expression;

    int choice;
    cout << "===== Построение дерева из префиксного выражения =====" << endl;
    cout << "Способ ввода:" << endl;
    cout << " 1 - ввести с консоли" << endl;
    cout << " 2 - загрузить из файла" << endl;
    choice = getNumber();

    while (choice != 1 && choice != 2) {
        cout << "Ошибка! Введите 1 или 2!!!\n";
        choice = getNumber();
    }

    if (choice == 1) {
        expression = readExpressionFromConsole();
    }
    else {
        string filename = getText();
        expression = readExpressionFromFile(filename);
    }

    if (expression.empty()) {
        cout << "Ошибка! Выражение пусто!" << endl;
        return;
    }

    
    tree.buildTree(expression);

    cout << "\n=== ИСХОДНОЕ ДЕРЕВО ===" << endl;
    outputTree(tree);

    
    tree.processTree();

    cout << "\n=== ДЕРЕВО ПОСЛЕ ОБРАБОТКИ ===" << endl;
    outputTree(tree);

}

void task_2() {
    using namespace std;
    solve();
}

