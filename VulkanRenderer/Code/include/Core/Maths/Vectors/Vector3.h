#pragma once

#include <iostream>
#include <cassert>

namespace Math
{
	class Vector3
	{
	private:
		float m_X;
		float m_Y;
		float m_Z;

	public:
		Vector3() = default;

		/// <summary> Creates a vector3 </summary>
		/// <param name="_x"> : x parameter</param>
		/// <param name="_y"> : y parameter</param>
		/// <param name="_y"> : z parameter</param>
		Vector3(float _x, float _y, float _z);

		/// <summary> Creates a vector of size 3 with the same values </summary>
		/// <param name="_nbr"> : Value you want in the vector </param>
		Vector3(const float _nbr);

		/// <summary> Creates a vector by copy </summary>
		/// <param name="_matrix"> : Vector you want to copy </param>
		/// <returns></returns>
		Vector3(const Vector3& _vector);

		~Vector3() = default;

		///////////////////////////////////////////////////////////////////////

		/// PREDIFINED VECTORS

		///////////////////////////////////////////////////////////////////////

		static const Vector3 zero;
		static const Vector3 one;

		///////////////////////////////////////////////////////////////////////

		/// FUNCTIONS

		///////////////////////////////////////////////////////////////////////

		/// <summary> Prints the vector </summary>
		/// <returns></returns>
		void Print() const;

		/// <summary> Returns the opposite of the vector </summary>
		/// <returns></returns>
		Vector3 Opposite();

		/// <summary> Returns the normalized vector </summary>
		/// <returns></returns>
		Vector3 Normalize();

		/// <summary> Returns the addition of two vectors </summary>
		/// <param name="_vector2"> : Second vector you want to add </param>
		/// <returns></returns>
		Vector3 Add(const Vector3& _vector2);

		/// <summary> Returns the addition of the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Vector3 Add(const float _scalar);

		/// <summary> Returns the product of two vectors </summary>
		/// <param name="_vector2"> : Second vector you want to multiply by </param>
		/// <returns></returns>
		Vector3 Multiply(const Vector3& _vector2);

		/// <summary> Returns the product of the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to multiply by </param>
		/// <returns></returns>
		Vector3 Multiply(const float _scalar);

		/// <summary> Returns the division of a vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to divide by </param>
		/// <returns></returns>
		Vector3 Divide(float _scalar);

		/// <summary> Returns the dot product of two vectors </summary>
		/// <param name="_vector2"> : Second vector for dot product </param>
		/// <returns></returns>
		[[nodiscard]] const float DotProduct(const Vector3& _vector2) const;

		/// <summary> Returns the cross product of two vectors </summary>
		/// <param name="_vector2"> : Second vector for the cross product </param>
		/// <returns></returns>
		[[nodiscard]] const Vector3 CrossProduct(const Vector3& _vector2) const;

		/// <summary> Returns the angle between two vectors </summary>
		/// <param name="_vector3"> : Second vector for the angle </param>
		/// <returns></returns>
		[[nodiscard]] const float Angle(Vector3& _vector2) const;

		/// <summary> Returns the distance between two vectors </summary>
		/// <param name="_vector2"> : Second vector for distance </param>
		/// <returns></returns>
		[[nodiscard]] const float Distance(const Vector3& _vector2) const;

		/// <summary> Returns the squared morm of a vector </summary>
		/// <returns></returns>
		[[nodiscard]] const float SquaredNorm() const;

		/// <summary> Returns the norm of a vector </summary>
		/// <returns></returns>
		[[nodiscard]] const float Norm() const;

		/// <summary> Returns the midpoint of two vectors </summary>
		/// <param name="_vector2"> : Second vector for midpoint </param>
		/// <returns></returns>
		[[nodiscard]] const Vector3 MidPoint(const Vector3& _vector2) const;

		/// <summary> Rotates the vector arround a second one with an axis </summary>
		/// <param name="_angle"> : The angle you want to rotate </param>
		/// <param name="_anchor"> : Second vector you want to rotate arround (Default = 0, 0, 0) </param>
		/// <param name="_axis"> : Axis you want to rotate arround </param>
		/// <returns></returns>
		Vector3 Rotate(const float _angle, Vector3 _anchor = zero, Vector3 _axis = {1.f, 0.f, 0.f});

		/// <summary> Rotates the vector arround a second one </summary>
		/// <param name="_angle"> : The angle you want to rotate </param>
		/// <param name="_anchor"> : Second vector you want to rotate arround (Default = 0, 0, 0) </param>
		/// <returns></returns>
		Vector3 Rotate(const float _xAngle, const float _yAngle, const float _zAngle, Vector3 _anchor = zero);

		//////////////////////////////////////////////////////////////////////

		/// OPERATOR OVERLOADING

		///////////////////////////////////////////////////////////////////////

		/// <summary> Overload : Returns the addition of two vectors </summary>
		/// <param name="_matrix2"></param>
		/// <returns></returns>
		Vector3 operator+(Vector3& _vector2);

		/// <summary> Overload : Returns the addition of the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Vector3 operator+(float _scalar);

		//// <summary> Overload : Adds two vectors together </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Vector3& operator+=(Vector3& _vector2);

		//// <summary> Overload : Adds a scalar to the vector </summary>
		/// <param name="_scalar"> : Scalar you want to add </param>
		/// <returns></returns>
		Vector3& operator+=(float _scalar);

		//// <summary> Overload : Returns the substraction of two vectors </summary>
		/// <param name="_scalar"> : Second vector you want to substract </param>
		/// <returns></returns>
		Vector3 operator-(Vector3& _vector2);

		//// <summary> Overload : Returns the substraction of the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to substract </param>
		/// <returns></returns>
		Vector3 operator-(float _scalar);

		//// <summary> Overload : Substracts two vectors together </summary>
		/// <param name="_scalar"> : Second vector you want to substract </param>
		/// <returns></returns>
		Vector3& operator-=(Vector3& _vector2);

		//// <summary> Overload : Substracts two the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to substract </param>
		/// <returns></returns>
		Vector3& operator-=(float _scalar);

		//// <summary> Overload : Returns the multiplication of two vectors </summary>
		/// <param name="_scalar"> : Second vector you want to multiply by </param>
		/// <returns></returns>
		Vector3 operator*(Vector3& _vector2);

		//// <summary> Overload : Returns the multiplication of the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to multiply the vector by </param>
		/// <returns></returns>
		Vector3 operator*(float _scalar);

		//// <summary> Overload : Multiplies two vectors together </summary>
		/// <param name="_scalar"> : Second vector you want to multiply the vector by </param>
		/// <returns></returns>
		Vector3& operator*=(Vector3& _vector2);

		//// <summary> Overload : Multiplies the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to multiply the vector by </param>
		/// <returns></returns>
		Vector3& operator*=(float _scalar);

		//// <summary> Overload : Returns the division of the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to divide the vector by </param>
		/// <returns></returns>
		Vector3 operator/(float _scalar);

		//// <summary> Overload : Divides the vector with a scalar </summary>
		/// <param name="_scalar"> : Scalar you want to divide the vector by </param>
		/// <returns></returns>
		Vector3& operator/=(float _scalar);

		/// <summary> Overloard : Returns a component of the vector </summary>
		/// <param name="_index"> : Index of the component you want </param>
		/// <returns></returns>
		float operator[](int _index);
	};
}