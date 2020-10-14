
#ifndef CPP_COMPLEX_H
#define CPP_COMPLEX_H

class Complex {
private:
    double rl;
    double img;
public:
    Complex();

    Complex(double, double);

    [[nodiscard]] double real() const;

    [[nodiscard]] double imag() const;

    Complex& operator+=(Complex c);

    Complex& operator*=(Complex c1); // concrete class does not have to implement all header funcs
};

#endif //CPP_COMPLEX_H
