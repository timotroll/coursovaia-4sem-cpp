#include <iostream>              // для ввода/вывода
#include <queue>                // для приоритетной очереди
#include <unordered_map>       // для хранения частот и кодов
#include <vector>              // для контейнеров
#include <string>              // для работы со строками

// Структура узла дерева Хаффмана
struct Node {
    char ch;                   // символ
    int freq;                  // частота появления символа
    Node* left;                // левый потомок
    Node* right;               // правый потомок

    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Компаратор для приоритетной очереди: наименьшая частота — наивысший приоритет
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq; // чем меньше частота, тем выше приоритет
    }
};

// Рекурсивная функция для построения кодов Хаффмана
void build_codes(Node* root, const std::string& str, std::unordered_map<char, std::string>& codes) {
    if (!root) return; // если узел пустой, ничего не делаем

    if (!root->left && !root->right) {
        codes[root->ch] = str; // достигли листа — сохраняем код
    }

    // Рекурсивно строим коды для левого и правого поддеревьев
    build_codes(root->left, str + "0", codes);
    build_codes(root->right, str + "1", codes);
}

// Функция построения дерева Хаффмана из строки
Node* build_huffman_tree(const std::string& text) {
    std::unordered_map<char, int> freq; // частотный словарь
    for (char c : text) freq[c]++;      // считаем частоту символов

    std::priority_queue<Node*, std::vector<Node*>, Compare> pq; // очередь с приоритетом
    for (auto& pair : freq) {
        pq.push(new Node(pair.first, pair.second)); // создаем узлы и добавляем в очередь
    }

    // Построение дерева: объединяем два узла с минимальной частотой
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();     // самый частый узел
        Node* right = pq.top(); pq.pop();    // второй по частоте

        Node* parent = new Node('\0', left->freq + right->freq); // новый родительский узел
        parent->left = left;
        parent->right = right;

        pq.push(parent); // помещаем обратно в очередь
    }

    return pq.top(); // корень дерева
}

// Функция кодирования текста на основе словаря кодов
std::string encode(const std::string& text, std::unordered_map<char, std::string>& codes) {
    std::string encoded;
    for (char c : text) encoded += codes[c]; // заменяем символы на коды
    return encoded;
}

// Функция декодирования закодированной строки
std::string decode(Node* root, const std::string& encoded) {
    std::string decoded;
    Node* current = root;
    for (char bit : encoded) {
        // Переход по дереву влево (0) или вправо (1)
        current = (bit == '0') ? current->left : current->right;

        // Если достигли листа — добавляем символ и возвращаемся к корню
        if (!current->left && !current->right) {
            decoded += current->ch;
            current = root;
        }
    }
    return decoded;
}

int main() {
    std::string text = "ABRACADABRA";             // исходная строка
    Node* root = build_huffman_tree(text);       // строим дерево Хаффмана

    std::unordered_map<char, std::string> codes; // словарь кодов символов
    build_codes(root, "", codes);                // заполняем словарь кодов

    std::string encoded = encode(text, codes);   // кодируем строку
    std::string decoded = decode(root, encoded); // декодируем обратно

    // Вывод результатов
    std::cout << "Original: " << text << "\n";
    std::cout << "Encoded: " << encoded << "\n";
    std::cout << "Decoded: " << decoded << "\n";
    return 0;
}