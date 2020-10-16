
#ifndef CPP_COMPLEX3_H
#define CPP_COMPLEX3_H

class Complex3 {
private:
    double rl;
    double img;
public:
    Complex3();

    Complex3(double, double);

    [[nodiscard]] double real() const;

    [[nodiscard]] double imag() const;

    Complex3& operator+=(Complex3 c);

    Complex3& operator*=(Complex3 c1); // concrete class does not have to implement all header funcs
};

#endif //CPP_COMPLEX3_H
