/*
 * Автор: Андреев Алексей АПО-12
 *
 * Задача: 3.2. Декартово дерево.
 * Вычислить количество узлов в самом широком слое декартового дерева
 * и количество узлов в самом широком слове наивного дерева поиска.
 * Вывести их разницу. Разница может быть отрицательна.
 *
 * Дано число N и последовательность пар целых чисел длиной N.
 * Построить декартово дерево из N узлов, характеризующихся парами
 * чисел {X, Y}.
 *
 * Каждая пара определяет ключ Xi и приоритет Yi в декартовом дереве.
 *
 * Добавление узла в декартово дерево выполняйте второй версией алгоритма,
 * рассказанного на лекции:
 *
 * (*) При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим
 * приоритетом.
 * Затем разбейте найденное поддерево по ключу x так, чтобы в первом
 * поддереве все ключи меньше x, а во втором больше или равны x.
 * Получившиеся два дерева сделайте дочерними для нового узла (x, y).
 * Новый узел вставьте на место узла P.
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

/*
/ Аргумент layerInfo служит для того, чтобы сообщить функтору о том,
/ что слой закончился. Если не установить флаг layerInfo,
/ f() никогда не получит nullptr как аргумент.
*/
template <class T, class Func>
void TraverseBreadthFirst(BinaryTree<T> &bt, Func f, bool layerInfo = false);

////////////////////////////////////////////////////////////////////////////////

template <class T> class Print {
public:
  void operator()(BinaryNode<T> *&arg);
};

template <class T> class MaxLayer {
public:
  MaxLayer(int *ptr = nullptr);
  ~MaxLayer();
  void operator()(BinaryNode<T> *&arg);

private:
  int *returnValue;
  int currentLayer;
  int maxLayer;
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

template <class T> struct TreapNode : public BinaryNode<T> {
  TreapNode();
  ~TreapNode();

  BinaryNode<T> *getLeft();
  BinaryNode<T> *getRight();
  T &getData();

  T data;
  int priority;
  TreapNode *left;
  TreapNode *right;
};

template <class T> class Treap : public BinaryTree<T> {
public:
  Treap();
  ~Treap();

  BinaryNode<T> *getRoot();

  void Add(const T &key, const int &priority);
  void Delete(const T &key);
  bool Has(const T &key);

private:
  TreapNode<T> *root;

  void _Split(TreapNode<T> *node, int key, TreapNode<T> *&left,
              TreapNode<T> *&right);
  TreapNode<T> *_Merge(TreapNode<T> *left, TreapNode<T> *right);
};

////////////////////////////////////////////////////////////////////////////////

std::vector<std::pair<int, int>> GetPairsFromStdin();

template <class T>
void AddToBinarySearchTree(BinarySearchTree<T> &bst, std::vector<T> const &vec);

template <class T>
void AddToBinarySearchTree(BinarySearchTree<T> &bst,
                           std::vector<std::pair<T, int>> const &vec);

template <class T>
void AddToTreap(Treap<T> &trp, std::vector<std::pair<T, int>> const &vec);

/******************************************************************************/

int main() {
  auto bst = BinarySearchTree<int>();
  auto trp = Treap<int>();

  auto items = GetPairsFromStdin();
  AddToTreap(trp, items);
  AddToBinarySearchTree(bst, items);

  int maxLayerTRP = 0;
  int maxLayerBST = 0;
  TraverseBreadthFirst(trp, MaxLayer<int>(&maxLayerTRP), true);
  TraverseBreadthFirst(bst, MaxLayer<int>(&maxLayerBST), true);

  std::cout << (maxLayerTRP - maxLayerBST);

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
    f(*node);
  }
}

////////////////////////////////////////////////////////////////////////////////

