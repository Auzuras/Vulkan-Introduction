#pragma once

#include <array>
#include <iostream>
#include <cassert>
#include "Vectors/Vector2.h"

namespace Math
{
	class Matrix2
	{
	private:
		std::array<std::array<float, 2>, 2> m_Values;

	public:
		Matrix2() = default;

		/// <summary> Creates a matrix of size 2 </summary>
		/// <param name="_values"> : Values you want in the matrix </param>
		/// <returns></returns>
		Matrix2(const std::array<float, 4> _values);

		/// <summary> Creates a matrix of size 2 with the same values </summary>
		/// <param name="_value"> : Value you want in your matrix </param>
		/// <returns></returns>
		Matrix2(const float _value);

		/// <summary> Creates a matrix of size 2 with 2 vectors of size 2 </summary>
		/// <param name="_vector1"> : First vector on row 0 </param>
		/// <param name="_vector2"> : Second vector on row 1 </param>
		/// <returns></returns>
		Matrix2(Vector2& _vector1, Vector2& _vector2);

		/// <summary> Creates a matrix by copy </summary>
		/// <param name="_matrix"> : Matrix you want to copy </param>
		Matrix2(const Matrix2& _matrix);

		~Matrix2() = default;

		///////////////////////////////////////////////////////////////////////

		/// PREDIFINED MATRICES

		///////////////////////////////////////////////////////////////////////

		static const Matrix2 identity;
		static const Matrix2 zero;
		static const Matrix2 one;

		///////////////////////////////////////////////////////////////////////

		/// FUNCTIONS

		///////////////////////////////////////////////////////////////////////

		/// <summary> Prints the matrix </summary>
		/// <returns></returns>
		void Print() const;

		/// <summary> Returns the opposite of the matrix </summary>
		/// <returns></returns>
		Matrix2 Opposite();

		/// <summary> Returns an identity matrix </summary>
		/// <returns></returns>
		Matrix2 Identity();

		/// <summary> Returns the addition of two matrices </summary>
		/// <param name="_matrix2"> : Second matrix you want to add </param>
		/// <returns></returns>
		Matrix2 Add(const Matrix2& _matrix2);

		/// <summary> Returns the addition of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Matrix2 Add(const float _scalar);

		/// <summary> Returns the product of two matrices </summary>
		/// <param name="_matrix2"> : Second matrix you want to multiply by </param>
		/// <returns></returns>
		Matrix2 Multiply(const Matrix2& _matrix2);

		/// <summary> Returns the product of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to multiply by </param>
		/// <returns></returns>
		Matrix2 Multiply(const float _scalar);

		/// <summary> Returns the product of the matrix with a vector </summary>
		/// <param name="_vector"> : Vector you want to multiply by </param>
		/// <returns></returns>
		Vector2 Multiply(Vector2 _vector);

		/// <summary> Returns the division of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to divide by </param>
		/// <returns></returns>
		Matrix2 Divide(float _scalar);

		/// <summary> Returns in a vector the diagonal of the matrix </summary>
		/// <returns></returns>
		[[nodiscard]] const Vector2 Diagonal() const;

		/// <summary> Returns the transpose of the matrix </summary>
		/// <returns></returns>
		Matrix2 Transpose();

		/// <summary> Returns the trace of the matrix </summary>
		/// <returns></returns>
		[[nodiscard]] const float Trace() const;

		/// <summary> Returns the determinant of the matrix </summary>
		/// <returns></returns>
		[[nodiscard]] const float Determinant() const;

		/// <summary> Returns the inverse of the matrix if possible </summary>
		/// <returns></returns>
		Matrix2 Inverse();

		/// <summary> Return the rotation matrix with an angle </summary>
		/// <param name="_angle"> : Angle you want in the matrix </param>
		/// <returns></returns>
		Matrix2 Rotation(const float _angle);

		///////////////////////////////////////////////////////////////////////

		/// OPERATOR OVERLOADING

		///////////////////////////////////////////////////////////////////////

		/// <summary> Overload : Returns the addition of two matrices </summary>
		/// <param name="_matrix2"> : Second matrix you want to add </param>
		/// <returns></returns>
		Matrix2 operator+(const Matrix2& _matrix2);

		/// <summary> Overload : Returns the addition of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Matrix2 operator+(const float _scalar);

		/// <summary> Overload : Adds two matrices together </summary>
		/// <param name="_matrix2"> : Second matrix you want to add </param>
		/// <returns></returns>
		Matrix2& operator+=(const Matrix2& _matrix2);

		/// <summary> Overload : Adds a scalar to the matrix </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Matrix2& operator+=(float _scalar);

		/// <summary> Overload : Returns the substraction of two matrices </summary>
		/// <param name="_matrix2"> : Second matrix you want to substract </param>
		/// <returns></returns>
		Matrix2 operator-(Matrix2& _matrix2);

		/// <summary> Overload : Returns the substraction of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to substract </param>
		/// <returns></returns>
		Matrix2 operator-(const float _scalar);

		/// <summary> Overload : Substracts twi matrices together </summary>
		/// <param name="_matrix2"> : Second matrix you want to substract </param>
		/// <returns></returns>
		Matrix2& operator-=(Matrix2& _matrix2);

		/// <summary> Overload : Substracts a scalar to the matrix </summary>
		/// <param name="_scalar"> : Scalar you want to substract </param>
		/// <returns></returns>
		Matrix2& operator-=(const float _scalar);

		/// <summary> Overload : Returns the product of two matrices </summary>
		/// <param name="_matrix2"> : Second matrix you want to multiply by </param>
		/// <returns></returns>
		Matrix2 operator*(const Matrix2& _matrix2);

		/// <summary> Overload : Returns the product of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to multiply by </param>
		/// <returns></returns>
		Matrix2 operator*(const float _scalar);

		/// <summary> Overload : Returns the product of the matrix with a vector </summary>
		/// <param name="_vector"> : Vector you want to multiply with the matrix </param>
		/// <returns></returns>
		Vector2 operator*(const Vector2& _vector);

		/// <summary> Overload : Multiplies two matrices together </summary>
		/// <param name="_matrix2"> : Second matrix you want to multiply by </param>
		/// <returns></returns>
		Matrix2& operator*=(const Matrix2& _matrix2);

		/// <summary> Overload : Multiplies a scalar to the matrix </summary>
		/// <param name="_scalar"> : Scalar you want to multiply the matrix by </param>
		/// <returns></returns>
		Matrix2& operator*=(const float _scalar);

		/// <summary> Overload : Returns the division of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to divide the matrix by </param>
		/// <returns></returns>
		Matrix2 operator/(const float _scalar);

		/// <summary> Overload : Divides the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to divide the matrix by </param>
		/// <returns></returns>
		Matrix2& operator/=(const float _scalar);

		/// <summary> Overload : Returns the row of the matrix in a vector </summary>
		/// <param name="_index"> : Index of the row you want </param>
		/// <returns></returns>
		Vector2 operator[](const int _index);
	};
}