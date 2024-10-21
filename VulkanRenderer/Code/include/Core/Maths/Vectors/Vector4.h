#pragma once

#include <iostream>
#include <cassert>

namespace Math
{
	class Vector4
	{
	private:
		float m_X;
		float m_Y;
		float m_Z;
		float m_W;

	public:
		Vector4() = default;

		/// <summary> Creates a vector4 </summary>
		/// <param name="_x"> : x parameter</param>
		/// <param name="_y"> : y parameter</param>
		/// <param name="_y"> : z parameter</param>
		/// <param name="_y"> : w parameter</param>
		Vector4(const float _x, const float _y, const float _z, const float _w);

		/// <summary> Creates a vector of size 4 with the same values </summary>
		/// <param name="_nbr"> : Value you want in the vector </param>
		Vector4(const float _nbr);

		/// <summary> Creates a vector by copy </summary>
		/// <param name="_matrix"> : Vector you want to copy </param>
		/// <returns></returns>
		Vector4(const Vector4& _vector);

		~Vector4() = default;

		///////////////////////////////////////////////////////////////////////

		/// PREDIFINED VECTORS

		///////////////////////////////////////////////////////////////////////

		static const Vector4 zero;
		static const Vector4 one;

		///////////////////////////////////////////////////////////////////////

		/// FUNCTIONS

		///////////////////////////////////////////////////////////////////////

		/// <summary> Prints the vector </summary>
		/// <returns></returns>
		void Print() const;

		/// <summary> Returns the opposite of the vector </summary>
		/// <returns></returns>
		Vector4 Opposite();

		/// <summary> Returns the normalized vector </summary>
		/// <returns></returns>
		Vector4 Normalize();

		/// <summary> Returns the addition of two vectors </summary>
		/// <param name="_vector2"> : Second vector you want to add </param>
		/// <returns></returns>
		Vector4 Add(const Vector4& _vector2);

		/// <summary> Returns the addition of the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Vector4 Add(const float _scalar);

		/// <summary> Returns the product of two vectors </summary>
		/// <param name="_vector2"> : Second vector you want to multiply by </param>
		/// <returns></returns>
		Vector4 Multiply(const Vector4& _vector2);

		/// <summary> Returns the product of the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to multiply by </param>
		/// <returns></returns>
		Vector4 Multiply(const float _scalar);

		/// <summary> Returns the division of a vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to divide by </param>
		/// <returns></returns>
		Vector4 Divide(float _scalar);

		/// <summary> Returns the dot product of two vectors </summary>
		/// <param name="_vector2"> : Second vector for dot product </param>
		/// <returns></returns>
		[[nodiscard]] const float DotProduct(const Vector4& _vector2) const;

		/// <summary> Returns the distance between two vectors </summary>
		/// <param name="_vector2"> : Second vector for distance </param>
		/// <returns></returns>
		[[nodiscard]] const float Distance(const Vector4& _vector2) const;

		/// <summary> Returns the squared morm of a vector </summary>
		/// <returns></returns>
		[[nodiscard]] const float SquaredNorm() const;

		/// <summary> Returns the norm of a vector </summary>
		/// <returns></returns>
		[[nodiscard]] const float Norm() const;

		/// <summary> Returns the midpoint of two vectors </summary>
		/// <param name="_vector2"> : Second vector for midpoint </param>
		/// <returns></returns>
		[[nodiscard]] const Vector4 MidPoint(const Vector4& _vector2) const;
		
		//////////////////////////////////////////////////////////////////////

		/// OPERATOR OVERLOADING

		///////////////////////////////////////////////////////////////////////

		/// <summary> Overload : Returns the addition of two vectors </summary>
		/// <param name="_matrix2"></param>
		/// <returns></returns>
		Vector4 operator+(Vector4& _vector2);

		/// <summary> Overload : Returns the addition of the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Vector4 operator+(float _scalar);

		//// <summary> Overload : Adds two vectors together </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Vector4& operator+=(Vector4& _vector2);

		//// <summary> Overload : Adds a scalar to the vector </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Vector4& operator+=(float _scalar);

		//// <summary> Overload : Returns the substraction of two vectors </summary>
		/// <param name="_scalar"> : Second vector you want to substract </param>
		/// <returns></returns>
		Vector4 operator-(Vector4& _vector2);

		//// <summary> Overload : Returns the substraction of the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to substract </param>
		/// <returns></returns>
		Vector4 operator-(float _scalar);

		//// <summary> Overload : Substracts two vectors together </summary>
		/// <param name="_scalar"> : Second vector you want to substract </param>
		/// <returns></returns>
		Vector4& operator-=(Vector4& _vector2);

		//// <summary> Overload : Substracts two the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to substract </param>
		/// <returns></returns>
		Vector4& operator-=(float _scalar);

		//// <summary> Overload : Returns the multiplication of two vectors </summary>
		/// <param name="_scalar"> : Second vector you want to multiply by </param>
		/// <returns></returns>
		Vector4 operator*(Vector4& _vector2);

		//// <summary> Overload : Returns the multiplication of the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to multiply the vector by </param>
		/// <returns></returns>
		Vector4 operator*(float _scalar);

		//// <summary> Overload : Multiplies two vectors together </summary>
		/// <param name="_scalar"> : Second vector you want to multiply the vector by </param>
		/// <returns></returns>
		Vector4& operator*=(Vector4& _vector2);

		//// <summary> Overload : Multiplies the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to multiply the vector by </param>
		/// <returns></returns>
		Vector4& operator*=(float _scalar);

		//// <summary> Overload : Returns the division of the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to divide the vector by </param>
		/// <returns></returns>
		Vector4 operator/(float _scalar);

		//// <summary> Overload : Divides the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to divide the vector by </param>
		/// <returns></returns>
		Vector4& operator/=(float _scalar);

		/// <summary> Overloard : Returns a component of the vector </summary>
		/// <param name="_index"> : Index of the component you want </param>
		/// <returns></returns>
		float operator[](int _index);
	};
}