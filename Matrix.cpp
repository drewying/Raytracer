
#include "Matrix.h"
#include "Vector.h"
#include <math.h>
#include <iomanip>


Matrix::Matrix(const Matrix& right)
{
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            x[i][j] = right.x[i][j];
}

void Matrix::invert()
{
    double det = determinant();
    Matrix inverse;

    inverse.x[0][0] =  det3(x[1][1], x[1][2], x[1][3],
            x[2][1], x[2][2], x[2][3],
            x[3][1], x[3][2], x[3][3]) / det;
    inverse.x[0][1] = -det3(x[0][1], x[0][2], x[0][3],
            x[2][1], x[2][2], x[2][3],
            x[3][1], x[3][2], x[3][3]) / det;
    inverse.x[0][2] =  det3(x[0][1], x[0][2], x[0][3],
            x[1][1], x[1][2], x[1][3],
            x[3][1], x[3][2], x[3][3]) / det;
    inverse.x[0][3] = -det3(x[0][1], x[0][2], x[0][3],
            x[1][1], x[1][2], x[1][3],
            x[2][1], x[2][2], x[2][3]) / det;

    inverse.x[1][0] = -det3(x[1][0], x[1][2], x[1][3],
            x[2][0], x[2][2], x[2][3],
            x[3][0], x[3][2], x[3][3]) / det;
    inverse.x[1][1] =  det3(x[0][0], x[0][2], x[0][3],
            x[2][0], x[2][2], x[2][3],
            x[3][0], x[3][2], x[3][3]) / det;
    inverse.x[1][2] = -det3(x[0][0], x[0][2], x[0][3],
            x[1][0], x[1][2], x[1][3],
            x[3][0], x[3][2], x[3][3]) / det;
    inverse.x[1][3] =  det3(x[0][0], x[0][2], x[0][3],
            x[1][0], x[1][2], x[1][3],
            x[2][0], x[2][2], x[2][3]) / det;

    inverse.x[2][0] =  det3(x[1][0], x[1][1], x[1][3],
            x[2][0], x[2][1], x[2][3],
            x[3][0], x[3][1], x[3][3]) / det;
    inverse.x[2][1] = -det3(x[0][0], x[0][1], x[0][3],
            x[2][0], x[2][1], x[2][3],
            x[3][0], x[3][1], x[3][3]) / det;
    inverse.x[2][2] =  det3(x[0][0], x[0][1], x[0][3],
            x[1][0], x[1][1], x[1][3],
            x[3][0], x[3][1], x[3][3]) / det;
    inverse.x[2][3] = -det3(x[0][0], x[0][1], x[0][3],
            x[1][0], x[1][1], x[1][3],
            x[2][0], x[2][1], x[2][3]) / det;

    inverse.x[3][0] = -det3(x[1][0], x[1][1], x[1][2],
            x[2][0], x[2][1], x[2][2],
            x[3][0], x[3][1], x[3][2]) / det;
    inverse.x[3][1] =  det3(x[0][0], x[0][1], x[0][2],
            x[2][0], x[2][1], x[2][2],
            x[3][0], x[3][1], x[3][2]) / det;
    inverse.x[3][2] = -det3(x[0][0], x[0][1], x[0][2],
            x[1][0], x[1][1], x[1][2],
            x[3][0], x[3][1], x[3][2]) / det;
    inverse.x[3][3] =  det3(x[0][0], x[0][1], x[0][2],
            x[1][0], x[1][1], x[1][2],
            x[2][0], x[2][1], x[2][2]) / det;

    *this = inverse;
}

void Matrix::transpose()
{
    //TODO refactor
    double temp[4][4];
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            //double temp = x[i][j];
            temp[i][j] = x[j][i];
            //x[j][i] = temp;
        }
    }
    for(int i = 0; i < 4; i++)
    {
      for(int j = 0; j < 4; j++)
      {
        //double temp = x[i][j];
        x[i][j] = temp[i][j];
        //x[j][i] = temp;
      }
    }
}

Matrix Matrix::getInverse() const
{
    Matrix ret = *this;
    ret.invert();
    return ret;
}

Matrix Matrix::getTranspose() const
{
    Matrix ret = *this;
    ret.transpose();
    return ret;
}

Matrix& Matrix::operator+=(const Matrix& right)
{
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            x[i][j] += right.x[i][j];
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& right)
{
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            x[i][j] -= right.x[i][j];
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& right)
{
    Matrix ret = *this;

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            double sum = 0;
            for(int k = 0; k < 4; k++)
                sum += ret.x[i][k] * right.x[k][j];
            x[i][j] = sum;
        }
    }

    return *this;
}

Matrix& Matrix::operator*=(double right)
{
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            x[i][j] *= right;
    return *this;
}



Matrix operator+(const Matrix& left, const Matrix& right)
{
    Matrix ret;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            ret.x[i][j] = left.x[i][j] + right.x[i][j];
    return ret;
}

Matrix operator-(const Matrix& left, const Matrix& right)
{
    Matrix ret;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            ret.x[i][j] = left.x[i][j] - right.x[i][j];
    return ret;
}

Matrix operator*(const Matrix& left, const Matrix& right)
{
    Matrix ret;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
        {
            double subt = 0;
            for(int k = 0; k < 4; k++)
                subt += left.x[i][k] * right.x[k][j];
            ret.x[i][j] = subt;
        }

    return ret;
}

