#include <fstream>
#include <iostream>
#include <cstring>
#include <immintrin.h>

template<int BucketSize>
class HashTable;

template<typename T>
class List {
private:
    size_t capacity;
    size_t size;


    size_t tail;
    size_t freeHead;

    T *values;
    size_t *nexts;
    size_t head;
public:


//    List();                                       // Default constructor
    explicit List(size_t capacity = 32);            // Constructor according to desired capacity
    List(const List &lst);                          // Copy constructor
    List(List &&lst) noexcept;                      // Move constructor
    ~List() noexcept;                               // Destructor
    List &operator=(const List &lst);               // Copy assignment
    List &operator=(List &&lst) noexcept;           // Move assignment

    size_t Head();                                  // Get head
    size_t Size();                                  // Get size
    void Reserve(size_t n);                         // Allocate space in order to ensure n elements could be placed
    void Insert(size_t pos, const T &value);        // Insert values after element at pos
    void PushBack(const T &value);                  // Insert values at the end of the list
    T Get(size_t pos);                              // Get element
    T *GetValuesArray();                            // Array of values
    size_t *GetNextsArray();                        // Array of next positions

    template<int BucketSize>
    friend
    class HashTable;
};

template<typename T>
size_t List<T>::Head() {
    return head;
}

template<typename T>
void List<T>::Reserve(size_t n) {
    if (capacity < n) {
        size_t *newNexts = new size_t[n];
        T *newValues = new T[n];

        memcpy(newNexts, nexts, sizeof(size_t) * capacity);
        memcpy(newValues, values, sizeof(T) * capacity);

        for (int i = capacity; i < n; i++) {
            newNexts[i] = i + 1;
        }

        delete[] values;
        delete[] nexts;

        values = newValues;
        nexts = newNexts;
    }
}

template<typename T>
List<T>::List(size_t capacity) : capacity(capacity), size(0), head(0), tail(0), freeHead(0) {
    values = new T[capacity];
    nexts = new size_t[capacity];

    for (int i = 0; i < capacity; i++) {
        nexts[i] = i + 1;                           // Set addresses of next free positions
    }
}

template<typename T>
List<T>::List(List &&lst) noexcept: capacity(lst.capacity), size(lst.size), head(lst.head), tail(lst.tail),
                                    freeHead(lst.freeHead) {
    values = lst.values;
    nexts = lst.nexts;

    lst.capacity = 0;
    lst.size = 0;
    lst.head = 0;
    lst.tail = 0;
    lst.freeHead = 0;

    lst.nexts = nullptr;
    lst.values = nullptr;
}

template<typename T>
List<T> &List<T>::operator=(const List &lst) {
    delete[] values;
    delete[] nexts;

    capacity = lst.capacity;
    size = lst.size;
    head = lst.head;
    tail = lst.tail;
    freeHead = lst.freeHead;

    values = new T[capacity];
    nexts = new size_t[capacity];

    memcpy(values, lst.values, sizeof(T) * capacity);
    memcpy(nexts, lst.nexts, sizeof(size_t) * capacity);

    return *this;
}

template<typename T>
List<T> &List<T>::operator=(List &&lst) noexcept {
    delete[] values;
    delete[] nexts;

    capacity = lst.capacity;
    size = lst.size;
    head = lst.head;
    tail = lst.tail;
    freeHead = lst.freeHead;

    values = lst.values;
    nexts = lst.nexts;

    lst.capacity = 0;
    lst.size = 0;
    lst.head = 0;
    lst.tail = 0;
    lst.freeHead = 0;

    lst.nexts = nullptr;
    lst.values = nullptr;

    return *this;
}

template<typename T>
List<T>::~List() noexcept {
    delete[] values;
    delete[] nexts;
}

template<typename T>
void List<T>::PushBack(const T &val) {
    if (size == capacity) {
        Reserve(2 * capacity);
        capacity *= 2;
    }

    values[freeHead] = val;

    if (!size) {
        head = freeHead;
        tail = freeHead;
    } else {
        nexts[tail] = freeHead;
        tail = freeHead;
    }

    freeHead = nexts[freeHead];
    size++;

}

template<typename T>
T *List<T>::GetValuesArray() {
    return values;
}

template<typename T>
size_t *List<T>::GetNextsArray() {
    return nexts;
}

template<typename T>
size_t List<T>::Size() {
    return size;
}

template<typename T>
T List<T>::Get(size_t pos) {
    if (pos < size) {
        int cur = head;
        for (int i = 0; i < pos; i++)
            cur = nexts[cur];

        return values[cur];
    }
    return static_cast<T>(0);
}

