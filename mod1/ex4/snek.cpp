/*
 * create by Aleksander
 * Вовочка ест фрукты из бабушкиной корзины.
 * В корзине лежат фрукты разной массы. Вовочка может поднять не более K грамм.
 * Каждый фрукт весит не более K грамм.
 * За раз он выбирает несколько самых тяжелых фруктов, которые может поднять одновременно,
 * откусывает от каждого половину и кладет огрызки обратно в корзину.
 * Если фрукт весит нечетное число грамм, он откусывает большую половину. Фрукт массы 1гр он съедает полностью.
 * Определить за сколько подходов Вовочка съест все фрукты в корзине.
 */

#include <iostream>

#define GROW_BUF_SIZE 2
#define DEFAULT_BUFFER_GROW_SIZE 5
#define DEFAULT_BUFFER_SIZE 16

template <class T>
class DefaultComparator {
public:
    bool operator() (const T& l, const T& r) {
        return l > r;
    }
};

template <typename T,  class Comparator = DefaultComparator<T>>
class Heap{
public:
    // Дефолтная инициализация
    Heap() :_bufferSize(DEFAULT_BUFFER_SIZE),
            _size(0) {
        _buffer = new T[_bufferSize];
    }
    // деструктор
    ~Heap() {
        delete[] _buffer;
    }
    // Добавить элемент
    void append(T value) {
        if(_size + 1 > _bufferSize){
            growSize(GROW_BUF_SIZE * _bufferSize);
        }
        _buffer[_size] = value;
        siftUp(_size - 1);
        _size++;
    }

    // Размер кучи
    int getSize() {
        return _size;
    }

    // Получить максимальный элемент, удалив его
    T   extractMax() {
            if(!_size)
                std::cerr << "Heap::extractTop : Empty heap\n";
            T min = _buffer[0];
            _buffer[0] = _buffer[_size - 1];
            _size--;
            if(_size)
                siftDown(0);
            return min;
        }

        // получить максимальный элемент
    T   getMaxValue() {
            if(!_size)
                std::cerr << "Heap::getTop : Empty heap\n";
            return _buffer[0];
        }

protected:
    // Проталкивание элемента вверх
    void siftUp(int index) {
        if(index >= _size)
            std::cerr << "Heap::siftUp : Wrong index\n";
        while(index && comparator(_buffer[index], _buffer[(index - 1)/2])){
            int parent = (index - 1)/2;
            std::swap(_buffer[index], _buffer[parent]);
            index = parent;
        }
    }
    // Проталкивание элемента вниз
    void siftDown(int index){
        if(index >= _size)
            std::cerr << "Heap::siftDown : Wrong index\n";
        int max = index;
        if(index*2 + 1 < _size && comparator(_buffer[index*2 + 1], _buffer[max]))
            max = index*2 + 1;
        if(index*2 + 2 < _size && comparator(_buffer[index*2 + 2], _buffer[max]))
            max = index*2 + 2;
        if(max != index){
            std::swap(_buffer[index], _buffer[max]);
            siftDown(max);
        }
    }

    // увеличить размер буфера
    void growSize(int n) {max = index*2 + 1;
        if(n < _bufferSize) return;

        int newBufferSize = (n > 0) ? n : DEFAULT_BUFFER_GROW_SIZE;

        T *newBuffer = new T[newBufferSize];

        for(size_t i = 0; i < _size; i++) {
            newBuffer[i] = _buffer[i];
        }

        delete[] _buffer;
        _buffer = newBuffer;
        _bufferSize = newBufferSize;
    }
    //компаратор
    Comparator comparator;
    T * _buffer;
    int _bufferSize;
    int _size;
};

int countSteps() {
    // Одна куча - чтобы править корзинкой, вторая куча - чтобы закидывать недоеденное обратно в корзинку
    Heap <int, DefaultComparator<int>> heap;
    Heap <int, DefaultComparator<int>> buffer;

    int fruitsCount, maxWeight;
    std::cin >> fruitsCount;
    for(int i = 0; i < fruitsCount; i++){
        int oneWeight;
        std::cin >> oneWeight;
        heap.append(oneWeight);
    }
    std::cin >> maxWeight;

    int counter = 0;
    while(heap.getSize() > 0){
        int currWeight = 0; // текущий вес фруктов
        while(heap.getSize() > 0 && currWeight + heap.getMaxValue() <= maxWeight) {
            // пока можем брать фрукты - берем максимальное из кучи и добавляем половину от него в буфер
            currWeight += heap.getMaxValue();
            buffer.append(heap.extractMax() / 2);
        }
        // Если буфер содержит объедки -> обратно в кучу
        while(buffer.getSize() > 0){
            int val = buffer.extractMax();
            if(val != 0)
                heap.append(val);
        }
        counter++;
    }

    return counter;
}

int main() {
    // считаем кол-во шагов для поедания фруктов
    int steps = countSteps();

    std::cout << steps << std::endl;

    return 0;
}