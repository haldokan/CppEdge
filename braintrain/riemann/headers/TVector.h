#ifndef BRAINTRAIN_TVECTOR_H
#define BRAINTRAIN_TVECTOR_H

#include <initializer_list>

template<typename T>

class TVector {
private:
    int length;
    T *items;
public:
    explicit TVector(int); // no implicit conversion from int to vector

    TVector(std::initializer_list<T>);

    TVector(const TVector&); //copy-cntr

    TVector(TVector&&); // move-cntr

    ~TVector();

    TVector &operator=(const TVector&); // copy assignment

    TVector &operator=(TVector&&); // move assignment

    // const signifies that this func doesn't modify its object. It can be called by const and non-const objects
    // however non-const funcs cannot be called by const objects (talk about simplicity!)
    int get_size() const;

    T &operator[](int); // get the subscript and alter it if you want

    // the left const enables calling the subscript op by const, the right const indicates that the op does not alter this object
    const T &operator[](int) const;
};


#endif //BRAINTRAIN_TVECTOR_H
