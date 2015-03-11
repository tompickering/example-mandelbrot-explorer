
#ifndef GUARD_COMPLEX_HPP
#define GUARD_COMPLEX_HPP

class Complex {
  public:
    Complex();
    Complex(double, double);

    //Complex& operator=(const );
    Complex operator+(const Complex& b);
    Complex operator-(const Complex& b);
    Complex operator*(const Complex& b);
    Complex operator/(const Complex& b);

    double modulus();

    void print();

    double r;
    double i;
};

#endif
