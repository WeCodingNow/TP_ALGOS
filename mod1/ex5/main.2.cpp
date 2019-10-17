/*
 * Автор: Андреев Алексей, АПО-12
 *
 * Задача:
 * 5_4. Закраска прямой в один слой
 * 
 * На числовой прямой окрасили N отрезков.
 * Известны координаты левого и правого концов каждого отрезка [Li, Ri].
 * Найти сумму длин частей числовой прямой, окрашенных ровно в один слой.
 * 
 * N ≤ 10000. Li, Ri — целые числа в диапазоне [0, 109].
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

struct Point {
    int x;
    int y;
    Point() : x(0), y(0) {};
};

struct PaintDifferential {
    int x;
    int delta;
    PaintDifferential () : x (0), delta (0) {};
    PaintDifferential (int _x, int _delta) : x (_x), delta (_delta) {};
};

template<typename T>
class IsLess {
public:
    bool operator()(const T& lhs, const T& rhs) {
        return lhs < rhs;
    }
private:
};

Array<Point> read_array_from_stdin();

Array<PaintDifferential> read_paint_differentials_from_stdin();

int find_longest_painted_line (Array<PaintDifferential>&, int = 1);

int main() {
    auto points = read_paint_differentials_from_stdin();
    std::cout << find_longest_painted_line(points) << std::endl;
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

Array<Point> read_array_from_stdin() {
  int n = 0;
  std::cin >> n;
  Array<Point> arr(n);
  for (int i = 0; i < n; i++) {
    std::cin >> arr[i].x >> arr[i].y;
  }

  return arr;
}

Array<PaintDifferential> read_paint_differentials_from_stdin() {
  int n = 0;
  std::cin >> n;
  Array<PaintDifferential> arr(n * 2);

  int l = 0, r = 0;
  for (int i = 0, j = 0; i < n; i++) {
      std::cin >> l >> r; 
      arr[j] = PaintDifferential(l, 1);
      arr[j + 1] = PaintDifferential(r, -1);
      j += 2;
  }

  return arr;
}

bool is_less_x(const PaintDifferential& lhs, const PaintDifferential& rhs) {
    return lhs.x < rhs.x;
}

bool is_less_delta(const PaintDifferential& lhs, const PaintDifferential& rhs) {
    return lhs.delta < rhs.delta;
}

template<typename T, typename Compare>
void selection_sort(Array<T> &arr, Compare cmp, int from = 0, int to = 0) {
    if (to == 0) {
        to = arr.get_current_size();
    }

    for (int i = from; i < to - 1; i++) {
      int min_index = i;
        for (int j = i + 1; j < to; j++)
            if (cmp(arr[j], arr[min_index]))
                min_index = j;
      arr.swap(i, min_index);
    }
}

template<typename T, typename Compare>
void insertion_sort(Array<T> &arr, Compare cmp, int from = 0, int to = 0) {
    if (to == 0) {
        to = arr.get_current_size();
    }

    for (int i = 1; i < to; i++) {
      T tmp = arr[i];
      int j = i - 1;
      for (; j >= 0 && cmp(tmp, arr[j]); j--)
        arr[j + 1] = arr[j];
      arr[j + 1] = tmp;
    }
}

//template<typename T, typename Compare>
//void merge_sort(Array<T> &arr, Compare cmp, int from = 0, int to = 0) {
    //if (to == 0) {
        //to = arr.get_current_size();
    //}

    //if (to - from <= 1) {
      //return;
    //}
    
    //int first_len = to / 2;
    //int second_len = to - first_len;

    //for (int i = 1; i < to; i++) {
      //T tmp = arr[i];
      //int j = i - 1;
      //for (; j >= 0 && cmp(tmp, arr[j]); j--)
        //arr[j + 1] = arr[j];
      //arr[j + 1] = tmp;
    //}
//}

template<class T, class Compare>
int partition (Array<T> &arr, Compare cmp, int from, int amount) {
    if (amount <= 1) {
      return 0;
    }

    const T &pivot = arr[from + amount - 1];
    int i = 0, j = amount - 2;
    while (i <= j) {
        for (; cmp(arr[from + i], pivot); ++i) {};
        for (; j >= 0 && !(cmp(arr[from + j], pivot)); --j) {};
        if (i < j) {
          arr.swap(from + i, from + j);
          i++; j--;
        }
    }
    arr.swap (from + i, from + amount - 1);
    return i;
}

template<class T, class Compare>
int partition_qs (Array<T> &arr, Compare cmp, int begin, int end) {
    if (end - begin <= 1) {
      return 0;
    }

    int pivot_pos = end - 1;

    const T &pivot = arr[pivot_pos];
    int i = begin, j = end - 2;
    while (i <= j) {
        for (; cmp(arr[i], pivot) && i < end; ++i) {};
        for (; j >= begin && !(cmp(arr[j], pivot)); --j) {};
        if (i < j) {
          arr.swap(i, j);
          i++; j--;
        }
    }
    arr.swap (i, pivot_pos);
    return i;
}

template<class T, class Compare>
int quick_sort (Array<T> &arr, Compare cmp, int begin, int end) {
    int part = partition_qs(arr, cmp, begin, end);
    if (part > 0)
      quick_sort(arr, cmp, begin, part);
    if (part + 1 < end) 
      quick_sort(arr, cmp, part + 1, end - (part + 1));
}

int find_longest_painted_line (Array<PaintDifferential> &arr, int target_height) {
    
    quick_sort(arr, is_less_x, 0, arr.get_current_size());

    int current_height = 0, the_length = 0, previous_coord = -1;
    for (int i = 0; i < arr.get_current_size(); i++) {
        current_height += arr[i].delta;
        if (current_height == target_height) {
            if (previous_coord == -1)
                previous_coord = arr[i].x;
        } else {
            if (previous_coord != -1) {
                the_length += arr[i].x - previous_coord;
                previous_coord = -1;
            }
        }
    }

    return the_length;
}