Vector operator*(const Matrix& left, const Vector& right)
{

    double temp;
    double x = (right.x() * left.x[0][0] + right.y() * left.x[0][1] + right.z() * left.x[0][2] + left.x[0][3]);
    double y = (right.x() * left.x[1][0] + right.y() * left.x[1][1] + right.z() * left.x[1][2] + left.x[1][3]);
    double z = (right.x() * left.x[2][0] + right.y() * left.x[2][1] + right.z() * left.x[2][2] + left.x[2][3]);
    temp   = right.x() * left.x[3][0] + right.y() * left.x[3][1] + right.z() * left.x[3][2] + left.x[3][3];

    Vector ret = Vector(x,y,z);

    ret /= temp;
    return ret;
}

Matrix operator*(const Matrix& left, double right)
{
    Matrix ret;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            ret.x[i][j] = left.x[i][j] * right;
    return ret;
}

Vector transformLoc(const Matrix& left, const Vector& right)
{
    return left * right;
}

Vector transformVec(const Matrix& left, const Vector& right)
{
    double x = (right.x() * left.x[0][0] + right.y() * left.x[0][1] + right.z() * left.x[0][2]);
    double y = (right.x() * left.x[1][0] + right.y() * left.x[1][1] + right.z() * left.x[1][2]);
    double z = (right.x() * left.x[2][0] + right.y() * left.x[2][1] + right.z() * left.x[2][2]);

    Vector ret(x, y, z);
    return ret;
}


Matrix zeroMatrix()
{
    Matrix ret;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            ret.x[i][j] = 0;
    return ret;
}

Matrix identityMatrix()
{
    Matrix ret;
    ret.x[0][0] = 1.0; ret.x[0][1] = 0.0; ret.x[0][2] = 0.0; ret.x[0][3] = 0.0;
    ret.x[1][0] = 0.0; ret.x[1][1] = 1.0; ret.x[1][2] = 0.0; ret.x[1][3] = 0.0;
    ret.x[2][0] = 0.0; ret.x[2][1] = 0.0;	ret.x[2][2] = 1.0; ret.x[2][3] = 0.0;
    ret.x[3][0] = 0.0; ret.x[3][1] = 0.0;	ret.x[3][2] = 0.0; ret.x[3][3] = 1.0;
    return ret;
}

Matrix translate(double x, double y, double z)
{
    Matrix ret = identityMatrix();
    ret.x[0][3] = x;
    ret.x[1][3] = y;
    ret.x[2][3] = z;
    return ret;
}

Matrix scale(double x, double y, double z)
{
    Matrix ret = zeroMatrix();
    ret.x[0][0] = x;
    ret.x[1][1] = y;
    ret.x[2][2] = z;
    ret.x[3][3] = 1.0;
    return ret;
}

Matrix rotate(const Vector& axis, double angle)
{
    Vector axisC = axis;
    axisC.normalize();
    Matrix ret;
    double x = axisC.x();
    double y = axisC.y();
    double z = axisC.z();
    double cosine = cos(angle);
    double sine = sin(angle);
    double t = 1 - cosine;

    ret.x[0][0] = t * x * x + cosine;
    ret.x[0][1] = t * x * y - sine * y;
    ret.x[0][2] = t * x * z + sine * y;
    ret.x[0][3] = 0.0;

    ret.x[1][0] = t * x * y + sine * z;
    ret.x[1][1] = t * y * y + cosine;
    ret.x[1][2] = t * y * z - sine * x;
    ret.x[1][3] = 0.0;

    ret.x[2][0] = t * x * z - sine * y;
    ret.x[2][1] = t * y * z + sine * x;
    ret.x[2][2] = t * z * z + cosine;
    ret.x[2][3] = 0.0;

    ret.x[3][0] = 0.0;
    ret.x[3][1] = 0.0;
    ret.x[3][2] = 0.0;
    ret.x[3][3] = 1.0;

    return ret;
}

Matrix rotateX(double angle)
{
    Matrix ret = identityMatrix();
    double cosine = cos(angle);
    double sine = sin(angle);

    ret.x[1][1] = cosine;
    ret.x[1][2] = -sine;
    ret.x[2][1] = sine;
    ret.x[2][2] = cosine;

    return ret;
}

Matrix rotateY(double angle)
{
    Matrix ret = identityMatrix();
    double cosine = cos(angle);
    double sine = sin(angle);

    ret.x[0][0] = cosine;
    ret.x[0][2] = sine;
    ret.x[2][0] = -sine;
    ret.x[2][2] = cosine;

    return ret;
}

Matrix rotateZ(double angle)
{
    Matrix ret = identityMatrix();
    double cosine = cos(angle);
    double sine = sin(angle);

    ret.x[0][0] = cosine;
    ret.x[0][1] = -sine;
    ret.x[1][0] = sine;
    ret.x[1][1] = cosine;

    return ret;
}

std::ostream& operator<<(std::ostream& out, const Matrix& right)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
            out << std::setprecision(3) << std::setw(10) << right.x[i][j];
        out << std::endl;
    }
    return out;
}

double Matrix::determinant()
{
    double det;
    det = x[0][0] * det3(x[1][1], x[1][2], x[1][3],
            x[2][1], x[2][2], x[2][3],
            x[3][1], x[3][2], x[3][3]);
    det -= x[0][1] * det3(x[1][0], x[1][2], x[1][3],
            x[2][0], x[2][2], x[2][3],
            x[3][0], x[3][2], x[3][3]);
    det += x[0][2] * det3(x[1][0], x[1][1], x[1][3],
            x[2][0], x[2][1], x[2][3],
            x[3][0], x[3][1], x[3][3]);
    det -= x[0][3] * det3(x[1][0], x[1][1], x[1][2],
            x[2][0], x[2][1], x[2][2],
            x[3][0], x[3][1], x[3][2]);

    return det;
}