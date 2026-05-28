#include "Header.h"

// ======================= ПРОВЕРКА ВВОДА =======================
// проверка ввода числа 
int getNumber() {
    using namespace std;
    int value;

    while (true) {
        cout << "Введите число: ";
        cin >> value;

        if (cin.fail() || value < 0) {
            cout << "Ошибка! Введите натуральное число!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

// проверка ввода строки
std::string getText() {
    using namespace std;
    string input;
    bool hasDigit;

    while (true) {
        hasDigit = false;
        cout << "Введите имя файла: ";
        getline(cin, input);

        if (input.empty()) {
            cout << "Ошибка! Строка не может быть пустой!" << endl;
            continue;
        }

        for (int i = 0; i < input.length(); i++) {
            if (isdigit(input[i])) {
                hasDigit = true;
                break;
            }
        }

        if (hasDigit) {
            cout << "Ошибка! В тексте не должно быть цифр!" << endl;
        }
        else {
            return input;
        }
    }
}

// очистка дерева
template<typename T>
void binaryTree<T>::clear(Node<T>* node) {
    if (node) {
        clear(node->getLeft());
        clear(node->getRight());
        delete node;
    }
}

// построение дерева из префиксного выражения
template<typename T>
Node<T>* binaryTree<T>::buildFromPrefix(const std::vector<T>& expr, int& index) {
    if (index >= expr.size()) return nullptr;

    T value = expr[index];
    index++;

    Node<T>* node = new Node<T>(value);

    if (value < 0) {
        Node<T>* leftChild = buildFromPrefix(expr, index);
        Node<T>* rightChild = buildFromPrefix(expr, index);
        node->setLeft(leftChild);
        node->setRight(rightChild);
    }

    return node;
}

// вычисление значения поддерева
template<typename T>
int binaryTree<T>::evaluate(Node<T>* node) const {
    if (!node) return 0;

    if (node->getKey() >= 0 && node->getKey() <= 9) {
        return node->getKey();
    }

    int leftVal = evaluate(node->getLeft());
    int rightVal = evaluate(node->getRight());

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

// преобразование дерева (оптимизация)
template<typename T>
Node<T>* binaryTree<T>::optimizeNode(Node<T>* node) {
    if (!node) return nullptr;

    Node<T>* newLeft = optimizeNode(node->getLeft());
    Node<T>* newRight = optimizeNode(node->getRight());

    node->setLeft(newLeft);
    node->setRight(newRight);

    if (node->getKey() < 0) {
        int result = evaluate(node);
        if (result >= 0 && result <= 9) {
            delete node;
            return new Node<T>(result);
        }
    }

    return node;
}

// рекурсивный вывод повернутого дерева
template<typename T>
void binaryTree<T>::printTreeRec(Node<T>* node, int level) const {
    using namespace std;

    if (!node) return;

    printTreeRec(node->getRight(), level + 1);

    for (int i = 0; i < level; i++) {
        cout << "    ";
    }

    T key = node->getKey();
    if (key >= -6 && key <= -1) {
        cout << codeToOperator(key) << endl;
    }
    else {
        cout << key << endl;
    }

    printTreeRec(node->getLeft(), level + 1);
}

// получение символа операции по коду
template<typename T>
char binaryTree<T>::getOpSymbol(T code) const {
    return codeToOperator(code);
}

// рекурсивное сохранение дерева в файл
template<typename T>
void binaryTree<T>::saveTreeRec(Node<T>* node, int level, std::ofstream& file) const {
    using namespace std;

    if (!node) return;

    saveTreeRec(node->getRight(), level + 1, file);

    for (int i = 0; i < level; i++) {
        file << "    ";
    }

    T key = node->getKey();
    if (key >= -6 && key <= -1) {
        file << codeToOperator(key) << endl;
    }
    else {
        file << key << endl;
    }

    saveTreeRec(node->getLeft(), level + 1, file);
}

// построение дерева из вектора
template<typename T>
void binaryTree<T>::buildTree(const std::vector<T>& expr) {
    clear(root);
    if (expr.empty()) return;

    int index = 0;
    root = buildFromPrefix(expr, index);
}

// основная функция оптимизации
template<typename T>
void binaryTree<T>::processTree() {
    root = optimizeNode(root);
}

// вывод дерева в консоль
template<typename T>
void binaryTree<T>::printTree() const {
    using namespace std;

    if (!root) {
        cout << "Дерево пусто." << endl;
        return;
    }
    cout << "\nДерево (повернуто на 90 градусов, корень слева):" << endl;
    printTreeRec(root, 0);
}

// сохранение дерева в файл
template<typename T>
void binaryTree<T>::saveTreeToFile(const std::string& filename) const {
    using namespace std;

    ofstream file(filename.c_str());
    if (!file.is_open()) {
        cout << "Ошибка открытия файла!" << endl;
        return;
    }

    if (!root) {
        file << "Дерево пусто." << endl;
    }
    else {
        saveTreeRec(root, 0, file);
    }

    file.close();
    cout << "Дерево сохранено в файл: " << filename << endl;
}


// создание экземпляров шаблона для типа int
template class binaryTree<int>;
template class Node<int>;
