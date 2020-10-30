#ifndef BRAINTRAIN_VECTOR3_H
#define BRAINTRAIN_VECTOR3_H

#include <initializer_list>

class Vector3 {

private:
    int length;
    double *items;
public:
    explicit Vector3(int); // no implicit conversion from int to vector

    Vector3(std::initializer_list<double>);

    Vector3(const Vector3&); //copy-cntr

    Vector3(Vector3&&) noexcept ; // move-cntr

    ~Vector3();

    Vector3 &operator=(const Vector3&); // copy assignment

    Vector3 &operator=(Vector3&&) noexcept ; // move assignment

    // const signifies that this func doesn't modify its object. It can be called by const and non-const objects
    // however non-const funcs cannot be called by const objects (talk about simplicity!)
    [[nodiscard]] int get_size() const;

    double &operator[](int); // get the subscript and alter it if you want

    // the left const enables calling the subscript op by const, the right const indicates that the op does not alter this object
    const double &operator[](int) const;
};


#endif //BRAINTRAIN_VECTOR3_H
