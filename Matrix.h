
#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <iostream>


class Vector;

class Matrix
{
public:
    Matrix() {}
    Matrix(const Matrix& right);
    ~Matrix() {};

    void invert();
    void transpose();
    Matrix getInverse() const;
    Matrix getTranspose() const;

    Matrix& operator+=(const Matrix& right);
    Matrix& operator-=(const Matrix& right);
    Matrix& operator*=(const Matrix& right);
    Matrix& operator*=(double right);

    double determinant();
    double x[4][4];
};

Matrix operator-(const Matrix& left, const Matrix& right);
Matrix operator+(const Matrix& left, const Matrix& right);
Matrix operator*(const Matrix& left, const Matrix& right);
Vector operator*(const Matrix& left, const Vector& right);
Matrix operator*(const Matrix& left, double right);

Vector transformLoc(const Matrix& left, const Vector& right);
Vector transformVec(const Matrix& left, const Vector& right);


Matrix identityMatrix();

Matrix scale(double x, double y, double z);
Matrix rotate(const Vector& axis, double angle);

std::ostream& operator<<(std::ostream& out, const Matrix& right);

Matrix translate(double x, double y, double z);
Matrix rotateX(double angle);
Matrix zeroMatrix();
Matrix rotateY(double angle);
Matrix rotateZ(double angle);

inline double det3(double a, double b, double c, double d, double e, double f, double g, double h, double i)
{
    return a*e*i + d*h*c + g*b*f - g*e*c - d*b*i - a*h*f;
}

#endif 