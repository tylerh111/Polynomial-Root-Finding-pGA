//
// Created by tdh5188 on 4/18/18.
//

#include <cstring>
#include "Polynomial.h"



/**
 * Polynomial file format:
 * - first line is an integer (d) where d is the degree of the polynomial
 * - following lines are d lines of complex numbers (a b)
 *    where a and b are real numbers; a is the real part and b is the complex part
 * @param filename
 * @return
 */
Polynomial::Polynomial(char* filename){
    this->filename.assign("");
    this->filename.assign(filename);
    std::ifstream infile(filename);

    this->degree = -2;
    infile >> this->degree;

    this->coefficients_len = this->degree + 1;
    this->coefficients = new std::complex<double>[this->coefficients_len];

    double real, imag;
    int i = 0;
    while (infile >> real >> imag){
        this->coefficients[i].real(real);
        this->coefficients[i].imag(imag);
        i++;
    }
}

Polynomial::~Polynomial() {
    delete[] coefficients;
}

Polynomial::Polynomial(const Polynomial &other) {
    this->filename = other.filename;
    this->degree = other.degree;
    delete[] this->coefficients;
    this->coefficients_len = other.coefficients_len;
    this->coefficients = new std::complex<double>[this->coefficients_len];
    for (int i = 0; i < this->coefficients_len; i++){
        this->coefficients[i] = other.coefficients[i];
    }
}
Polynomial::Polynomial(const Polynomial &&other) noexcept {
    this->filename = other.filename;
    this->degree = other.degree;
    delete[] this->coefficients;
    this->coefficients_len = other.coefficients_len;
    this->coefficients = new std::complex<double>[this->coefficients_len];
    for (int i = 0; i < this->coefficients_len; i++){
        this->coefficients[i] = other.coefficients[i];
    }
}
Polynomial& Polynomial::operator=(const Polynomial &other) {
    this->filename = other.filename;
    this->degree = other.degree;
    delete[] this->coefficients;
    this->coefficients_len = other.coefficients_len;
    this->coefficients = new std::complex<double>[this->coefficients_len];
    for (int i = 0; i < this->coefficients_len; i++){
        this->coefficients[i] = other.coefficients[i];
    }
}
Polynomial& Polynomial::operator=(Polynomial &&other) noexcept {
    this->filename = other.filename;
    this->degree = other.degree;
    delete[] this->coefficients;
    this->coefficients_len = other.coefficients_len;
    this->coefficients = new std::complex<double>[this->coefficients_len];
    for (int i = 0; i < this->coefficients_len; i++){
        this->coefficients[i] = other.coefficients[i];
    }
}






std::string Polynomial::to_string() const {
    if (filename.empty()) return ""; // throw an exception instead
    std::stringstream ss;
    for(int i = 0; i < coefficients_len; i++){
        bool isRealZero = coefficients[i].real() == 0;
        bool isImagZero = coefficients[i].imag() == 0;
        if (!isRealZero && !isImagZero) ss << "(" << coefficients[i].real() << " + " << coefficients[i].imag() << "i)";
        else if (!isRealZero) ss << coefficients[i].real();
        else if (!isImagZero) ss << coefficients[i].imag();
        else continue;
        if (i + 1 != coefficients_len) ss << "*x^" << degree - i << " + ";
    }
    return ss.str();
}



std::complex<double> Polynomial::operator()(const std::complex<double>& input) const {
    if (filename.empty()) return {0,0}; //throw an exception instead
    auto z = std::complex<double>(0,0);
    for (int i = degree, j = 0; j < coefficients_len; i--, j++)
        z = z + coefficients[j] * std::pow(input, i);

    return z;
}



