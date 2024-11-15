#pragma once

#include <array>
#include <iostream>
#include "Vectors/Vector4.h"
#include "Vectors/Vector3.h"

namespace Math
{
	class Matrix4
	{
	private:
		std::array<std::array<float, 4>, 4> m_Values;

	public:
		Matrix4() = default;

		/// <summary> Creates a matrix of size 4 </summary>
		/// <param name="_values"> : Values you want in the matrix </param>
		/// <returns></returns>
		Matrix4(const std::array<float, 16> _values);

		/// <summary> Creates a matrix of size 4 with the same values </summary>
		/// <param name="_value"> : Value you want in your matrix </param>
		/// <returns></returns>
		Matrix4(const float _value);

		/// <summary> Creates a matrix of size 4 with 4 vectors of size 4 </summary>
		/// <param name="_vector1"> : First vector on row 0 </param>
		/// <param name="_vector2"> : Second vector on row 1 </param>
		/// <param name="_vector3"> : Third vector on row 2 </param>
		/// <param name="_vector4"> : Fourth vector on row 3 </param>
		/// <returns></returns>
		Matrix4(Vector4& _vector1, Vector4& _vector2, Vector4& _vector3, Vector4& _vector4);

		/// <summary> Creates a matrix by copy </summary>
		/// <param name="_matrix"> : Matrix you want to copy </param>
		Matrix4(const Matrix4& _matrix);

		~Matrix4() = default;

		///////////////////////////////////////////////////////////////////////

		/// PREDIFINED MATRICES

		///////////////////////////////////////////////////////////////////////

		static const Matrix4 identity;
		static const Matrix4 zero;
		static const Matrix4 one;

		///////////////////////////////////////////////////////////////////////

		/// FUNCTIONS

		///////////////////////////////////////////////////////////////////////

		/// <summary> Prints the matrix </summary>
		/// <returns></returns>
		void Print() const;

		/// <summary> Returns the opposite of the matrix </summary>
		/// <returns></returns>
		Matrix4 Opposite();

		/// <summary> Returns an identity matrix </summary>
		/// <returns></returns>
		Matrix4 Identity();

		/// <summary> Returns the addition of two matrices </summary>
		/// <param name="_matrix2"> : Second matrix you want to add </param>
		/// <returns></returns>
		Matrix4 Add(const Matrix4& _matrix2);

		/// <summary> Returns the addition of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Matrix4 Add(const float _scalar);

		/// <summary> Returns the product of two matrices </summary>
		/// <param name="_matrix2"> : Second matrix you want to multiply by </param>
		/// <returns></returns>
		Matrix4 Multiply(const Matrix4& _matrix2);

		/// <summary> Returns the product of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to multiply by </param>
		/// <returns></returns>
		Matrix4 Multiply(const float _scalar);

		/// <summary> Returns the product of the matrix with a vector </summary>
		/// <param name="_vector"> : Vector you want to multiply by </param>
		/// <returns></returns>
		Vector4 Multiply(Vector4 _vector);

		/// <summary> Returns the division of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to divide by </param>
		/// <returns></returns>
		Matrix4 Divide(float _scalar);

		/// <summary> Returns in a vector the diagonal of the matrix </summary>
		/// <returns></returns>
		[[nodiscard]] const Vector4 Diagonal() const;
		/// <summary> Returns the transpose of the matrix </summary>
		/// <returns></returns>
		Matrix4 Transpose();

		/// <summary> Returns the trace of the matrix </summary>
		/// <returns></returns>
		[[nodiscard]] const float Trace() const;

		/// <summary> Returns the determinant of the matrix </summary>
		/// <returns></returns>
		[[nodiscard]] const float Determinant() const;

		/// <summary> Returns the inverse of the matrix if possible </summary>
		/// <returns></returns>
		Matrix4 Inverse();

		/// <summary> Returns the TRS matrix </summary>
		/// <param name="_xTranslation"> : X parameter for translation </param>
		/// <param name="_yTranslation"> : Y parameter for translation </param>
		/// <param name="_zTranslation"> : Z parameter for translation </param>
		/// <param name="_xAngle"> : X parameter for rotation </param>
		/// <param name="_yAngle"> : Y parameter for rotation </param>
		/// <param name="_zAngle"> : Z parameter for rotation </param>
		/// <param name="_xScale"> : X parameter for scaling </param>
		/// <param name="_yScale"> : Y parameter for scaling </param>
		/// <param name="_zScale"> : Z parameter for scaling </param>
		/// <returns></returns>
		static Matrix4 TRS(const float _xTranslation, const float _yTranslation, const float _zTranslation, const float _xAngle, const float _yAngle, const float _zAngle, const float _xScale, const float _yScale, const float _zScale);

		/// <summary> Returns the TRS matrix </summary>
		/// <param name="_translation"> : Vector3 for translation </param>
		/// <param name="_rotation"> : Vector3 for rotation (represent the rotation on the angle x, y, z) </param>
		/// <param name="_scaling"> : Vector3 for scaling </param>
		/// <returns></returns>
		static Matrix4 TRS(Vector3 _translation, Vector3 _rotation, Vector3 _scaling);

		/// <summary> Returns the translation matrix </summary>
		/// <param name="_xScale"> : X parameter for translation </param>
		/// <param name="_yScale"> : Y parameter for translation </param>
		/// <param name="_zScale"> : Z parameter for translation </param>
		/// <returns></returns>
		static Matrix4 Translate(const float _xTranslation, const float _yTranslation, const float _zTranslation);

		/// <summary> Returns the global rotation matrix </summary>
		/// <param name="_yScale"> : Y parameter for rotation </param>
		/// <param name="_zScale"> : Z parameter for rotation </param>
		/// <param name="_xScale"> : X parameter for rotation </param>
		/// <returns></returns>
		static Matrix4 GlobalRotation(const float _xAngle, const float _yAngle, const float _zAngle);

