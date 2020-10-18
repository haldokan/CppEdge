#include "headers/TVector.h"

#include <iostream>

using namespace std;

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
TVector<T>::TVector(TVector<T> &&v): length {v.length}, items {v.items} { // move cntr
    cout << "TVector-move-cntr is called" << endl;
    v.length = 0;
    v.items = nullptr;
}

template<typename T>
TVector<T> &TVector<T>::operator=(TVector<T> &&v) { // move-assignment
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
T &TVector<T>::operator[](int i) {
    return items[i];
}

template<typename T>
const T &TVector<T>::operator[](int i) const {
    return items[i];
}
