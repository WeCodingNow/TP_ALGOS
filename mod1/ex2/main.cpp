/*
 * Автор: Андреев Алексей, АПО-12
 *
 * Задача:
 * 2_4. Поиск близнеца
 *
 * Дан отсортированный массив разлиных целых чисел A[0..n-1] и массив целых
 * чисел B[0..m-1]. Для каждого элемента массива B[i] найдите минимальный индекс
 * элемента массива A[k], ближайшего по значению к B[i].
 *
 * n <= 110000, m <= 1000.
 * Время работы поиска для каждого элемента B[i]: O(log(k))
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

size_t find_closest_to(Array<int> &arr, int element);

template<typename T>
Array<T> solve(Array<T> &A, Array<T> &B);

int main() {
  auto A = read_array_from_stdin();
  auto B = read_array_from_stdin();

  auto answer = solve(A, B);
  for (size_t i = 0; i < answer.get_max_size(); i++) {
    std::cout << answer[i] << " ";
  }
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
T &Array<T>::operator[](const size_t &index) { return this->buffer[index]; }

Array<int> read_array_from_stdin() {
  int n = 0;
  std::cin >> n;
  Array<int> arr(n);
  for (int i = 0; i < n; i++) {
    std::cin >> arr[i];
  }

  return arr;
}

size_t find_closest_to(Array<int> &arr, int element) {
  size_t max_index = arr.get_max_size() - 1;
  //тривиальные случаи
  if (element <= arr[0]) {
    return 0;
  } else if (element >= arr[max_index]) {
    return max_index;
  }

  size_t begin = 0;
  size_t end = 1;
  //ищем окрестность, в которой находится "близнец"
  while (end < max_index) {
    if (element < arr[end]) {
      break;
    }
    begin = end;
    end *= 2;
  }

  if (end > max_index) {
    end = max_index;
  }

  //бинарный поиск
  size_t mid = (begin + end) / 2;
  while (begin != mid) {
    if (element < arr[mid]) {
      end = mid;
    } else if (element > arr[mid]) {
      begin = mid;
    } else {
      begin = mid;
      end = mid;
      break;
    }
    mid = (begin + end) / 2;
  }

  //у нас в конце поиска остается выбор из двух элементов: левый или правый.
  //в случае, если оба элемента - "близнецы", приоритетнее левый элемент
  if (abs(element - arr[begin]) <= abs(element - arr[end])) {
    return begin;
  } else {
    return end;
  }
}

template<typename T>
Array<T> solve(Array<T> &A, Array<T> &B) {
  Array<T> return_array(B.get_max_size());
  for (size_t i = 0; i < B.get_max_size(); i++) {
    return_array[i] = find_closest_to(A, B[i]);
  }
  return return_array;
}