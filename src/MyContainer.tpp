#ifndef MYCONTAINER_TPP
#define MYCONTAINER_TPP
#include <iostream>
#include <../headers/MyContainer.h>
template <typename T>
void MyContainer<T>::add(const T& value) {
    data.push_back(value);
}
template <typename T>
void MyContainer<T>::remove(const T& value) {
    auto it = std::remove(data.begin(), data.end(), value);
    if (it != data.end()) {
        data.erase(it, data.end());
    } else {
        std::cerr << "Element not found in MyContainer::remove" << std::endl;
    }
}
template <typename T>
size_t MyContainer<T>::size() const {
    return data.size();
}
template <typename T>
bool MyContainer<T>::empty() const {
    return data.empty();
}
template <typename T>
void MyContainer<T>::clear() {
    data.clear();
}
template <typename T>
T& MyContainer<T>::operator[](size_t index) {
    return data[index];
}
template <typename T>
const T& MyContainer<T>::operator[](size_t index) const {
    return data[index];
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
typename MyContainer<T>::MyIterator MyContainer<T>::begin() {
    return MyIterator(data.begin(), data.end());
}
template <typename T>
typename MyContainer<T>::MyIterator MyContainer<T>::end() {
    return MyIterator(data.end(), data.end());
}
#endif 