#ifndef MYCONTAINER_H
#define MYCONTAINER_H

#include <vector>
#include <iterator> =

template <typename T>
class MyContainer {
private:
    std::vector<T> data;
public:
    void add(const T& value);
    void remove(const T& value);
    size_t size() const;

    class MyIterator; 
    MyIterator begin();
    MyIterator end();
};

template <typename T>
class MyContainer<T>::MyIterator {
private:
    typename std::vector<T>::iterator current;
    typename std::vector<T>::iterator end;
public:
    MyIterator(typename std::vector<T>::iterator start, typename std::vector<T>::iterator end);
    T& operator*() const;
    MyIterator& operator++();
    bool operator!=(const MyIterator& other) const;
    
};
#endif 