/*
 * Автор: Андреев Алексей АПО-12
 *
 * Задача: 2_3. Порядок обхода. Выведите элементы в порядке post-order
 * (снизу-вверх).
 *
 * Дано число N < 106 и последовательность целых чисел длиной N.
 * Требуется построить бинарное дерево, заданное наивным порядком вставки.
 * Т.е., при добавлении очередного числа K в дерево с корнем root,
 * если root→Key ≤ K, то узел K добавляется в правое поддерево root;
 * иначе в левое поддерево root.
 *
 * Рекурсия запрещена.
 */

#include <deque>
#include <iostream>
#include <stack>
#include <vector>

/*
/ Набор интерфейсов для того чтобы сделать единый алгоритм обхода
/ деревьев
*/
template <class T> class BinaryNode {
public:
  virtual ~BinaryNode(){};
  virtual BinaryNode *getLeft() = 0;
  virtual BinaryNode *getRight() = 0;
  virtual T &getData() = 0;
};

template <class T> class BinaryTree {
public:
  virtual ~BinaryTree(){};
  virtual BinaryNode<T> *getRoot() = 0;
};

////////////////////////////////////////////////////////////////////////////////

template <class T, class Func>
void TraversePostOrder(BinaryTree<T> &bt, Func f);

////////////////////////////////////////////////////////////////////////////////

template <class T> class Print {
public:
  void operator()(BinaryNode<T> *&arg);
};

////////////////////////////////////////////////////////////////////////////////

template <class T> struct BinarySearchNode : public BinaryNode<T> {
  BinarySearchNode();
  ~BinarySearchNode();

  BinaryNode<T> *getLeft();
  BinaryNode<T> *getRight();
  T &getData();

  T data;
  BinarySearchNode *left;
  BinarySearchNode *right;
};

template <class T> class BinarySearchTree : public BinaryTree<T> {
public:
  BinarySearchTree();
  ~BinarySearchTree();

  void Add(const T &key);
  void Delete(const T &key);
  bool Has(const T &key);

  BinaryNode<T> *getRoot();

  template <class Func> void TraversePostOrder(Func f);

private:
  BinarySearchNode<T> *root;

  BinarySearchNode<T> **_Find(const T &key);
};

////////////////////////////////////////////////////////////////////////////////

std::vector<int> GetNumbersFromStdin();

template <class T>
void AddToBinarySearchTree(BinarySearchTree<T> &bst, std::vector<T> const &vec);

/******************************************************************************/

int main() {
  auto bst = BinarySearchTree<int>();

  auto items = GetNumbersFromStdin();
  AddToBinarySearchTree(bst, items);

  TraversePostOrder(bst, Print<int>());

  return 0;
}

/******************************************************************************/

template <class T, class Func>
void TraversePostOrder(BinaryTree<T> &bt, Func f) {
  BinaryNode<T> *root = bt.getRoot();
  if (!root)
    return;

  std::stack<BinaryNode<T> *> stack1, stack2;
  stack1.push(root);
  while (!stack1.empty()) {
    auto node = stack1.top();
    stack1.pop();
    stack2.push(node);
    if (node->getLeft())
      stack1.push(node->getLeft());
    if (node->getRight())
      stack1.push(node->getRight());
  }

  while (!stack2.empty()) {
    auto node = stack2.top();
    stack2.pop();
    f(node);
  }
}

////////////////////////////////////////////////////////////////////////////////

template <class T> void Print<T>::operator()(BinaryNode<T> *&arg) {
  if (!arg) {
    std::cout << "| ";
    return;
  }
  std::cout << arg->getData() << " ";
}

////////////////////////////////////////////////////////////////////////////////

template <class T>
BinarySearchNode<T>::BinarySearchNode() : left(nullptr), right(nullptr) {}

template <class T> BinarySearchNode<T>::~BinarySearchNode() {}

template <class T> BinaryNode<T> *BinarySearchNode<T>::getLeft() {
  return left;
}

template <class T> BinaryNode<T> *BinarySearchNode<T>::getRight() {
  return right;
}

template <class T> T &BinarySearchNode<T>::getData() { return data; }

////////////////////////////////////////////////////////////////////////////////

template <class T> BinarySearchTree<T>::BinarySearchTree() : root(nullptr) {}

template <class T> BinarySearchTree<T>::~BinarySearchTree() {
  while (root)
    Delete(root->data);
}

template <class T> BinaryNode<T> *BinarySearchTree<T>::getRoot() {
  return root;
}

/*
/ Возвращает указатель на указатель на узел дерева с ключом key
/ Если же такого узла нет, возвращает указатель на нулевой указатель, который
/ указывал бы на узел, если бы его поместили в дерево
*/
template <class T>
BinarySearchNode<T> **BinarySearchTree<T>::_Find(const T &key) {
  BinarySearchNode<T> **res = &root;
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

template <class T> void BinarySearchTree<T>::Add(const T &key) {
  auto key_parent = _Find(key);
  *key_parent = new BinarySearchNode<T>();
  (*key_parent)->data = key;
}

template <class T> void BinarySearchTree<T>::Delete(const T &key) {
  BinarySearchNode<T> **node = _Find(key);
  if (!(*node))
    return;

  if (!(*node)->left) {
    BinarySearchNode<T> *right = (*node)->right;
    delete *node;
    *node = right;
  } else if (!(*node)->right) {
    BinarySearchNode<T> *left = (*node)->left;
    delete *node;
    *node = left;
  } else {
    BinarySearchNode<T> *minParent = *node;
    BinarySearchNode<T> *min = (*node)->right;
    while (min->left) {
      minParent = min;
      min = min->left;
    }
    (*node)->data = min->data;
    (minParent->left == min ? minParent->left : minParent->right) = min->right;
    delete min;
  }
}

template <class T> bool BinarySearchTree<T>::Has(const T &key) {
  return (_Find(key));
}

////////////////////////////////////////////////////////////////////////////////

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

template <class T>
void AddToBinarySearchTree(BinarySearchTree<T> &bst,
                           std::vector<T> const &vec) {
  for (auto const &i : vec) {
    bst.Add(i);
  }
}

////////////////////////////////////////////////////////////////////////////////
