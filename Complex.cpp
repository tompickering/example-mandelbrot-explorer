#include "Complex.hpp"

#include <iostream>
#include <cmath>

using std::cout;
using std::endl;

Complex::Complex() {
  r = 0.;
  i = 0.;
}

Complex::Complex(double ir, double ii) {
  r = ir;
  i = ii;
}

double Complex::modulus() {
  return sqrt(r * r + i * i);
}

void Complex::print() {
  cout << r << " + " << i << "i" << endl;
}

Complex Complex::operator+(const Complex& b) {
  double rr = r + b.r;
  double ri = i + b.i;
  return Complex(rr, ri);
}


Complex Complex::operator-(const Complex& b) {
  double rr = r - b.r;
  double ri = i - b.i;
  return Complex(rr, ri);

}

Complex Complex::operator*(const Complex& b) {
  double rr = (r * b.r) - (i * b.i);
  double ri = (i * b.r) + (r * b.i);
  return Complex(rr, ri);
}

Complex Complex::operator/(const Complex& b) {
}