template<typename T>
void List<T>::Insert(size_t pos, const T &val) {
    if (size == capacity) {
        Reserve(2 * capacity);
        capacity *= 2;
    }

    int cur = head;
    for (int i = 0; i < pos; i++) {
        cur = nexts[cur];
    }

    int newPos = freeHead;
    freeHead = nexts[freeHead];

    values[newPos] = val;
    nexts[newPos] = nexts[cur];
    nexts[cur] = newPos;
    size++;
    if (tail == cur)
        tail = newPos;

}

template<typename T>
List<T>::List(const List<T> &lst): capacity(lst.capacity), size(lst.size), head(lst.head), tail(lst.tail),
                                   freeHead(lst.freeHead) {
    values = new T[capacity];
    nexts = new size_t[capacity];

    memcpy(values, lst.values, sizeof(T) * capacity);
    memcpy(nexts, lst.nexts, sizeof(size_t) * capacity);
}


template<int BucketSize>
class HashTable {
private:
    struct KeyValuePair {
        const char *key;
        int value;

        KeyValuePair() = default;

        KeyValuePair(const char *key, int value);
    };

    size_t capacity;                                // Hash table capacity
    List<KeyValuePair> *table;                      // Hash table itself
    unsigned int hash(const char *key);

    void releaseMemory();                           // Function to release memory
public:
//    HashTable();                                    // Default constructor
    explicit HashTable(
            size_t n = 997);                      // Constructor that ensures n different hash values could be stored
    ~HashTable() noexcept;                          // Destructor
    HashTable(const HashTable &other);              // Copy constructor
    HashTable(HashTable &&other) noexcept;                   // Move constructor
    HashTable &operator=(const HashTable &other);   // Copy assignment
    HashTable &operator=(HashTable &&other);        // Move assignment

    void Insert(const char *key, int value);        // Insertion method
    int Get(const char *key);                       // Get values by key
//    void Delete(const char *key);                   // Delete values by key

    void DumpListLength(const char *filename);      // Dump lengths of all the lists in the Hash Table
};

template<int BucketSize>
unsigned int HashTable<BucketSize>::hash(const char *key) {
    size_t len = strlen(key);
    unsigned int hash = 0;

    for (int i = 0; i < len; i++)
        hash = _mm_crc32_u8(hash, key[i]);

    return hash;
}

template<int BucketSize>
HashTable<BucketSize>::KeyValuePair::KeyValuePair(const char *key, int value): value(value) {
    this->key = key;
}

template<int BucketSize>
void HashTable<BucketSize>::releaseMemory() {
    delete[] table;
}

template<int BucketSize>
HashTable<BucketSize>::HashTable(size_t n): capacity(n) {
    table = new List<KeyValuePair>[capacity];
    for (int i = 0; i < capacity; i++) {
        table[i] = List<KeyValuePair>(BucketSize);
    }
}

template<int BucketSize>
HashTable<BucketSize>::~HashTable() noexcept {
    releaseMemory();
}

template<int BucketSize>
HashTable<BucketSize>::HashTable(const HashTable<BucketSize> &other): capacity(
        other.capacity) {
    table = new List<KeyValuePair>[capacity];

    for (int i = 0; i < capacity; i++) {
        table[i] = other.table[i];
    }
}

template<int BucketSize>
HashTable<BucketSize>::HashTable(HashTable<BucketSize> &&other) noexcept: capacity(
        other.capacity) {
    table = other.table;

    other.capacity = 0;
    other.table = nullptr;
}

template<int BucketSize>
HashTable<BucketSize> &
HashTable<BucketSize>::operator=(const HashTable<BucketSize> &other) {
    capacity = other.capacity;

    releaseMemory();

    table = new List<KeyValuePair>[capacity];
    for (int i = 0; i < capacity; i++) {
        table[i] = other.table[i];
    }

    return *this;
}

template<int BucketSize>
HashTable<BucketSize> &HashTable<BucketSize>::operator=(HashTable &&other) {
    releaseMemory();

    table = other.table;
    capacity = other.capacity;

    other.capacity = 0;
    other.table = nullptr;

    return *this;
}

template<int BucketSize>
void HashTable<BucketSize>::DumpListLength(const char *filename) {
    std::ofstream dumpFile;
    dumpFile.open(filename);

    for (int i = 0; i < capacity; i++) {
        dumpFile << table[i].Size() << '\n';
    }

    dumpFile.close();
}

template<int BucketSize>
void HashTable<BucketSize>::Insert(const char *key, int value) {
    unsigned int pos = hash(key) % capacity;
    List<KeyValuePair> &bucket = table[pos];

    KeyValuePair *bucketData = bucket.GetValuesArray();
    size_t *nexts = bucket.GetNextsArray();

    int cur = bucket.Head();

    for (int i = 0; i < bucket.Size(); i++) {
        if (!strcmp(bucketData[cur].key, key)) {
            bucketData[cur].value = value;
            return;
        }
        cur = nexts[cur];
    }
    bucket.PushBack(KeyValuePair(key, value));
}

