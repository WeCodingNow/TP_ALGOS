/*
 * Автор: Андреев Алексей АПО-12
 *
 * Задача: 1.1. Хеш-таблица.
 *
 * Реализуйте структуру данных типа “множество строк” на основе динамической
 * хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из
 * строчных латинских букв. Хеш-функция строки должна быть реализована с помощью
 * вычисления значения многочлена методом Горнера.
 *
 * Начальный размер таблицы должен быть равным 8-ми.
 * Перехеширование выполняйте при добавлении элементов в случае, когда
 * коэффициент заполнения таблицы достигает 3/4. Структура данных должна
 * поддерживать операции добавления строки в множество, удаления строки из
 * множества и проверки принадлежности данной строки множеству.
 *
 * 1_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая
 * проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
 */

#include <iostream>

const int DEFAULT_HASH_TABLE_SIZE = 8;
const int DEFAULT_HASH_FUNCTION_COEF = 127;
const double MAX_FILL_COEF = 3.0 / 4.0;

/* TODO:
/ Сделать конструктор копирования
*/

template <class T> class HashTable {
public:
  HashTable(int coef = DEFAULT_HASH_FUNCTION_COEF,
            int maxSize = DEFAULT_HASH_TABLE_SIZE);
  HashTable(const HashTable &other);
  ~HashTable();

  bool Has(const T &key) const;
  bool Add(const T &key);
  bool Delete(const T &key);

private:
  T **table;
  bool *deleted;

  int maxSize;
  int hash_function_coef;
  int keysCount;

  int _Hash(const T &key) const;
  // h(k) не меняется, => сокращаем число вычислений, не вычисляя хеш для
  // каждого
  int _Hash(const int &hash, const int &i) const;

  int _Has(const T &key) const;
  double _Coef() const;
  void _GrowTable();
};

class HashTableController {
public:
  HashTableController(HashTable<std::string> *ht) : ht(ht) {}

  void add(std::string const &str) {
    std::cout << (ht->Add(str) ? "OK" : "FAIL") << std::endl;
  };

  void has(const std::string &str) {
    std::cout << (ht->Has(str) ? "OK" : "FAIL") << std::endl;
  }

  void remove(const std::string &str) {
    std::cout << (ht->Delete(str) ? "OK" : "FAIL") << std::endl;
  }

  void REPL() {
    char op;
    std::string input_str;
    while (std::cin >> op >> input_str) {
      switch (op) {
      case '+':
        add(input_str);
        break;
      case '-':
        remove(input_str);
        break;
      case '?':
        has(input_str);
        break;
      }
    }
  }

private:
  HashTable<std::string> *ht;
};

int main() {
  auto ht = HashTable<std::string>();
  auto htd = HashTableController(&ht);

  htd.REPL();
  return 0;
}

template <class T>
HashTable<T>::HashTable(int coef, int maxSize)
    : table(nullptr), deleted(nullptr), hash_function_coef(coef),
      maxSize(maxSize), keysCount(0) {
  table = new T *[maxSize];
  deleted = new bool[maxSize];
  for (int i = 0; i < maxSize; i++) {
    table[i] = nullptr;
    deleted[i] = false;
  }
}

template <class T> HashTable<T>::HashTable(const HashTable &other) {
  table = new T *[other.maxSize];
  for (int i = 0; i < other.maxSize; i++) {
    table[i] = other.table[i];
    deleted[i] = other.deleted[i];
  }
}

template <class T> HashTable<T>::~HashTable() {
  for (int i = 0; i < maxSize; i++) {
    if (table[i])
      delete table[i];
  }
  delete[] table;
  delete[] deleted;
}

template <class T> bool HashTable<T>::Has(const T &key) const {
  return _Has(key) != -1;
}

template <class T> bool HashTable<T>::Add(const T &key) {
  if (Has(key)) {
    return false;
  }

  if (_Coef() >= MAX_FILL_COEF) {
    _GrowTable();
  }

  int hash = _Hash(key);

  int first_hash = hash;

  int i = 0;
  while (table[hash] != nullptr || deleted[hash]) {
    //нашли место в котором был когда-то удалён элемент,
    //при этом добавляемого ключа точно нет в таблице; можно добавлять
    if (deleted[hash])
      break;
    hash = _Hash(hash, ++i);

    if (hash == first_hash) {
      // Если мы прошли по всей таблице и не нашли свободного места, добавление
      // провалилось
      return false;
    }
  }

  keysCount++;
  table[hash] = new T(key);
  deleted[hash] = false;
}

template <class T> bool HashTable<T>::Delete(const T &key) {
  int hash = _Has(key);
  if (hash == -1)
    return false;

  delete table[hash];
  table[hash] = nullptr;
  deleted[hash] = true;
  keysCount--;

  return true;
}

template <> int HashTable<std::string>::_Hash(const std::string &key) const {
  int hash = 0;
  for (auto &c : key)
    hash = (hash * hash_function_coef + c) % maxSize;

  return hash;
}

template <class T>
int HashTable<T>::_Hash(const int &hash, const int &i) const {
  return (hash + i * i) % maxSize;
}

template <class T> int HashTable<T>::_Has(const T &key) const {
  if (keysCount == 0)
    return -1;

  int hash = _Hash(key);
  int first_hash = hash;
  int i = 0;
  while (table[hash] != nullptr || deleted[hash]) {
    if (!deleted[hash] && *table[hash] == key) {
      return hash;
    }

    hash = _Hash(hash, ++i);

    if (hash == first_hash) {
      break;
    }
    // Если мы прошли по всей таблице и не смогли найти наш элемент
  }
  return -1;
}

template <class T> double HashTable<T>::_Coef() const {
  return double(keysCount) / double(maxSize);
}

template <class T> void HashTable<T>::_GrowTable() {

  int oldSize = maxSize;
  T **oldTable = table;

  maxSize *= 2;
  table = new T *[maxSize];

  delete[] deleted;
  deleted = new bool[maxSize];
  for (int i = 0; i < maxSize; i++) {
    table[i] = nullptr;
    deleted[i] = false;
  }

  //переносим ключи из старой таблицы в новую
  keysCount = 0;
  for (int i = 0; i < oldSize; i++) {
    if (oldTable[i]) {
      Add(*oldTable[i]);
      delete oldTable[i];
    }
  }

  delete[] oldTable;
}