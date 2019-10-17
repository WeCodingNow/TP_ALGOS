/*
 * Автор: Андреев Алексей, АПО-12
 *
 * Задача:
 * 3_1. Очередь с динамическим буфером
 *
 * Реализовать очередь с динамическим зацикленным буфером.
 * Обрабатывать команды push back и pop front.
 *
 * Формат ввода:
 * В первой строке количество команд n.
 * n <= 100000.
 *
 * Каждая команда задаётся как 2 целых числа: a b.
 * a = 2 - pop front
 * a = 3 - push back
 *
 * Если дана команда pop front, то число b - ожидаемое значение.
 * Если команда pop front вызвана для пустой струкуры данных, то ожидается "-1".
 *
 * Формат вывода:
 * Требуется напечатать YES - если все ожидаемые значения совпали.
 * Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.
 */

#include <iostream>

constexpr int QUEUE_INITIAL_SIZE = 4;
constexpr int QUEUE_GROWTH_MULTIPLIER = 2;
constexpr int POP_FRONT = 2;
constexpr int PUSH_BACK = 3;

template<typename T>
class Queue {
public:
  Queue(size_t initial_size = QUEUE_INITIAL_SIZE);
  ~Queue();

  void push_back(T element); // < -- поправить инт
  T pop_front();
  size_t size();

private:
  T *buffer;
  size_t buffer_size;
  size_t head;
  size_t tail;

  void grow();
};

void handle_input(Queue<int> &queue);

int main() {
  Queue<int> queue;

  handle_input(queue);

  return 0;
}

template<typename T>
Queue<T>::Queue(size_t initial_size) // <-- проверять initial_size на 0
    : buffer(nullptr), buffer_size(initial_size), head(0), tail(0) {
  buffer = new T[buffer_size];
}

template<typename T>
Queue<T>::~Queue() { delete[] buffer; }

template<typename T>
void Queue<T>::push_back(T element) {
  if ((tail + 1) % buffer_size == head) {
    this->grow();
  }

  this->buffer[tail] = element;
  tail = (tail + 1) % this->buffer_size;
}

template<typename T>
T Queue<T>::pop_front() {
  if (tail == head) {
    return -1;
  }
  
  int value = buffer[head];
  head = (head + 1) % buffer_size;
  return value;
}

template<typename T>
size_t Queue<T>::size() { return buffer_size; }

template<typename T>
void Queue<T>::grow() {
  size_t old_buffer_size = buffer_size;
  buffer_size *= QUEUE_GROWTH_MULTIPLIER;

  int* tmp_buffer = new T[buffer_size];

  if (head > tail) 
  {
    for (size_t i = 0; i < tail + 1; i++) {
      tmp_buffer[i] = buffer[i];
    }

    size_t counter = 0;
    for (size_t i = buffer_size - (old_buffer_size - head); i < buffer_size; i++) {
      tmp_buffer[i] = buffer[head + counter++];
    }

    head = buffer_size - (old_buffer_size - head);
  }
  else {
    for (size_t i = 0; i < old_buffer_size; i++) {
      tmp_buffer[i] = buffer[i];
    }
  }

  delete[] buffer;
  buffer = tmp_buffer;
}

void handle_input(Queue<int> &queue) {
  int n = 0;
  std::cin >> n;

  int a = 0, b = 0;
  bool expectations_met = true;
  for (int i = 0; i < n; i++) {
    std::cin >> a >> b;
    if (a == POP_FRONT) {
      int result = queue.pop_front();
      if (result != b) {
        expectations_met = false;
      }
    } else if (a == PUSH_BACK) {
      queue.push_back(b);
    }
  }

  //вынести вывод в main()
  if (expectations_met) {
    std::cout << "YES";
  } else {
    std::cout << "NO";
  }
}