template<int BucketSize>
int HashTable<BucketSize>::Get(const char *key) {
    size_t len = strlen(key);
    unsigned int hashValue = 0;

    for (int i = 0; i < len; i++)
        hashValue = _mm_crc32_u8(hashValue, key[i]);

    unsigned int bucketNum = hashValue % capacity;

    List<KeyValuePair> &bucket = table[bucketNum];

    size_t cur = bucket.head;
    size_t *nexts = bucket.nexts;
    KeyValuePair *elems = bucket.values;

    for (int i = 0; i < bucket.size; i++) {
        const char *cur_elem = elems[cur].key;
        asm goto (
        ".intel_syntax noprefix;"
        "mov rax, %0;"
        "mov rbx, %1;"
        "vpxor ymm2, ymm2, ymm2;"
        "cmpcycle:;"
        "vmovdqa ymm0, [rax];"
        "vmovdqa ymm1, [rbx];"
        "vpcmpeqb ymm3, ymm0, ymm1;"
        "vpmovmskb ecx, ymm3;"
        "not ecx;"
        "cmp ecx, 0;"
        "jne %l2;"
        "vpcmpeqb ymm3, ymm0, ymm2;"
        "vpmovmskb ecx, ymm3;"
        "cmp ecx, 0;"
        "lea rax, [rax+32];"
        "lea rbx, [rbx+32];"
        "je cmpcycle;"
        ".att_syntax prefix"
        :
        : "r"(key), "r"(cur_elem)
        : "rax", "rbx", "rcx", "ymm0", "ymm1", "ymm2", "ymm3"
        : mismatch);
        return elems[cur].value;

        mismatch:
        cur = nexts[cur];

//        __m256i key_batch = _mm256_load_si256(reinterpret_cast<const __m256i *>(key));
//        __m256i elem_batch = _mm256_load_si256(reinterpret_cast<const __m256i *>(cur_elem));
//
//        __m256i eos_mask = _mm256_set_epi64x(0, 0, 0, 0);
//        __m256i cmpres;
//        while(true) {
//            cmpres = _mm256_cmpeq_epi8(key_batch, elem_batch);
//            if(~_mm256_movemask_epi8(cmpres)) {
//               cur = nexts[cur];
//               break;
//            }
//
//            cmpres = _mm256_cmpeq_epi8(key_batch, eos_mask);
//            if(_mm256_movemask_epi8(cmpres)) {
//                return elems[cur].value;
//            }
//
//            key += 32;
//            cur_elem += 32;
//
//            key_batch = _mm256_load_si256(reinterpret_cast<const __m256i *>(key));
//            elem_batch = _mm256_load_si256(reinterpret_cast<const __m256i *>(cur_elem));
//        }
    }

    return -1;
}

size_t getFilesize(FILE *f) {
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    return size;
}


char *loadFile(const char *filename, size_t *fileSize) {
    FILE *input = fopen(filename, "r");

    size_t size = getFilesize(input);
    if (fileSize)
        *fileSize = size;

    char *content = (char *) calloc(size, sizeof(char));
    fread(content, sizeof(char), size, input);

    fclose(input);

    return content;
}

char **splitWordlist(char *words, size_t *wordCount) {
    *wordCount = 0;

    char *start = words;
    char *wordEnd = nullptr;

    size_t totalLength = 0;

    while (words) {
        wordEnd = strchr(words, '\n');
        if (!wordEnd)
            break;

        totalLength += (wordEnd - words + 1) + (32 - (wordEnd - words + 1) % 32);
        words = wordEnd;
        (*wordCount)++;
        *words = '\0';
        words++;
    }

    words = start;

    char **wordlist = new char *[*wordCount];
    char *alignedWords = new char[totalLength + 16];
    if (reinterpret_cast<size_t>(alignedWords) % 32)
        alignedWords += 16;

    for (int i = 0; i < *wordCount; i++) {
        wordlist[i] = alignedWords;
        memcpy(alignedWords, words, strlen(words));
        wordEnd = strchr(words, '\0');
        alignedWords += (wordEnd - words + 1) + (32 - (wordEnd - words + 1) % 32);
        words = wordEnd;
        words++;
    }

    return wordlist;
}

int main() {
    char *words = loadFile("wordlist.txt", nullptr);
    size_t wordcount = 0;
    char **wordlist = splitWordlist(words, &wordcount);

    HashTable<64> ht(wordcount);

    for (int i = 0; i < wordcount; i++) {
        ht.Insert(wordlist[i], i);
    }

    for (int t = 0; t < 400; t++) {
        for (int i = 0; i < wordcount; i++) {
            ht.Get(wordlist[i]);
        }
    }
}