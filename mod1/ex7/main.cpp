/*
 * Автор: Андреев Алексей, АПО-12
 * 
 * Задача:
 * 7_1. MSD для строк.
 * Дан массив строк.
 * Количество строк не больше 100000.
 * Отсортировать массив методом поразрядной сортировки MSD по символам.
 * Размер алфавита - 256 символов.
 * Последний символ строки = ‘0’.
 */

#include <iostream>
#include <string.h>

const int alphabet_size = 256;

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

  void push_back(const T&);
  void pop(size_t);
  void swap(size_t, size_t);
  T &operator[](const size_t &);

private:
  T *buffer;
  size_t current_size;
  size_t max_size;

  void grow();
};


char** read_strings_from_stdin(int *n);

void counting_sort(char **arr, int n, int digit, int *counts);

void MSD_sort(char** arr, int n, int digit = 0);

int main() {
    int n = 0;
    auto input = read_strings_from_stdin(&n);

    MSD_sort(input, n, 0);

    for (int i = 0; i < n; i++) {
      std::cout << input[i] << std::endl;
    }

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

char** read_strings_from_stdin(int *n) {
    Array<char*> arr;

    while(true) {
      std::string input_str;
      std::getline(std::cin, input_str);

      if (input_str.empty()) {
        break;
      }

      char *c_string = new char[input_str.length() + 1];
      strcpy(c_string, input_str.c_str());

      arr.push_back(c_string);
    }

    char** return_string = new char*[arr.get_current_size()];
    for (int i = 0; i < arr.get_current_size(); i++) {
      return_string[i] = arr[i];
    }

    *n = arr.get_current_size();
    return return_string;
}

void counting_sort(char **arr, int n, int digit, int *counts) {
    for (int i = 0; i < alphabet_size; i++) {
      counts[i] = 0;
    }

    for (int i = 0; i < n; ++i) {
      ++counts[arr[i][digit]];
    }

    int sum = 0;

    for (int i = 0; i < alphabet_size; ++i) {
      int tmp = counts[i];
      counts[i] = sum;
      sum += tmp;
    }

    char** aux = new char*[n];

    for (int i = 0; i < n; ++i) {
      aux[counts[arr[i][digit]]] = arr[i];
      counts[arr[i][digit]]++;
    }

    memcpy(arr, aux, n * sizeof(char*));
}

void MSD_sort(char **arr, int n, int digit) {
    int groups[alphabet_size + 1];
    groups[alphabet_size] = n;

    counting_sort(arr, n, digit, groups);

    for (int i = 0; i < alphabet_size; ++i) {
      int group_size = groups[i + 1] - groups[i];
      if (group_size > 1) {
        MSD_sort(arr + groups[i], group_size, digit + 1);
      }
    }
}
