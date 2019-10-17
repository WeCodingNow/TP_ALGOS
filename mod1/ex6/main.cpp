/*
 * Автор: Андреев Алексей, АПО-12
 *
 * Задача:
 * 6_1. Порядковая статистика.
 *
 * Даны неотрицательные целые числа n, k и массив целых чисел из диапазона
 * [0..109] размера n. Требуется найти k-ю порядковую статистику. т.е.
 * напечатать число, которое бы стояло на позиции с индексом k ∈[0..n-1] в
 * отсортированном массиве.
 *
 * Напишите нерекурсивный алгоритм.
 *
 * Требования к дополнительной памяти: O(n).
 * Требуемое среднее время работы: O(n).
 *
 * Функцию Partition следует реализовывать методом прохода двумя итераторами в
 * одном направлении.
 *
 * Описание для случая прохода от начала массива к концу:
 * Выбирается опорный элемент.
 * Опорный элемент меняется с последним элементом массива.
 * Во время работы Partition в начале массива содержатся элементы, не бОльшие
 * опорного. Затем располагаются элементы, строго бОльшие опорного. В конце
 * массива лежат нерассмотренные элементы. Последним элементом лежит опорный.
 * Итератор (индекс) i указывает на начало группы элементов, строго бОльших
 * опорного. Итератор j больше i, итератор j указывает на первый нерассмотренный
 * элемент.
 *
 * Шаг алгоритма.
 * Рассматривается элемент, на который указывает j.
 * Если он больше опорного, то сдвигаем j.
 * Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и
 * сдвигаем j. В конце работы алгоритма меняем опорный и элемент, на который
 * указывает итератор i.
 *
 * Реализуйте стратегию выбора опорного элемента "медиана трёх".
 * Функцию Partition реализуйте методом прохода двумя итераторами от начала
 * массива к концу.
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

template <typename T> class IsLess {
public:
  bool operator()(const T &lhs, const T &rhs) { return lhs < rhs; }

private:
};

Array<int> read_array_from_stdin(int);

bool is_less_int(const int &lhs, const int &rhs);

template <class T, class Compare>
int pivot(Array<T> &arr, Compare cmp, int begin, int end);

template <class T, class Compare>
int partition(Array<T> &arr, Compare cmp, int from, int amount);

template <class T, class Compare> int k_stat(Array<T> &arr, Compare cmp, T k);

int main() {
  int n = 0, k = 0;

  std::cin >> n >> k;
  auto input = read_array_from_stdin(n);

  std::cout << k_stat(input, is_less_int, k) << std::endl;
}

template <typename T>
Array<T>::Array()
    : current_size(0), max_size(ARRAY_DEFAULT_SIZE), buffer(nullptr) {
  this->buffer = new T[max_size];
}

template <typename T>
Array<T>::Array(size_t needed_size)
    : current_size(needed_size), max_size(needed_size), buffer(nullptr) {
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
  if (current_size > 1 && lhs != rhs) {
    T tmp_object = this->buffer[lhs];
    this->buffer[lhs] = this->buffer[rhs];
    this->buffer[rhs] = tmp_object;
  }
}

template <typename T> T &Array<T>::operator[](const size_t &index) {
  return this->buffer[index];
}

Array<int> read_array_from_stdin(int n) {
  Array<int> arr(n);
  for (int i = 0; i < n; i++) {
    std::cin >> arr[i];
  }

  return arr;
}

template <class T, class Compare>
int pivot(Array<T> &arr, Compare cmp, int begin, int end) {
  int middle = (begin + end - 1) / 2;

  if (cmp(arr[begin], arr[middle])) {
    if (cmp(arr[end - 1], arr[begin])) {
      return begin;
    } else if (cmp(arr[end - 1], arr[middle])) {
      return end - 1;
    }
    return middle;
  } else {
    if (cmp(arr[end - 1], arr[middle])) {
      return middle;
    } else if (cmp(arr[end - 1], arr[begin])) {
      return end - 1;
    }
    return begin;
  }
}

bool is_less_int(const int &lhs, const int &rhs) { return lhs < rhs; }

template <class T, class Compare>
int partition(Array<T> &arr, Compare cmp, int begin, int end) {

  int pivot_pos = pivot(arr, cmp, begin, end);

  arr.swap(pivot_pos, end - 1);

  const T &pivot = arr[end - 1];
  int i = begin, j = begin;
  while (j < end - 1) {
    if (cmp(arr[j], pivot) || arr[j] == pivot) {
      arr.swap(i, j);
      i++;
    }
    j++;
  }

  arr.swap(i, end - 1);
  return i;
}

template <class T, class Compare>
int k_stat(Array<T> &orig_arr, Compare cmp, T k) {
  Array<T> arr = orig_arr;
  int begin = 0;
  int end = arr.get_current_size();

  int part = partition(arr, cmp, 0, end);
  while (part != k) {
    if (part > k) {
      end = part;
      part = partition(arr, cmp, begin, end);
    } else {
      begin = part + 1;
      part = partition(arr, cmp, begin, end);
    }
  }

  return arr[part];
}