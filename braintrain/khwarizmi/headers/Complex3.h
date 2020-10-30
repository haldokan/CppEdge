
#ifndef CPP_COMPLEX3_H
#define CPP_COMPLEX3_H

class Complex3 {
private:
    double rl = 0; // default member initializer used when the cntr does not have a corresponding param
    double img = 0;
public:
    Complex3();

    Complex3(double, double);

    Complex3(Complex3&);

    [[nodiscard]] double real() const;

    [[nodiscard]] double imag() const;

    Complex3& operator+=(Complex3 &c);

    Complex3& operator*=(Complex3 c1); // concrete class does not have to implement all header funcs
};

#endif //CPP_COMPLEX3_H