template <class T, class Func>
void TraverseBreadthFirst(BinaryTree<T> &bt, Func f, bool layerInfo) {
  BinaryNode<T> *root = bt.getRoot();
  if (!root)
    return;

  std::deque<BinaryNode<T> *> deque;
  deque.push_back(root);
  if (layerInfo)
    deque.push_back(nullptr);

  while (!deque.empty()) {
    BinaryNode<T> *node = deque.front();
    deque.pop_front();
    f(node);
    if (!node) {
      if (deque.empty())
        break;
      else {
        deque.push_back(nullptr);
        continue;
      }
    }
    if (node->getLeft()) {
      deque.push_back(node->getLeft());
    }
    if (node->getRight()) {
      deque.push_back(node->getRight());
    }
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
MaxLayer<T>::MaxLayer(int *ptr)
    : returnValue(ptr), currentLayer(0), maxLayer(0) {}

template <class T> MaxLayer<T>::~MaxLayer() {
  if (returnValue)
    *returnValue = maxLayer;
}

template <class T> void MaxLayer<T>::operator()(BinaryNode<T> *&arg) {
  if (!arg) {
    maxLayer = (currentLayer > maxLayer ? currentLayer : maxLayer);
    currentLayer = 0;
  } else {
    currentLayer++;
  }
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

template <class T>
TreapNode<T>::TreapNode() : priority(0), left(nullptr), right(nullptr) {}

template <class T> TreapNode<T>::~TreapNode() {}

template <class T> BinaryNode<T> *TreapNode<T>::getLeft() { return left; }

template <class T> BinaryNode<T> *TreapNode<T>::getRight() { return right; }

template <class T> T &TreapNode<T>::getData() { return data; }

////////////////////////////////////////////////////////////////////////////////

template <class T> Treap<T>::Treap() : root(nullptr) {}

template <class T> Treap<T>::~Treap() {
  while (root)
    Delete(root->data);
}

template <class T> BinaryNode<T> *Treap<T>::getRoot() { return root; }

template <class T>
void Treap<T>::_Split(TreapNode<T> *node, int key, TreapNode<T> *&left,
                      TreapNode<T> *&right) {
  if (!node) {
    left = nullptr;
    right = nullptr;
  } else if (node->data <= key) {
    _Split(node->right, key, node->right, right);
    left = node;
  } else {
    _Split(node->left, key, left, node->left);
    right = node;
  }
}

template <class T>
TreapNode<T> *Treap<T>::_Merge(TreapNode<T> *left, TreapNode<T> *right) {
  if (!left || !right) {
    return (left ? left : right);
  }
  if (left->priority > right->priority) {
    left->right = _Merge(left->right, right);
    return left;
  } else {
    right->left = _Merge(left, right->left);
    return right;
  }
}

template <class T> void Treap<T>::Add(const T &key, const int &priority) {
  if (!root) {
    root = new TreapNode<T>();
    root->data = key;
    root->priority = priority;
    return;
  }

  TreapNode<T> **node = &root;
  while (*node) {
    if ((*node)->priority < priority)
      break;
    if (key <= (*node)->data)
      node = &(*node)->left;
    else
      node = &(*node)->right;
  }

  TreapNode<T> *newNode = new TreapNode<T>();
  newNode->left = nullptr;
  newNode->right = nullptr;
  newNode->data = key;
  newNode->priority = priority;

  _Split(*node, key, newNode->left, newNode->right);

  *node = newNode;
}

template <class T> void Treap<T>::Delete(const T &key) {
  TreapNode<T> **node = &root;

  while (*node) {
    if ((*node)->data == key) {
      auto oldLeft = (*node)->left;
      auto oldRight = (*node)->right;
      delete *node;

      *node = _Merge(oldLeft, oldRight);
      break;
    }
    if (key <= (*node)->data)
      node = &(*node)->left;
    else
      node = &(*node)->right;
  }
}

template <class T> bool Treap<T>::Has(const T &key) {
  TreapNode<T> *node = root;

  while (node) {
    if (node->data == key) {
      return true;
    }
    if (key <= node->data)
      node = node->left;
    else
      node = node->right;
  }

  return false;
}
////////////////////////////////////////////////////////////////////////////////

std::vector<std::pair<int, int>> GetPairsFromStdin() {
  int n = 0;
  int input1 = 0, input2 = 0;
  std::cin >> n;

  auto ret = std::vector<std::pair<int, int>>();
  ret.resize(n);
  for (int i = 0; i < n; i++) {
    std::cin >> input1 >> input2;
    ret[i] = std::make_pair(input1, input2);
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

template <class T>
void AddToBinarySearchTree(BinarySearchTree<T> &bst,
                           std::vector<std::pair<T, int>> const &vec) {
  std::vector<T> items;
  for (auto const &i : vec) {
    items.push_back(i.first);
  }
  AddToBinarySearchTree(bst, items);
}

template <class T>
void AddToTreap(Treap<T> &trp, std::vector<std::pair<T, int>> const &vec) {
  for (auto const &i : vec) {
    trp.Add(i.first, i.second);
  }
}

////////////////////////////////////////////////////////////////////////////////
