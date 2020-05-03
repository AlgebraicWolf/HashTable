#include <iostream>

template<typename T>
class List {
private:
    size_t capacity;
    size_t size;

    size_t head;
    size_t tail;
    size_t freeHead;
    size_t freeTail;

    T *value;
    size_t *next;
public:
//    List();                                       // Default constructor
    List(size_t capacity = 32);                     // Constructor according to desired capacity
    List(const List &lst);                          // Copy constructor
    ~List() noexcept;                               // Destructor

    size_t Size();                                  // Get size
//    void Reserve(size_t n);                         // Allocate space in order to ensure n elements could be placed
    void Insert(size_t pos, const T &value);        // Insert value after element at pos
    void PushBack(const T &value);                  // Insert value at the end of the list
    T Get(size_t pos);                              // Get element
    T *ValueData();                                 // Array of values
    size_t *NextData();                             // Array of next positions
};

template<typename T>
List<T>::List(size_t capacity) : capacity(capacity), size(0), head(0), tail(0), freeHead(0) {
    value = new T[capacity];
    next = new size_t[capacity];

    for (int i = 0; i < capacity - 1; i++) {
        next[i] = i + 1;                           // Set addresses of next free positions
    }
}

template<typename T>
List<T>::~List() noexcept {
    delete[] value;
    delete[] next;
}

template<typename T>
void List<T>::PushBack(const T &val) {
    if (size < capacity) {
        value[freeHead] = val;

        if (!size) {
            head = freeHead;
            tail = freeHead;
        } else {
            next[tail] = freeHead;
            tail = freeHead;
        }

        freeHead = next[freeHead];
        size++;
    }
}

template<typename T>
T *List<T>::ValueData() {
    return value;
}

template<typename T>
size_t *List<T>::NextData() {
    return next;
}

template<typename T>
size_t List<T>::Size() {
    return size;
}

template<typename T>
T List<T>::Get(size_t pos) {
    if(pos < size) {
        int cur = head;
        for(int i = 0; i < pos; i++)
            cur = next[cur];

        return value[cur];
    }
    return static_cast<T>(0);
}

template<typename T>
void List<T>::Insert(size_t pos, const T& val) {
    if(size < capacity) {
        int cur = head;
        for(int i = 0; i < pos; i++) {
            cur = next[cur];
        }

        int newPos = freeHead;
        freeHead = next[freeHead];

        value[newPos] = val;
        next[newPos] = next[cur];
        next[cur] = newPos;
        size++;
        if(tail == cur)
            tail = newPos;
    }
}


template<typename FunctorObject>
class HashTable {
private:
    size_t capacity;                                // Hash table capacity
    List<int> *table;                               // Hash table itself
    FunctorObject hash;
public:
    HashTable();                                    // Default constructor
    HashTable(size_t n);                            // Constructor that ensures n different hash values could be stored
    ~HashTable() noexcept;                         // Destructor
    HashTable(const HashTable &other);              // Copy constructor
    HashTable(HashTable &&other);                   // Move constructor
    HashTable &operator=(const HashTable &other);   // Copy assignment
    HashTable &operator=(HashTable &&other);        // Move assignment

    void Insert(const char *key, int value);        // Insertion method
    int Get(const char *key);                       // Get value by key
    void Delete(const char *key);                   // Delete value by key

    void DumpListLength(const char *filename);      // Dump lengths of all the lists in the Hash Table

};

int main() {
    List<int> lst(64);
    lst.PushBack(0);
    for(int i = 1; i < 64; i ++) {
        lst.Insert(i - 1, i * 2);
        for(int j = 0; j < 64; j++) {
            std::cout << lst.NextData()[j] << " ";
        }
        std::cout << std::endl;
    }

    for(int i = 0; i < 64; i++) {
        std::cout << lst.Get(i) << std::endl;
    }
}