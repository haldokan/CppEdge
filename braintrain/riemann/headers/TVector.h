#ifndef BRAINTRAIN_TVECTOR_H
#define BRAINTRAIN_TVECTOR_H

#include <iostream>
using namespace std;

template<typename T>

class TVector {
private:
    int length;
    T *items;
public:
    explicit TVector(int); // no implicit conversion from int to vector

    TVector(std::initializer_list<T>);

    TVector(const TVector&); //copy-cntr

    TVector(TVector&&) noexcept; // move-cntr

    ~TVector();

    TVector &operator=(const TVector&); // copy assignment

    TVector &operator=(TVector&&) noexcept; // move assignment

    // const signifies that this func doesn't modify its object. It can be called by const and non-const objects
    // however non-const funcs cannot be called by const objects (talk about simplicity!)
    [[nodiscard]] int get_size() const;
    // note the way B.S. implemented this is to pass TVector& but it did not work.. same for end (I think he made a mistake)
    T* begin() const;

    T* end() const;

    T &operator[](int); // get the subscript and alter it if you want

    // the left const enables calling the subscript op by const, the right const indicates that the op does not alter this object
    const T &operator[](int) const;
};

// template implementation must be in the header file

template<typename T>
TVector<T>::TVector(int len) {
    cout << "TVector-conversion-cntr is called" << endl;
    if (len < 0) {
        throw length_error("negative vector length: " + to_string(len));
    }
    length = len;
    items = new T[len];
}

template<typename T>
TVector<T>::TVector(initializer_list<T> init): length {static_cast<int>(init.size())}, items{new T[length]} {
    cout << "TVector-initializer-list-cntr is called" << endl;
    copy(init.begin(), init.end(), items);
}

template<typename T>
TVector<T>::TVector(const TVector<T> & v): length {v.length}, items {new T[v.length]} {
    cout << "TVector-copy-cntr is called" << endl;
    for (int i = 0; i != length; ++i) {
        items[i] = v.items[i];
    }
}

template<typename T>
TVector<T>::TVector(TVector<T> &&v) noexcept: length {v.length}, items {v.items} { // move cntr
    cout << "TVector-move-cntr is called" << endl;
    v.length = 0;
    v.items = nullptr;
}

template<typename T>
TVector<T> &TVector<T>::operator=(TVector<T> &&v) noexcept { // move-assignment
    cout << "TVector-move-assign is called" << endl;
    delete [] items;
    length = v.length;
    items = v.items;

    v.length = 0;
    v.items = nullptr;
    return *this;
}

template<typename T>
TVector<T> &TVector<T>::operator=(const TVector<T> &v) { // copy-assignment
    cout << "TVector-copy-assign is called" << endl;
    if (v == this) {
        return *this;
    }
    T* old = items;

    items = new T[v.length];
    length = v.length;
    for (int i = 0; i < v.length; i++) {
        items[i] = v.items[i];
    }
    delete [] old;
    return *this;
}

template<typename T>
TVector<T>::~TVector() {
    cout << "TVector-destructor is called" << endl;
    delete [] items; // plain delete deletes a single object, delete[] deletes the array
}

template<typename T>
int TVector<T>::get_size() const {
    return length;
}

template<typename T>
T* TVector<T>::begin() const {
    return length ? &items[0] : nullptr;
}

template<typename T>
T* TVector<T>::end() const {
    return length ? &items[length] : nullptr;
}

template<typename T>
T &TVector<T>::operator[](int i) {
    return items[i];
}

template<typename T>
const T &TVector<T>::operator[](int i) const {
    return items[i];
}

#endif //BRAINTRAIN_TVECTOR_H
