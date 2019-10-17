/*
 * Автор: Андреев Алексей, АПО-12
 *
 * Задача:
 * 1_3 Нужная сумма
 *
 * Даны два строго возрастающих массива целых чисел A[0..n) и B[0..m) и число k.
 * Найти количество таких пар индексов (i, j), что A[i] + B[j] = k.
 * Время работы O(n + m). n, m <= 100000.
 *
 * Указание. Обходите массив B от конца к началу.
 */

#include <iostream>

template<typename T>
class Array {
public:
  Array() = delete;
  Array(size_t);
  Array(const Array<T>&);
  ~Array();

  size_t get_max_size();
  T &operator[](const size_t &index);
private:
  T *buffer;
  size_t max_size;
};

Array<int> read_array_from_stdin();

template<typename T>
int how_many_sums_equal_to_k(Array<T> &A, Array<T> &B, T k);

int main() {
  auto A = read_array_from_stdin();
  auto B = read_array_from_stdin();

  int k = 0;
  std::cin >> k;

  std::cout << how_many_sums_equal_to_k(A, B, k) << std::endl;
}

template<typename T>
Array<T>::Array(size_t init_size) : max_size(init_size), buffer(nullptr) {
  this->buffer = new T[init_size];
}

template<typename T>
Array<T>::Array(const Array<T> &_arr) : max_size(_arr.max_size), buffer(nullptr) {
  this->buffer = new T[max_size];
  for (size_t i = 0; i < max_size; i++) {
    this->buffer[i] = _arr.buffer[i];
  }
}

template<typename T>
Array<T>::~Array() { delete[] this->buffer; }

template<typename T>
size_t Array<T>::get_max_size() { return this->max_size; }

template<typename T>
T &Array<T>::operator[](const size_t &index) {
  return this->buffer[index];
}

Array<int> read_array_from_stdin() {
  int n = 0;
  std::cin >> n;
  Array<int> arr(n);
  for (int i = 0; i < n; i++) {
    std::cin >> arr[i];
  }

  return arr;
}

template<typename T>
int how_many_sums_equal_to_k(Array<T> &A, Array<T> &B, T k) {
  size_t i = 0, j = B.get_max_size();

  // здесь вычисляем один раз максимальный размер массива
  // A, чтобы не делать этого на каждой итерации цикла
  size_t a_max_size = A.get_max_size();

  unsigned int counter = 0;
  while (i < a_max_size && j > 0) {
    if (A[i] + B[j - 1] == k) {
      counter++;
      i++;
      j--;
    } else {
      if (A[i] + B[j - 1] > k) {
        j--;
        continue;
      } else {
        i++;
        continue;
      }
    }
  }

  return counter;
}