#pragma once

#include <array>
#include <iostream>
#include <cassert>
#include "Vectors/Vector3.h"

namespace Math
{
	class Matrix3
	{
	private:
		std::array<std::array<float, 3>, 3> m_Values;

	public:
		Matrix3() = default;

		/// <summary> Creates a matrix of size 3 </summary>
		/// <param name="_values"> : Values you want in the matrix </param>
		/// <returns></returns>
		Matrix3(const std::array<float, 9> _values);

		/// <summary> Creates a matrix of size 3 with the same values </summary>
		/// <param name="_value"> : Value you want in your matrix </param>
		/// <returns></returns>
		Matrix3(const float _value);

		/// <summary> Creates a matrix of size 3 with 3 vectors of size 3 </summary>
		/// <param name="_vector1"> : First vector on row 0 </param>
		/// <param name="_vector2"> : Second vector on row 1 </param>
		/// <param name="_vector3"> : Third vector on row 2 </param>
		/// <returns></returns>
		Matrix3(Vector3& _vector1, Vector3& _vector2, Vector3& _vector3);

		/// <summary> Create a matrix by copy </summary>
		/// <param name="_matrix"> : Matrix you want to copy </param>
		Matrix3(const Matrix3& _matrix);

		~Matrix3() = default;

		///////////////////////////////////////////////////////////////////////

		/// PREDIFINED MATRICES

		///////////////////////////////////////////////////////////////////////

		static const Matrix3 identity;
		static const Matrix3 zero;
		static const Matrix3 one;

		///////////////////////////////////////////////////////////////////////

		/// FUNCTIONS

		///////////////////////////////////////////////////////////////////////

		/// <summary> Prints the matrix </summary>
		/// <returns></returns>
		void Print() const;

		/// <summary> Returns the opposite of the matrix </summary>
		/// <returns></returns>
		Matrix3 Opposite();

		/// <summary> Returns an identity matrix </summary>
		/// <returns></returns>
		Matrix3 Identity();

		/// <summary> Returns the addition of two matrices </summary>
		/// <param name="_matrix2"> : Second matrix you want to add </param>
		/// <returns></returns>
		Matrix3 Add(const Matrix3& _matrix2);

		/// <summary> Returns the addition of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Matrix3 Add(const float _scalar);

		/// <summary> Returns the product of two matrices </summary>
		/// <param name="_matrix2"> : Second matrix you want to multiply by </param>
		/// <returns></returns>
		Matrix3 Multiply(const Matrix3& _matrix2);

		/// <summary> Returns the product of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to multiply by </param>
		/// <returns></returns>
		Matrix3 Multiply(const float _scalar);

		/// <summary> Returns the product of the matrix with a vector </summary>
		/// <param name="_vector"> : Vector you want to multiply by </param>
		/// <returns></returns>
		Vector3 Multiply(Vector3 _vector);

		/// <summary> Returns the division of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to divide by </param>
		/// <returns></returns>
		Matrix3 Divide(float _scalar);

		/// <summary> Returns in a vector the diagonal of the matrix </summary>
		/// <returns></returns>
		[[nodiscard]] const Vector3 Diagonal() const;

		/// <summary> Returns the transpose of the matrix </summary>
		/// <returns></returns>
		Matrix3 Transpose();

		/// <summary> Returns the trace of the matrix </summary>
		/// <returns></returns>
		[[nodiscard]] const float Trace() const;

		/// <summary> Returns the determinant of the matrix </summary>
		/// <returns></returns>
		[[nodiscard]] const float Determinant() const;

		/// <summary> Returns the inverse of the matrix if possible </summary>
		/// <returns></returns>
		Matrix3 Inverse();

		/// <summary> Returns the X rotation matrix </summary>
		/// <param name="_angle"> : Angle you want in the matrix </param>
		/// <returns></returns>
		Matrix3 XRotation(const float _angle);

