#pragma once

#include <iostream>
#include <cassert>

namespace Math
{
	class Vector2
	{
	private:
		float m_X;
		float m_Y;

	public:
		Vector2() = default;

		/// <summary> Creates a vector2 </summary>
		/// <param name="_x"> : x parameter</param>
		/// <param name="_y"> : y parameter</param>
		Vector2(float _x, float _y);

		/// <summary> Creates a vector of size 2 with the same values </summary>
		/// <param name="_nbr"> : Value you want in the vector </param>
		Vector2(const float _nbr);

		/// <summary> Creates a vector by copy </summary>
		/// <param name="_matrix"> : Vector you want to copy </param>
		/// <returns></returns>
		Vector2(const Vector2& _vector);

		~Vector2() = default;

		///////////////////////////////////////////////////////////////////////

		/// PREDIFINED VECTORS

		///////////////////////////////////////////////////////////////////////

		static const Vector2 zero;
		static const Vector2 one;

		///////////////////////////////////////////////////////////////////////

		/// FUNCTIONS

		///////////////////////////////////////////////////////////////////////

		/// <summary> Prints the vector </summary>
		/// <returns></returns>
		void Print() const;

		/// <summary> Returns the opposite of the vector </summary>
		/// <returns></returns>
		Vector2 Opposite();

		/// <summary> Returns the normalized vector </summary>
		/// <returns></returns>
		Vector2 Normalize();

		/// <summary> Returns the addition of two vectors </summary>
		/// <param name="_vector2"> : Second vector you want to add </param>
		/// <returns></returns>
		Vector2 Add(const Vector2& _vector2);

		/// <summary> Returns the addition of the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Vector2 Add(const float _scalar);

		/// <summary> Returns the product of two vectors </summary>
		/// <param name="_vector2"> : Second vector you want to multiply by </param>
		/// <returns></returns>
		Vector2 Multiply(const Vector2& _vector2);

		/// <summary> Returns the product of the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to multiply by </param>
		/// <returns></returns>
		Vector2 Multiply(const float _scalar);

		/// <summary> Returns the division of a vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to divide by </param>
		/// <returns></returns>
		Vector2 Divide(float _scalar);

		/// <summary> Returns the dot product of two vectors </summary>
		/// <param name="_vector2"> : Second vector for dot product </param>
		/// <returns></returns>
		[[nodiscard]] const float DotProduct(const Vector2& _vector2) const;

		/// <summary> Returns the cross product of two vectors </summary>
		/// <param name="_vector2"> : Second vector for the cross product </param>
		/// <returns></returns>
		[[nodiscard]] const float CrossProduct(const Vector2& _vector2) const;

		/// <summary> Returns the angle between two vectors </summary>
		/// <param name="_vector3"> : Second vector for the angle </param>
		/// <returns></returns>
		[[nodiscard]] const float Angle(Vector2& _vector2) const;

		/// <summary> Returns the distance between two vectors </summary>
		/// <param name="_vector2"> : Second vector for distance </param>
		/// <returns></returns>
		[[nodiscard]] const float Distance(const Vector2& _vector2) const;

		/// <summary> Returns the squared morm of a vector </summary>
		/// <returns></returns>
		[[nodiscard]] const float SquaredNorm() const;

		/// <summary> Returns the norm of a vector </summary>
		/// <returns></returns>
		[[nodiscard]] const float Norm() const;

		/// <summary> Returns the midpoint of two vectors </summary>
		/// <param name="_vector2"> : Second vector for midpoint </param>
		/// <returns></returns>
		[[nodiscard]] const Vector2 MidPoint(const Vector2& _vector2) const;

		/// <summary> Rotates the vector arround a second one </summary>
		/// <param name="angle"> : The angle you want to rotate </param>
		/// <param name="_anchor"> : Second vector you want to rotate arround (Default = 0, 0, 0) </param>
		/// <returns></returns>
		Vector2 Rotate(const float _angle, Vector2 _anchor = zero);

		//////////////////////////////////////////////////////////////////////

		/// OPERATOR OVERLOADING

		///////////////////////////////////////////////////////////////////////

		/// <summary> Overload : Returns the addition of two vectors </summary>
		/// <param name="_matrix2"></param>
		/// <returns></returns>
		Vector2 operator+(Vector2& _vector2);

		/// <summary> Overload : Returns the addition of the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Vector2 operator+(float _scalar);

		//// <summary> Overload : Adds two vectors together </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Vector2& operator+=(Vector2& _vector2);

		//// <summary> Overload : Adds a scalar to the vector </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Vector2& operator+=(float _scalar);

		//// <summary> Overload : Returns the substraction of two vectors </summary>
		/// <param name="_scalar"> : Second vector you want to substract </param>
		/// <returns></returns>
		Vector2 operator-(Vector2& _vector2);

		//// <summary> Overload : Returns the substraction of the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to substract </param>
		/// <returns></returns>
		Vector2 operator-(float _scalar);

		//// <summary> Overload : Substracts two vectors together </summary>
		/// <param name="_scalar"> : Second vector you want to substract </param>
		/// <returns></returns>
		Vector2& operator-=(Vector2& _vector2);

		//// <summary> Overload : Substracts two the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to substract </param>
		/// <returns></returns>
		Vector2& operator-=(float _scalar);

		//// <summary> Overload : Returns the multiplication of two vectors </summary>
		/// <param name="_scalar"> : Second vector you want to multiply by </param>
		/// <returns></returns>
		Vector2 operator*(Vector2& _vector2);

		//// <summary> Overload : Returns the multiplication of the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to multiply the vector by </param>
		/// <returns></returns>
		Vector2 operator*(float _scalar);

		//// <summary> Overload : Multiplies two vectors together </summary>
		/// <param name="_scalar"> : Second vector you want to multiply the vector by </param>
		/// <returns></returns>
		Vector2& operator*=(Vector2& _vector2);

		//// <summary> Overload : Multiplies the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to multiply the vector by </param>
		/// <returns></returns>
		Vector2& operator*=(float _scalar);

		//// <summary> Overload : Returns the division of the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to divide the vector by </param>
		/// <returns></returns>
		Vector2 operator/(float _scalar);

		//// <summary> Overload : Divides the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to divide the vector by </param>
		/// <returns></returns>
		Vector2& operator/=(float _scalar);

		/// <summary> Overloard : Returns a component of the vector </summary>
		/// <param name="_index"> : Index of the component you want </param>
		/// <returns></returns>
		float operator[](int _index);
	};
}