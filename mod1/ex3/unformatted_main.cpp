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

constexpr int QUEUE_INITIAL_SIZE = 16;
constexpr int POP_FRONT = 2;
constexpr int PUSH_BACK = 3;
//constexpr int QUEUE_GROWTH_MULTIPLIER = 2; 

class Queue {
public:
    Queue(size_t initial_size = QUEUE_INITIAL_SIZE);
    ~Queue();

    void push_back(int element);
    int pop_front();
    size_t size();
private:
    int* buffer;
    size_t buffer_size;
    size_t head;
    size_t tail;

    void grow();
};

void handle_input(Queue &queue);

int main() {
    int n = 0;
    int a = 0;
    int b = 0;

    std::cin >> n;
    Queue queue(n);

    handle_input(queue);

    return 0;
}

Queue::Queue(size_t initial_size) : buffer(nullptr), buffer_size(initial_size), head(0), tail(0) {
    buffer = new int[buffer_size];
    for (size_t i = 0; i < buffer_size; i++) {
        buffer[i] = -1;
    }
}

Queue::~Queue() {
    delete[] buffer;
}

void Queue::push_back(int element) {
    buffer[tail] = element;
    tail = (tail + 1) % buffer_size;
}

int Queue::pop_front() {
    int value = buffer[head];
    head = (head + 1) % buffer_size;
    return value;
}

size_t Queue::size() {
    return buffer_size;
}

void handle_input(Queue &queue) {
    size_t n = queue.size();

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

    if (expectations_met) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
}