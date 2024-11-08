#include "../headers/MyContainer.h"
#include <algorithm>

template <typename T>
void MyContainer<T>::add(const T& value) {
    data.push_back(value);
}

template <typename T>
void MyContainer<T>::remove(const T& value) {
    data.erase(std::remove(data.begin(), data.end(), value), data.end());
}

template <typename T>
size_t MyContainer<T>::size() const {
    return data.size();
}

template <typename T>
MyContainer<T>::MyIterator::MyIterator(typename std::vector<T>::iterator start, typename std::vector<T>::iterator end) : current(start), end(end) {}

template <typename T>
T& MyContainer<T>::MyIterator::operator*() const {
    return *current;
}

template <typename T>
typename MyContainer<T>::MyIterator& MyContainer<T>::MyIterator::operator++() {
    ++current;
    return *this;
}

template <typename T>
bool MyContainer<T>::MyIterator::operator!=(const MyIterator& other) const {
    return current != other.current;
}

template <typename T>
typename MyContainer<T>::MyIterator MyContainer<T>::begin() { return MyIterator(data.begin(), data.end()); }

template <typename T>
typename MyContainer<T>::MyIterator MyContainer<T>::end() { return MyIterator(data.end(), data.end()); }
