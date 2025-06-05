#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
using namespace std;

// Узел дерева Хаффмана
struct Node {
    char data;         // Символ (для листьев)
    int freq;          // Частота
    Node *left, *right;// Левый и правый дочерние узлы

    Node(char data, int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

// Компаратор для приоритетной очереди (сортировка по частоте)
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq; // Минимальная частота — высший приоритет
    }
};

// Построение дерева Хаффмана
Node* buildHuffmanTree(const string& text) {
    // Шаг 1: Подсчет частот символов
    unordered_map<char, int> freqMap;
    for (char c : text) freqMap[c]++;

    // Шаг 2: Создание приоритетной очереди из узлов
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto& pair : freqMap) {
        pq.push(new Node(pair.first, pair.second));
    }

    // Шаг 3: Построение дерева
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        // Создание родительского узла с суммарной частотой
        Node* parent = new Node('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }

    return pq.top(); // Корень дерева
}

// Генерация кодов (рекурсивная функция)
void generateCodes(Node* root, string code, unordered_map<char, string>& huffmanCodes) {
    if (!root) return;

    // Если узел — лист, сохраняем код
    if (!root->left && !root->right) {
        huffmanCodes[root->data] = code;
    }

    generateCodes(root->left, code + "0", huffmanCodes);  // Левая ветвь: 0
    generateCodes(root->right, code + "1", huffmanCodes); // Правая ветвь: 1
}

// Кодирование текста
string encode(const string& text, unordered_map<char, string>& huffmanCodes) {
    string encodedText;
    for (char c : text) {
        encodedText += huffmanCodes[c];
    }
    return encodedText;
}

// Функция декодирования
string decode(const string& encodedText, Node* root) {
    string decodedText;
    Node* current = root;

    for (char bit : encodedText) {
        if (bit == '0') {
            current = current->left;  // Движение влево по дереву
        } else if (bit == '1') {
            current = current->right; // Движение вправо по дереву
        }

        // Если достигнут лист, добавляем символ в результат
        if (!current->left && !current->right) {
            decodedText += current->data;
            current = root; // Возвращаемся к корню для следующего символа
        }
    }

    return decodedText;
}

int main() {
    string text = "ABRACADABRA";
    Node* root = buildHuffmanTree(text);

    unordered_map<char, string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);

    cout << "symbol codes:\n";
    for (auto& pair : huffmanCodes) {
        cout << pair.first << ": " << pair.second << endl;
    }

    string encodedText = encode(text, huffmanCodes);
    cout << "coded string:" << encodedText << endl;

    string decodedText = decode(encodedText, root);
    cout << "decoded string: " << decodedText << endl;

    return 0;
}