		/// <summary> Returns the X rotation matrix </summary>
		/// <param name="_angle"> : Angle you want in the matrix </param>
		/// <returns></returns>
		static Matrix4 XRotation(const float _angle);

		/// <summary> Returns the Y rotation matrix </summary>
		/// <param name="_angle"> : Angle you want in the matrix </param>
		/// <returns></returns>
		static Matrix4 YRotation(const float _angle);

		/// <summary> Returns the Z rotation </summary>
		/// <param name="_angle"> : Angle you want in the matrix </param>
		/// <returns></returns>
		static Matrix4 ZRotation(const float _angle);

		/// <summary> Returns the scale matrix </summary>
		/// <param name="_xScale"> : X parameter for scaling </param>
		/// <param name="_yScale"> : Y parameter for scaling </param>
		/// <param name="_zScale"> : Z parameter for scaling </param>
		/// <returns></returns>
		static Matrix4 Scale(const float _xScale, const float _yScale, const float _zScale);

		/// <summary>
		/// Calculates the view matrix
		/// </summary>
		/// <param name="_eye"> : position of the camera </param>
		/// <param name="_lookAt"> : </param>
		/// <returns></returns>
		static Matrix4 ViewMatrix(Vector3 _eye, Vector3 _lookAt, Vector3 _up);

		/// <summary>
		/// Calculates the projection matrix
		/// </summary>
		/// <param name="_near"> : near plane </param>
		/// <param name="_far"> : far plane </param>
		/// <param name="_aspect"> : aspect ratio </param>
		/// <param name="_fov"> : field of view </param>
		/// <param name="_isOrthographic"> : choose between orthographic or perspective projection </param>
		/// <returns></returns>
		static Matrix4 ProjectionPerspectiveMatrix(const float _near, const float _far, const float _aspect, const float _fov);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_near"></param>
		/// <param name="_far"></param>
		/// <param name="_top"></param>
		/// <param name="_bottom"></param>
		/// <param name="_right"></param>
		/// <param name="_left"></param>
		/// <returns></returns>
		static Matrix4 ProjectionOrthographicMatrix(const float _near, const float _far, const float _top, const float _bottom, const float _right, const float _left);

		/// <summary>
		/// Returns the pointer of the first element from the matrix
		/// </summary>
		/// <returns></returns>
		float* Value();


		///////////////////////////////////////////////////////////////////////

		/// OPERATOR OVERLOADING

		///////////////////////////////////////////////////////////////////////

		/// <summary> Overload : Returns the addition of two matrices </summary>
		/// <param name="_matrix2"> : Second matrix you want to add </param>
		/// <returns></returns>
		Matrix4 operator+(const Matrix4& _matrix2);

		/// <summary> Overload : Returns the addition of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Matrix4 operator+(const float _scalar);

		/// <summary> Overload : Adds two matrices together </summary>
		/// <param name="_matrix2"> : Second matrix you want to add </param>
		/// <returns></returns>
		Matrix4& operator+=(const Matrix4& _matrix2);

		/// <summary> Overload : Adds a scalar to the matrix </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Matrix4& operator+=(float _scalar);

		/// <summary> Overload : Returns the substraction of two matrices </summary>
		/// <param name="_matrix2"> : Second matrix you want to substract </param>
		/// <returns></returns>
		Matrix4 operator-(Matrix4& _matrix2);

		/// <summary> Overload : Returns the substraction of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to substract </param>
		/// <returns></returns>
		Matrix4 operator-(const float _scalar);

		/// <summary> Overload : Substracts twi matrices together </summary>
		/// <param name="_matrix2"> : Second matrix you want to substract </param>
		/// <returns></returns>
		Matrix4& operator-=(Matrix4& _matrix2);

		/// <summary> Overload : Substracts a scalar to the matrix </summary>
		/// <param name="_scalar"> : Scalar you want to substract </param>
		/// <returns></returns>
		Matrix4& operator-=(const float _scalar);

		/// <summary> Overload : Returns the product of two matrices </summary>
		/// <param name="_matrix2"> : Second matrix you want to multiply by </param>
		/// <returns></returns>
		Matrix4 operator*(const Matrix4& _matrix2);

		/// <summary> Overload : Returns the product of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to multiply by </param>
		/// <returns></returns>
		Matrix4 operator*(const float _scalar);

		/// <summary> Overload : Returns the product of the matrix with a vector </summary>
		/// <param name="_vector"> : Vector you want to multiply with the matrix </param>
		/// <returns></returns>
		Vector4 operator*(const Vector4& _vector);

		/// <summary> Overload : Multiplies two matrices together </summary>
		/// <param name="_matrix2"> : Second matrix you want to multiply by </param>
		/// <returns></returns>
		Matrix4& operator*=(const Matrix4& _matrix2);

		/// <summary> Overload : Multiplies a scalar to the matrix </summary>
		/// <param name="_scalar"> : Scalar you want to multiply the matrix by </param>
		/// <returns></returns>
		Matrix4& operator*=(const float _scalar);

		/// <summary> Overload : Returns the division of the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to divide the matrix by </param>
		/// <returns></returns>
		Matrix4 operator/(const float _scalar);

		/// <summary> Overload : Divides the matrix with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to divide the matrix by </param>
		/// <returns></returns>
		Matrix4& operator/=(const float _scalar);

		/// <summary> Overload : Returns the row of the matrix in a vector </summary>
		/// <param name="_index"> : Index of the row you want </param>
		/// <returns></returns>
		Vector4 operator[](const int _index);
	};
}