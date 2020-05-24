# Hash Table optimization

## Hash table 
Hash table is a data structure that implements an associative array i. e. data structure that can map keys to values. In order to do so, it computes hash value for each key and uses that hash as an array index. Since collisions between different keys are possible, this data structures utilizes the idea of buckets that store pairs of keys and values. One of the ways of implementing buckets is to use linked lists. Here is a good schematic of this datastructure [GeeksforGeeks article](https://www.geeksforgeeks.org/implementing-our-own-hash-table-with-separate-chaining-in-java/):

![Hash table schematic](img/HashTable.png)

I have implemented this Data Structure in C++.

## Toolchain and useful links
I've used [perf](https://github.com/torvalds/linux/tree/master/tools/perf) profiler along with [Hotspot GUI](https://github.com/KDAB/hotspot). [Intel Intrinsics Guide]() would also turn out to be incredebely useful.

## Dataset
In order to test Hash Table performance, I've used MAS [Open ANC](https://www.anc.org/data/oanc/) English corpus. It contains approximately 1.5B words with average word length of 4.85 letters.

![Word length distribution](img/Words.png)

I have managed to extract 200k unique words with average length of 9.15 letters and following length distribution:

![Unique word length distribution](img/Unique.png)

In order to choose hash function, I've loaded dataset into Hash Table of size that heavily overloads buckets. I've tested several hash functions:
1. strlen()
2. Sum of ASCII codes of symbols
3. Sum of ASCII codes of symbols divided by strlen()
4. Rolling hash
5. Jenkins one-at-a-time hash
6. CRC32C

Here is the comparison of bucket lengths with these functions:

![Bucket lengths graph](img/comparison.png)

It seems that CRC32C and joaat have best performance. If we look at close-up graph, we can see that they are almost identical

![Close-up graph of joaat and CRC32C](img/closeup.png)

For the performance reasons I've decided to proceed with CRC32C.