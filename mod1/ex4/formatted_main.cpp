/*
 * Вовочка ест фрукты из бабушкиной корзины.
 * В корзине лежат фрукты разной массы.
 * Вовочка может поднять не более K грамм.
 * Каждый фрукт весит не более K грамм.
 * За раз он выбирает несколько самых тяжелых фруктов, которые может поднять
 * одновременно, откусывает от каждого половину и кладет огрызки обратно в
 * корзину. Если фрукт весит нечетное число грамм, он откусывает большую
 * половину. Фрукт массы 1гр он съедает полностью.
 *
 * Определить за сколько подходов Вовочка съест все фрукты в корзине.
 *
 * Формат ввода
 * Вначале вводится n - количество фруктов и n строк с массами фруктов. n ≤
 * 50000. Затем K - "грузоподъемность". K ≤ 1000.
 *
 * Формат вывода
 * Неотрицательное число - количество подходов к корзине.
 */

#include <iostream>

const int ARRAY_DEFAULT_SIZE = 8;
const int ARRAY_GROWTH_MULTIPLIER = 2;

template <typename T> class Array {
public:
  Array();
  Array(size_t);
  Array(const Array<T> &);
  ~Array();

  size_t get_max_size();
  size_t get_current_size();
  bool is_empty();

  void push_back(const T &);
  void pop(size_t);
  void swap(size_t, size_t);
  T &operator[](const size_t &);

private:
  T *buffer;
  size_t current_size;
  size_t max_size;

  void grow();
};

template <typename T> class Heap {
public:
  Heap();
  Heap(const Array<T> &);
  ~Heap();

  void insert(const T &);
  T get_top();
  T peek_top();
  bool is_empty();

private:
  Array<T> arr;

  void sift_down(int);
  void sift_up(int);
  void build_heap();
};

unsigned int how_many_trips(Array<unsigned int> &fruits, unsigned int k);

int main() {
  unsigned int n = 0;
  std::cin >> n;

  Array<unsigned int> fruit_masses(n);
  for (size_t i = 0; i < n; i++) {
    unsigned int next_fruit = 0;
    std::cin >> next_fruit;
    fruit_masses.push_back(next_fruit);
  }

  unsigned int k = 0;
  std::cin >> k;

  std::cout << how_many_trips(fruit_masses, k);
}

template <typename T>
Array<T>::Array()
    : current_size(0), max_size(ARRAY_DEFAULT_SIZE), buffer(nullptr) {
  this->buffer = new T[max_size];
}

template <typename T>
Array<T>::Array(size_t needed_size)
    : current_size(0), max_size(needed_size), buffer(nullptr) {
  this->buffer = new T[max_size];
}

template <typename T>
Array<T>::Array(const Array<T> &_arr)
    : current_size(_arr.current_size), max_size(_arr.max_size),
      buffer(nullptr) {
  this->buffer = new T[max_size];
  for (size_t i = 0; i < current_size; i++) {
    this->buffer[i] = _arr.buffer[i];
  }
}

template <typename T> Array<T>::~Array() { delete[] this->buffer; }

template <typename T> size_t Array<T>::get_max_size() { return this->max_size; }

template <typename T> size_t Array<T>::get_current_size() {
  return this->current_size;
}

template <typename T> bool Array<T>::is_empty() {
  return this->current_size == 0;
}

template <typename T> void Array<T>::grow() {
  T *tmp_buf = this->buffer;
  this->buffer = new T[max_size * ARRAY_GROWTH_MULTIPLIER];

  for (size_t i = 0; i < max_size; i++) {
    this->buffer[i] = tmp_buf[i];
  }
  delete[] tmp_buf;

  this->max_size *= ARRAY_GROWTH_MULTIPLIER;
}

template <typename T> void Array<T>::push_back(const T &element) {
  if (current_size + 1 > max_size) {
    this->grow();
  }

  buffer[current_size++] = element;
};

template <typename T> void Array<T>::pop(size_t index) {
  if (current_size != 0) {
    current_size--;

    for (size_t i = index; i < current_size; i++) {
      this->swap(i, i + 1);
    }
  }
}

template <typename T> void Array<T>::swap(size_t lhs, size_t rhs) {
  if (current_size > 1) {
    T tmp_object = this->buffer[lhs];
    this->buffer[lhs] = this->buffer[rhs];
    this->buffer[rhs] = tmp_object;
  }
}

template <typename T> T &Array<T>::operator[](const size_t &index) {
  return this->buffer[index];
}

template <typename T> Heap<T>::Heap() : arr(Array<T>()) {}

template <typename T> Heap<T>::Heap(const Array<T> &_arr) : arr(_arr) {
  this->build_heap();
}

template <typename T> Heap<T>::~Heap() {}

template <typename T> void Heap<T>::insert(const T &element) {
  arr.push_back(element);
  sift_up(arr.get_current_size() - 1);
}

template <typename T> T Heap<T>::get_top() {

  T tmp_object = arr[0];

  arr.swap(0, arr.get_current_size() - 1);
  arr.pop(arr.get_current_size() - 1);

  if (!arr.is_empty()) {
    sift_down(0);
  }

  return tmp_object;
}

template <typename T> T Heap<T>::peek_top() { return arr[0]; }

template <typename T> bool Heap<T>::is_empty() { return arr.is_empty(); }

template <typename T> void Heap<T>::sift_down(int index) {
  int left = 2 * index + 1;
  int right = 2 * index + 2;

  int largest = index;
  if (left < arr.get_current_size() && arr[left] > arr[index]) {
    largest = left;
  }
  if (right < arr.get_current_size() && arr[right] > arr[largest]) {
    largest = right;
  }

  if (largest != index) {
    arr.swap(index, largest);
    sift_down(largest);
  }
}

template <typename T> void Heap<T>::sift_up(int index) {
  while (index > 0) {
    int parent = (index - 1) / 2;
    if (arr[index] <= arr[parent]) {
      return;
    }
    arr.swap(index, parent);
    index = parent;
  }
}

template <typename T> void Heap<T>::build_heap() {
  for (int i = arr.get_current_size() / 2 - 1; i >= 0; --i) {
    sift_down(i);
  }
}

unsigned int how_many_trips(Array<unsigned int> &fruits, unsigned int k) {

  auto heap = Heap<unsigned int>(fruits);
  Array<unsigned int> hand;

  int trips = 0;
  while (!heap.is_empty()) {
    int hand_space = 0;
    // пока в руке есть место - добавляем фрукты в руку
    while (!heap.is_empty() && hand_space + heap.peek_top() <= k) {
      hand_space += heap.peek_top();
      hand.push_back(heap.get_top() / 2);
    }
    // Пока в руке есть фрукты, которые можно добавить обратно в кучу (больше 0
    // грамм), добавляем обратно в кучу
    while (!hand.is_empty()) {
      int val = hand[hand.get_current_size() - 1];
      hand.pop(hand.get_current_size() - 1);
      if (val != 0)
        heap.insert(val);
    }
    trips++;
  }
  return trips;
}