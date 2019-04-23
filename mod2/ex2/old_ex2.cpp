#include <iostream>
#include <stack>
#include <vector>

template<class T>
struct BinaryNode {
    BinaryNode();
    ~BinaryNode();

    T data;
    BinaryNode* left;
    BinaryNode* right;
};

template<class T>
class BinaryTree {
public:
    BinaryTree();
    ~BinaryTree();

    void Add(const T& key);
    void Delete(const T& key);
    bool Has(const T& key);

    template<class Func>
    void Traverse(Func f);
private:
    BinaryNode<T>* root;

    BinaryNode<T>** _Find(const T& key);
};

template<class T>
class Print {
public:
    void operator()(const T& arg) {
        std::cout << arg << " ";
    }
};

std::vector<int> GetNumbersFromStdin() {
    int n = 0;
    int input = 0;
    std::cin >> n;

    auto ret = std::vector<int>();
    ret.resize(n);
    for (int i = 0; i < n; i++) {
        std::cin >> input;
        ret[i] = input;
    }

    return ret;
}

template<class T>
void AddToBinaryTree(BinaryTree<T>& bst, std::vector<T> const& vec) {
    for (auto const& i: vec) {
        bst.Add(i);
    }
}

int main() {

    auto bst = BinaryTree<int>();
    auto items = GetNumbersFromStdin();

    AddToBinaryTree(bst, items);
    bst.Traverse(Print<int>());

    return 0;
}

template<class T>
BinaryNode<T>::BinaryNode() : left(nullptr), right(nullptr) {
}

template<class T>
BinaryNode<T>::~BinaryNode() {
    if (left) 
        delete left;
    if (right)
        delete right;
}

template<class T>
BinaryTree<T>::BinaryTree() : root(nullptr) {
}

template<class T>
BinaryTree<T>::~BinaryTree() {
    delete root;
}

template<class T>
class IsLess {
public:
    bool operator()(const T& lhs, const T& rhs) {
        return lhs < rhs;
    }
};

/* 
/ Возвращает указатель на указатель на узел дерева с ключом key
/ Если же такого узла нет, возвращает указатель на нулевой указатель, который
/ указывал бы на узел, если бы его поместили в дерево
*/
template<class T>
BinaryNode<T>** BinaryTree<T>::_Find(const T& key) {
    BinaryNode<T>** res = &root;
    while (*res) {
        if ((*res)->data == key)
            break;
        if (key <= (*res)->data)
            res = &(*res)->left;
        else 
            res = &(*res)->right;
    }

    return res;
}

template<class T>
void BinaryTree<T>::Add(const T& key) {
    auto key_parent = _Find(key);
    *key_parent = new BinaryNode<T>();
    (*key_parent)->data = key;
}

/*
/ Post-order: левый, правый, сам
*/
// template<class T>
// template<class Func>
// void BinaryTree<T>::Traverse(Func f) {
//     std::stack<BinaryNode<T>*> stack;

//     BinaryNode<T>* currentNode = root;
//     do {
//         while(currentNode) {
//             if (currentNode->right)
//                 stack.push(currentNode->right);
//             stack.push(currentNode);
//             currentNode = currentNode->left;
//         }

//         currentNode = stack.top(); stack.pop();

//         if (currentNode->right) {
//             if (currentNode->right == stack.top()) {
//                 BinaryNode<T>* tmp_node = stack.top(); stack.pop();
//                 stack.push(currentNode);
//                 currentNode = currentNode->right;
//             }
//         } else {
//             f(currentNode->data);
//             currentNode = nullptr;
//         }
//     } while (!stack.empty());
// }

template<class T>
template<class Func>
void BinaryTree<T>::Traverse(Func f) {
    if(!root)
        return;

    std::stack<BinaryNode<T>*> stack1, stack2;
    stack1.push(root);
    while(!stack1.empty()) {
        auto node = stack1.top(); stack1.pop();
        stack2.push(node);
        if (node->left) 
            stack1.push(node->left);
        if (node->right)
            stack1.push(node->right);
    }

    while (!stack2.empty()) {
        auto node = stack2.top(); stack2.pop();
        f(node->data);
    }
}