		/// <summary> Returns the Y rotation matrix </summary>
		/// <param name="_angle"> : Angle you want in the matrix </param>
		/// <returns></returns>
		Matrix3 YRotation(const float _angle);

		/// <summary> Returns the Z rotation matrix </summary>
		/// <param name="_angle"> : Angle you want in the matrix </param>
		/// <returns></returns>
		Matrix3 ZRotation(const float _angle);

		/// <summary> Returns the rotation matrix arround an axis </summary>
		/// <param name="_axis"> : Axis you want to rotate arround </param>
		/// <param name="_angle"> : Angle you want in the matrix </param>
		/// <returns></returns>
		Matrix3 RotationAxix(Vector3& _axis, const float _angle);

		///////////////////////////////////////////////////////////////////////

		/// OPERATOR OVERLOADING

		///////////////////////////////////////////////////////////////////////

		/// <summary> Overload : Returns the addition of two matrices </summary>
		/// <param name="_matrix2"> : Second matrix you want to add </param>
		/// <returns></returns>
		Matrix3 operator+(const Matrix3& _matrix2);

		/// <summary> Overload : Returns the addition of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Matrix3 operator+(const float _scalar);

		/// <summary> Overload : Adds two matrices together </summary>
		/// <param name="_matrix2"> : Second matrix you want to add </param>
		/// <returns></returns>
		Matrix3& operator+=(const Matrix3& _matrix2);

		/// <summary> Overload : Adds a scalar to the matrix </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Matrix3& operator+=(float _scalar);

		/// <summary> Overload : Returns the substraction of two matrices </summary>
		/// <param name="_matrix2"> : Second matrix you want to substract </param>
		/// <returns></returns>
		Matrix3 operator-(Matrix3& _matrix2);

		/// <summary> Overload : Returns the substraction of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to substract </param>
		/// <returns></returns>
		Matrix3 operator-(const float _scalar);

		/// <summary> Overload : Substracts twi matrices together </summary>
		/// <param name="_matrix2"> : Second matrix you want to substract </param>
		/// <returns></returns>
		Matrix3& operator-=(Matrix3& _matrix2);

		/// <summary> Overload : Substracts a scalar to the matrix </summary>
		/// <param name="_scalar"> : Scalar you want to substract </param>
		/// <returns></returns>
		Matrix3& operator-=(const float _scalar);

		/// <summary> Overload : Returns the product of two matrices </summary>
		/// <param name="_matrix2"> : Second matrix you want to multiply by </param>
		/// <returns></returns>
		Matrix3 operator*(const Matrix3& _matrix2);

		/// <summary> Overload : Returns the product of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to multiply by </param>
		/// <returns></returns>
		Matrix3 operator*(const float _scalar);

		/// <summary> Overload : Returns the product of the matrix with a vector </summary>
		/// <param name="_vector"> : Vector you want to multiply with the matrix </param>
		/// <returns></returns>
		Vector3 operator*(const Vector3& _vector);

		/// <summary> Overload : Multiplies two matrices together </summary>
		/// <param name="_matrix2"> : Second matrix you want to multiply by </param>
		/// <returns></returns>
		Matrix3& operator*=(const Matrix3& _matrix2);

		/// <summary> Overload : Multiplies a scalar to the matrix </summary>
		/// <param name="_scalar"> : Scalar you want to multiply the matrix by </param>
		/// <returns></returns>
		Matrix3& operator*=(const float _scalar);

		/// <summary> Overload : Returns the division of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to divide the matrix by </param>
		/// <returns></returns>
		Matrix3 operator/(const float _scalar);

		/// <summary> Overload : Divides the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to divide the matrix by </param>
		/// <returns></returns>
		Matrix3& operator/=(const float _scalar);

		/// <summary> Overload : Returns the row of the matrix in a vector </summary>
		/// <param name="_index"> : Index of the row you want </param>
		/// <returns></returns>
		Vector3 operator[](const int _index);
	};
}