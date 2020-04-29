#include <iostream>

template <typename T>
class List {
private:
    size_t capacity;
    size_t size;

    size_t head;
    size_t tail;
    size_t freeHead;

    T* value;
    size_t* next;
public:
    List();                                         // Default constructor
    List(size_t capacity);                          // Constructor according to desired capacity
    List(const List& lst);                          // Copy constructor
    ~List() noexcept;                               // Destructor

    size_t Size();                                  // Get size
    void Reserve(size_t n);                         // Allocate space in order to ensure n elements could be placed
    void Insert(size_t pos, const T& value);        // Insert value after element at pos
    void PushBack(const T& value);                  // Insert value at the end of the list
    T Get(size_t pos);                              // Get element
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}