/*
 * Автор: Андреев Алексей, АПО-12
 * 
 * Задача: 
 * 2_4. Поиск близнеца
 * 
 * Дан отсортированный массив разлиных целых чисел A[0..n-1] и массив целых чисел B[0..m-1].
 * Для каждого элемента массива B[i] найдите минимальный индекс элемента массива A[k],
 * ближайшего по значению к B[i]. 
 * 
 * n <= 110000, m <= 1000. 
 * Время работы поиска для каждого элемента B[i]: O(log(k))
 */

#include <iostream>

class Array {
public:
  Array() = delete;
  Array(size_t);
  ~Array();

  size_t get_max_size();
  int& operator[](const size_t &index);

private:
  int *buffer;
  size_t maxSize;
};

Array read_array_from_stdin();

size_t find_closest_to(Array &arr, int element);

int main() {
  Array A = read_array_from_stdin();
  Array B = read_array_from_stdin();

  for (size_t i = 0; i < B.get_max_size(); i++) {
    std::cout << find_closest_to(A, B[i]) << " ";
  }
}

Array::Array(size_t init_size) : maxSize(init_size), buffer(nullptr) {
  this->buffer = new int[init_size];
}

Array::~Array() { delete[] this->buffer; }

size_t Array::get_max_size() { return this->maxSize; }

int& Array::operator[](const size_t &index) {
  return this->buffer[index];
}

Array read_array_from_stdin() {
  int n = 0;
  std::cin >> n;
  Array arr(n);
  for (int i = 0; i < n; i++) {
    std::cin >> arr[i];
  }

  return arr;
}

size_t find_closest_to(Array &arr, int element) {
  size_t max_index = arr.get_max_size() - 1;
  //тривиальные случаи
  if (element <= arr[0]) {
    return 0;
  } else if (element >= arr[max_index]) {
    return max_index;
  } 

  size_t begin = 0;
  size_t end = 2;
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
  if (abs(element - arr[begin]) <= abs (element - arr[end])) {
    return begin;
  } else {
    return end;
  }
}