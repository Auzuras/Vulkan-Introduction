#include "Matrices/Matrix4.h"
#include "Matrices/Matrix3.h"
#include "Utils/Utils.h"

namespace Math
{
	const Matrix4 Matrix4::identity({
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
		});

	const Matrix4 Matrix4::zero(0.f);

	const Matrix4 Matrix4::one(1.f);

	Matrix4::Matrix4(const std::array<float, 16> _values)
	{
		m_Values[0][0] = _values[0], m_Values[0][1] = _values[1], m_Values[0][2] = _values[2], m_Values[0][3] = _values[3];
		m_Values[1][0] = _values[4], m_Values[1][1] = _values[5], m_Values[1][2] = _values[6], m_Values[1][3] = _values[7];
		m_Values[2][0] = _values[8], m_Values[2][1] = _values[9], m_Values[2][2] = _values[10], m_Values[2][3] = _values[11];
		m_Values[3][0] = _values[12], m_Values[3][1] = _values[13], m_Values[3][2] = _values[14], m_Values[3][3] = _values[15];
	}

	Matrix4::Matrix4(const float _value)
	{
		m_Values[0][0] = _value, m_Values[0][1] = _value, m_Values[0][2] = _value, m_Values[0][3] = _value;
		m_Values[1][0] = _value, m_Values[1][1] = _value, m_Values[1][2] = _value, m_Values[1][3] = _value;
		m_Values[2][0] = _value, m_Values[2][1] = _value, m_Values[2][2] = _value, m_Values[2][3] = _value;
		m_Values[3][0] = _value, m_Values[3][1] = _value, m_Values[3][2] = _value, m_Values[3][3] = _value;
	}

	Matrix4::Matrix4(Vector4& _vector1, Vector4& _vector2, Vector4& _vector3, Vector4& _vector4)
	{
		m_Values[0][0] = _vector1[0], m_Values[0][1] = _vector1[1], m_Values[0][2] = _vector1[2], m_Values[0][3] = _vector1[3];
		m_Values[1][0] = _vector2[0], m_Values[1][1] = _vector2[1], m_Values[1][2] = _vector2[2], m_Values[1][3] = _vector2[3];
		m_Values[2][0] = _vector3[0], m_Values[2][1] = _vector3[1], m_Values[2][2] = _vector3[2], m_Values[2][3] = _vector3[3];
		m_Values[3][0] = _vector4[0], m_Values[3][1] = _vector4[1], m_Values[3][2] = _vector4[2], m_Values[3][3] = _vector4[3];
	}

	Matrix4::Matrix4(const Matrix4& _matrix)
	{
		*this = _matrix;
	}

	void Matrix4::Print() const
	{
		std::cout << "[ " << RED << m_Values[0][0] << " " << m_Values[0][1] << " " << m_Values[0][2] << " " << m_Values[0][3] << END << " ]" << std::endl;
		std::cout << "[ " << GREEN << m_Values[1][0] << " " << m_Values[1][1] << " " << m_Values[1][2] << " " << m_Values[1][3] << END << " ]" << std::endl;
		std::cout << "[ " << BLUE << m_Values[2][0] << " " << m_Values[2][1] << " " << m_Values[2][2] << " " << m_Values[2][3] << END << " ]" << std::endl;
		std::cout << "[ " << YELLOW << m_Values[3][0] << " " << m_Values[3][1] << " " << m_Values[3][2] << " " << m_Values[3][3] << END << " ]" << std::endl;
		std::cout << std::endl;
	}

	Matrix4 Matrix4::Opposite()
	{
		return Matrix4({
			-m_Values[0][0], -m_Values[0][1], -m_Values[0][2], -m_Values[0][3],
			-m_Values[1][0], -m_Values[1][1], -m_Values[1][2], -m_Values[1][3],
			-m_Values[2][0], -m_Values[2][1], -m_Values[2][2], -m_Values[2][3],
			-m_Values[3][0], -m_Values[3][1], -m_Values[3][2], -m_Values[3][3]
			});
	}

	Matrix4 Matrix4::Identity()
	{
		return identity;
	}

	Matrix4 Matrix4::Add(const Matrix4& _matrix2)
	{
		Matrix4 result(0.f);

		result.m_Values[0][0] = m_Values[0][0] + _matrix2.m_Values[0][0];
		result.m_Values[1][0] = m_Values[1][0] + _matrix2.m_Values[1][0];
		result.m_Values[2][0] = m_Values[2][0] + _matrix2.m_Values[2][0];
		result.m_Values[3][0] = m_Values[3][0] + _matrix2.m_Values[3][0];

		result.m_Values[0][1] = m_Values[0][1] + _matrix2.m_Values[0][1];
		result.m_Values[1][1] = m_Values[1][1] + _matrix2.m_Values[1][1];
		result.m_Values[2][1] = m_Values[2][1] + _matrix2.m_Values[2][1];
		result.m_Values[3][1] = m_Values[3][1] + _matrix2.m_Values[3][1];

		result.m_Values[0][2] = m_Values[0][2] + _matrix2.m_Values[0][2];
		result.m_Values[1][2] = m_Values[1][2] + _matrix2.m_Values[1][2];
		result.m_Values[2][2] = m_Values[2][2] + _matrix2.m_Values[2][2];
		result.m_Values[3][2] = m_Values[3][2] + _matrix2.m_Values[3][2];

		result.m_Values[0][3] = m_Values[0][3] + _matrix2.m_Values[0][3];
		result.m_Values[1][3] = m_Values[1][3] + _matrix2.m_Values[1][3];
		result.m_Values[2][3] = m_Values[2][3] + _matrix2.m_Values[2][3];
		result.m_Values[3][3] = m_Values[3][3] + _matrix2.m_Values[3][3];

		return result;
	}

	Matrix4 Matrix4::Add(const float _scalar)
	{
		return Matrix4({
			m_Values[0][0] + _scalar, m_Values[0][1] + _scalar, m_Values[0][2] + _scalar, m_Values[0][3] + _scalar,
			m_Values[1][0] + _scalar, m_Values[1][1] + _scalar, m_Values[1][2] + _scalar, m_Values[1][3] + _scalar,
			m_Values[2][0] + _scalar, m_Values[2][1] + _scalar, m_Values[2][2] + _scalar, m_Values[2][3] + _scalar,
			m_Values[3][0] + _scalar, m_Values[3][1] + _scalar, m_Values[3][2] + _scalar, m_Values[3][3] + _scalar
			});
	}

	Matrix4 Matrix4::Multiply(const Matrix4& _matrix2)
	{
		Matrix4 result(0.f);

		result.m_Values[0][0] = m_Values[0][0] * _matrix2.m_Values[0][0] + m_Values[0][1] * _matrix2.m_Values[1][0] + m_Values[0][2] * _matrix2.m_Values[2][0] + m_Values[0][3] * _matrix2.m_Values[3][0];
		result.m_Values[0][1] = m_Values[0][0] * _matrix2.m_Values[0][1] + m_Values[0][1] * _matrix2.m_Values[1][1] + m_Values[0][2] * _matrix2.m_Values[2][1] + m_Values[0][3] * _matrix2.m_Values[3][1];
		result.m_Values[0][2] = m_Values[0][0] * _matrix2.m_Values[0][2] + m_Values[0][1] * _matrix2.m_Values[1][2] + m_Values[0][2] * _matrix2.m_Values[2][2] + m_Values[0][3] * _matrix2.m_Values[3][2];
		result.m_Values[0][3] = m_Values[0][0] * _matrix2.m_Values[0][3] + m_Values[0][1] * _matrix2.m_Values[1][3] + m_Values[0][2] * _matrix2.m_Values[2][3] + m_Values[0][3] * _matrix2.m_Values[3][3];

		result.m_Values[1][0] = m_Values[1][0] * _matrix2.m_Values[0][0] + m_Values[1][1] * _matrix2.m_Values[1][0] + m_Values[1][2] * _matrix2.m_Values[2][0] + m_Values[1][3] * _matrix2.m_Values[3][0];
		result.m_Values[1][1] = m_Values[1][0] * _matrix2.m_Values[0][1] + m_Values[1][1] * _matrix2.m_Values[1][1] + m_Values[1][2] * _matrix2.m_Values[2][1] + m_Values[1][3] * _matrix2.m_Values[3][1];
		result.m_Values[1][2] = m_Values[1][0] * _matrix2.m_Values[0][2] + m_Values[1][1] * _matrix2.m_Values[1][2] + m_Values[1][2] * _matrix2.m_Values[2][2] + m_Values[1][3] * _matrix2.m_Values[3][2];
		result.m_Values[1][3] = m_Values[1][0] * _matrix2.m_Values[0][3] + m_Values[1][1] * _matrix2.m_Values[1][3] + m_Values[1][2] * _matrix2.m_Values[2][3] + m_Values[1][3] * _matrix2.m_Values[3][3];

		result.m_Values[2][0] = m_Values[2][0] * _matrix2.m_Values[0][0] + m_Values[2][1] * _matrix2.m_Values[1][0] + m_Values[2][2] * _matrix2.m_Values[2][0] + m_Values[2][3] * _matrix2.m_Values[3][0];
		result.m_Values[2][1] = m_Values[2][0] * _matrix2.m_Values[0][1] + m_Values[2][1] * _matrix2.m_Values[1][1] + m_Values[2][2] * _matrix2.m_Values[2][1] + m_Values[2][3] * _matrix2.m_Values[3][1];
		result.m_Values[2][2] = m_Values[2][0] * _matrix2.m_Values[0][2] + m_Values[2][1] * _matrix2.m_Values[1][2] + m_Values[2][2] * _matrix2.m_Values[2][2] + m_Values[2][3] * _matrix2.m_Values[3][2];
		result.m_Values[2][3] = m_Values[2][0] * _matrix2.m_Values[0][3] + m_Values[2][1] * _matrix2.m_Values[1][3] + m_Values[2][2] * _matrix2.m_Values[2][3] + m_Values[2][3] * _matrix2.m_Values[3][3];

		result.m_Values[3][0] = m_Values[3][0] * _matrix2.m_Values[0][0] + m_Values[3][1] * _matrix2.m_Values[1][0] + m_Values[3][2] * _matrix2.m_Values[2][0] + m_Values[3][3] * _matrix2.m_Values[3][0];
		result.m_Values[3][1] = m_Values[3][0] * _matrix2.m_Values[0][1] + m_Values[3][1] * _matrix2.m_Values[1][1] + m_Values[3][2] * _matrix2.m_Values[2][1] + m_Values[3][3] * _matrix2.m_Values[3][1];
		result.m_Values[3][2] = m_Values[3][0] * _matrix2.m_Values[0][2] + m_Values[3][1] * _matrix2.m_Values[1][2] + m_Values[3][2] * _matrix2.m_Values[2][2] + m_Values[3][3] * _matrix2.m_Values[3][2];
		result.m_Values[3][3] = m_Values[3][0] * _matrix2.m_Values[0][3] + m_Values[3][1] * _matrix2.m_Values[1][3] + m_Values[3][2] * _matrix2.m_Values[2][3] + m_Values[3][3] * _matrix2.m_Values[3][3];

		return result;
	}

	Matrix4 Matrix4::Multiply(const float _scalar)
	{
		return Matrix4({
			m_Values[0][0] * _scalar, m_Values[0][1] * _scalar, m_Values[0][2] * _scalar, m_Values[0][3] * _scalar,
			m_Values[1][0] * _scalar, m_Values[1][1] * _scalar, m_Values[1][2] * _scalar, m_Values[1][3] * _scalar,
			m_Values[2][0] * _scalar, m_Values[2][1] * _scalar, m_Values[2][2] * _scalar, m_Values[2][3] * _scalar,
			m_Values[3][0] * _scalar, m_Values[3][1] * _scalar, m_Values[3][2] * _scalar, m_Values[3][3] * _scalar
			});
	}

	Vector4 Matrix4::Multiply(Vector4 _vector)
	{
		return Vector4(
			m_Values[0][0] * _vector[0] + m_Values[0][1] * _vector[1] + m_Values[0][2] * _vector[2] + m_Values[0][3] * _vector[3],
			m_Values[1][0] * _vector[0] + m_Values[1][1] * _vector[1] + m_Values[1][2] * _vector[2] + m_Values[1][3] * _vector[3],
			m_Values[2][0] * _vector[0] + m_Values[2][1] * _vector[1] + m_Values[2][2] * _vector[2] + m_Values[2][3] * _vector[3],
			m_Values[3][0] * _vector[0] + m_Values[3][1] * _vector[1] + m_Values[3][2] * _vector[2] + m_Values[3][3] * _vector[3]
		);
	}

	Matrix4 Matrix4::Divide(float _scalar)
	{
		assert(_scalar != 0.f && "The scalar is equal to zero");

		if (_scalar == 0)
			return *this;

		return Matrix4({
			m_Values[0][0] / _scalar, m_Values[0][1] / _scalar, m_Values[0][2] / _scalar, m_Values[0][3] / _scalar,
			m_Values[1][0] / _scalar, m_Values[1][1] / _scalar, m_Values[1][2] / _scalar, m_Values[1][3] / _scalar,
			m_Values[2][0] / _scalar, m_Values[2][1] / _scalar, m_Values[2][2] / _scalar, m_Values[2][3] / _scalar,
			m_Values[3][0] / _scalar, m_Values[3][1] / _scalar, m_Values[3][2] / _scalar, m_Values[3][3] / _scalar
			});
	}

	const Vector4 Matrix4::Diagonal() const
	{
		return Vector4(m_Values[0][0], m_Values[1][1], m_Values[2][2], m_Values[3][3]);
	}

	Matrix4 Matrix4::Transpose()
	{
		return Matrix4({
			m_Values[0][0], m_Values[1][0], m_Values[2][0], m_Values[3][0],
			m_Values[0][1], m_Values[1][1], m_Values[2][1], m_Values[3][1],
			m_Values[0][2], m_Values[1][2], m_Values[2][2], m_Values[3][2],
			m_Values[0][3], m_Values[1][3], m_Values[2][3], m_Values[3][3]
			});
	}

	const float Matrix4::Trace() const
	{
		return m_Values[0][0] + m_Values[1][1] + m_Values[2][2] + m_Values[3][3];
	}

	const float Matrix4::Determinant() const
	{
		return fabsf(m_Values[0][0] * (m_Values[1][1] * (m_Values[2][2] * m_Values[3][3] - m_Values[3][2] * m_Values[2][3]) - m_Values[2][1] * (m_Values[1][2] * m_Values[3][3] - m_Values[3][2] * m_Values[1][3]) + m_Values[3][1] * (m_Values[1][2] * m_Values[2][3] - m_Values[2][2] * m_Values[1][3]))
			- m_Values[0][1] * (m_Values[1][0] * (m_Values[2][2] * m_Values[3][3] - m_Values[3][2] * m_Values[2][3]) - m_Values[2][0] * (m_Values[1][2] * m_Values[3][3] - m_Values[3][2] * m_Values[1][3]) + m_Values[3][0] * (m_Values[1][2] * m_Values[2][3] - m_Values[2][2] * m_Values[1][3]))
			+ m_Values[0][2] * (m_Values[1][0] * (m_Values[2][1] * m_Values[3][3] - m_Values[3][1] * m_Values[2][3]) - m_Values[2][0] * (m_Values[1][1] * m_Values[3][3] - m_Values[3][1] * m_Values[1][3]) + m_Values[3][0] * (m_Values[1][1] * m_Values[2][3] - m_Values[2][1] * m_Values[1][3]))
			- m_Values[0][3] * (m_Values[1][0] * (m_Values[2][1] * m_Values[3][2] - m_Values[3][1] * m_Values[2][2]) - m_Values[2][0] * (m_Values[1][1] * m_Values[3][2] - m_Values[3][1] * m_Values[1][2]) + m_Values[3][0] * (m_Values[1][1] * m_Values[2][2] - m_Values[2][1] * m_Values[1][2])));
	}

	Matrix4 Matrix4::Inverse()
	{
		float det = Determinant();

		if (det == 0.f)
			return *this;

		float invDet = 1.f / det;

		return Matrix4({
			invDet * (m_Values[1][1] * m_Values[2][2] * m_Values[3][3] + m_Values[1][2] * m_Values[2][3] * m_Values[3][2] + m_Values[1][3] * m_Values[2][1] * m_Values[3][2] - m_Values[1][1] * m_Values[2][3] * m_Values[3][2] - m_Values[1][2] * m_Values[2][1] * m_Values[3][3] - m_Values[1][3] * m_Values[2][2] * m_Values[3][1]),
			invDet * (m_Values[0][1] * m_Values[2][3] * m_Values[3][2] + m_Values[0][2] * m_Values[2][1] * m_Values[3][3] + m_Values[0][3] * m_Values[2][2] * m_Values[3][1] - m_Values[0][1] * m_Values[2][2] * m_Values[3][3] - m_Values[0][2] * m_Values[2][3] * m_Values[3][1] - m_Values[0][3] * m_Values[2][1] * m_Values[3][2]),
			invDet * (m_Values[0][1] * m_Values[1][2] * m_Values[3][3] + m_Values[0][2] * m_Values[1][3] * m_Values[3][1] + m_Values[0][3] * m_Values[1][1] * m_Values[3][2] - m_Values[0][1] * m_Values[1][3] * m_Values[3][2] - m_Values[0][2] * m_Values[1][1] * m_Values[3][3] - m_Values[0][3] * m_Values[1][2] * m_Values[3][1]),
			invDet * (m_Values[0][1] * m_Values[1][3] * m_Values[2][2] + m_Values[0][2] * m_Values[1][1] * m_Values[2][3] + m_Values[0][3] * m_Values[1][2] * m_Values[2][1] - m_Values[0][1] * m_Values[1][2] * m_Values[2][3] - m_Values[0][2] * m_Values[1][3] * m_Values[2][1] - m_Values[0][3] * m_Values[1][1] * m_Values[2][2]),

			invDet * (m_Values[1][0] * m_Values[2][3] * m_Values[3][2] + m_Values[1][2] * m_Values[2][0] * m_Values[3][3] + m_Values[1][3] * m_Values[2][2] * m_Values[3][0] - m_Values[1][0] * m_Values[2][2] * m_Values[3][3] - m_Values[1][2] * m_Values[2][3] * m_Values[3][0] - m_Values[1][3] * m_Values[2][0] * m_Values[3][2]),
			invDet * (m_Values[0][0] * m_Values[2][2] * m_Values[3][3] + m_Values[0][2] * m_Values[2][3] * m_Values[3][0] + m_Values[0][3] * m_Values[2][0] * m_Values[3][2] - m_Values[0][0] * m_Values[2][3] * m_Values[3][2] - m_Values[0][2] * m_Values[2][0] * m_Values[3][3] - m_Values[0][3] * m_Values[2][2] * m_Values[3][0]),
			invDet * (m_Values[0][0] * m_Values[1][3] * m_Values[3][2] + m_Values[0][2] * m_Values[1][0] * m_Values[3][3] + m_Values[0][3] * m_Values[1][2] * m_Values[3][0] - m_Values[0][0] * m_Values[1][2] * m_Values[3][3] - m_Values[0][2] * m_Values[1][3] * m_Values[3][0] - m_Values[0][3] * m_Values[1][0] * m_Values[3][2]),
			invDet * (m_Values[0][0] * m_Values[1][2] * m_Values[2][3] + m_Values[0][2] * m_Values[1][3] * m_Values[2][0] + m_Values[0][3] * m_Values[1][0] * m_Values[2][2] - m_Values[0][0] * m_Values[1][3] * m_Values[2][2] - m_Values[0][2] * m_Values[1][0] * m_Values[2][3] - m_Values[0][3] * m_Values[1][2] * m_Values[2][0]),

			invDet * (m_Values[1][0] * m_Values[2][1] * m_Values[3][3] + m_Values[1][1] * m_Values[2][3] * m_Values[3][0] + m_Values[1][3] * m_Values[2][0] * m_Values[3][1] - m_Values[1][0] * m_Values[2][3] * m_Values[3][1] - m_Values[1][1] * m_Values[2][0] * m_Values[3][3] - m_Values[1][3] * m_Values[2][1] * m_Values[3][0]),
			invDet * (m_Values[0][0] * m_Values[2][3] * m_Values[3][1] + m_Values[0][1] * m_Values[2][0] * m_Values[3][3] + m_Values[0][3] * m_Values[2][1] * m_Values[3][0] - m_Values[0][0] * m_Values[2][1] * m_Values[3][3] - m_Values[0][1] * m_Values[2][3] * m_Values[3][0] - m_Values[0][3] * m_Values[2][0] * m_Values[3][1]),
			invDet * (m_Values[0][0] * m_Values[1][1] * m_Values[3][3] + m_Values[0][1] * m_Values[1][3] * m_Values[3][0] + m_Values[0][3] * m_Values[1][0] * m_Values[3][1] - m_Values[0][0] * m_Values[1][3] * m_Values[3][1] - m_Values[0][1] * m_Values[1][0] * m_Values[3][3] - m_Values[0][3] * m_Values[1][1] * m_Values[3][0]),
			invDet * (m_Values[0][0] * m_Values[1][3] * m_Values[2][1] + m_Values[0][1] * m_Values[1][0] * m_Values[2][3] + m_Values[0][3] * m_Values[1][1] * m_Values[2][0] - m_Values[0][0] * m_Values[1][1] * m_Values[2][3] - m_Values[0][1] * m_Values[1][3] * m_Values[2][0] - m_Values[0][3] * m_Values[1][0] * m_Values[2][1]),

			invDet * (m_Values[1][0] * m_Values[2][2] * m_Values[3][1] + m_Values[1][1] * m_Values[2][0] * m_Values[3][2] + m_Values[1][2] * m_Values[2][1] * m_Values[3][0] - m_Values[1][0] * m_Values[2][1] * m_Values[3][2] - m_Values[1][1] * m_Values[2][2] * m_Values[3][0] - m_Values[1][2] * m_Values[2][0] * m_Values[3][1]),
			invDet * (m_Values[0][0] * m_Values[2][1] * m_Values[3][2] + m_Values[0][1] * m_Values[2][2] * m_Values[3][0] + m_Values[0][2] * m_Values[2][0] * m_Values[3][1] - m_Values[0][0] * m_Values[2][2] * m_Values[3][1] - m_Values[0][1] * m_Values[2][0] * m_Values[3][2] - m_Values[0][2] * m_Values[2][1] * m_Values[3][0]),
			invDet * (m_Values[0][0] * m_Values[1][2] * m_Values[3][1] + m_Values[0][1] * m_Values[1][0] * m_Values[3][2] + m_Values[0][2] * m_Values[1][1] * m_Values[3][0] - m_Values[0][0] * m_Values[1][1] * m_Values[3][2] - m_Values[0][1] * m_Values[1][2] * m_Values[3][0] - m_Values[0][2] * m_Values[1][0] * m_Values[3][1]),
			invDet * (m_Values[0][0] * m_Values[1][1] * m_Values[2][2] + m_Values[0][1] * m_Values[1][2] * m_Values[2][0] + m_Values[0][2] * m_Values[1][0] * m_Values[2][1] - m_Values[0][0] * m_Values[1][2] * m_Values[2][1] - m_Values[0][1] * m_Values[1][0] * m_Values[2][2] - m_Values[0][2] * m_Values[1][1] * m_Values[2][0])
			});

	}

	Matrix4 Matrix4::TRS(const float _xTranslation, const float _yTranslation, const float _zTranslation,
		const float _xAngle, const float _yAngle, const float _zAngle,
		const float _xScale, const float _yScale, const float _zScale)
	{
		return Translate(_xTranslation, _yTranslation, _zTranslation).Multiply(GlobalRotation(_xAngle, _yAngle, _zAngle)).Multiply(Scale(_xScale, _yScale, _zScale));
	}

	Matrix4 Matrix4::TRS(Vector3& _translation, Vector3& _rotation, Vector3& _scaling)
	{
		return Translate(_translation[0], _translation[1], _translation[2]).Multiply(GlobalRotation(_rotation[0], _rotation[1], _rotation[2])).Multiply(Scale(_scaling[0], _scaling[1], _scaling[2]));
	}

	Matrix4 Matrix4::Translate(const float _xTranslation, const float _yTranslation, const float _zTranslation)
	{
		return Matrix4({
			1.f, 0.f, 0.f, _xTranslation,
			0.f, 1.f, 0.f, _yTranslation,
			0.f, 0.f, 1.f, _zTranslation,
			0.f, 0.f, 0.f, 1.f
			});
	}

	Matrix4 Matrix4::GlobalRotation(const float _xAngle, const float _yAngle, const float _zAngle)
	{
		return XRotation(_xAngle).Multiply(YRotation(_yAngle)).Multiply(ZRotation(_zAngle));
	}

	Matrix4 Matrix4::XRotation(const float _angle)
	{
		return Matrix4({
			1.f, 0.f, 0.f, 0.f,
			0.f, cosf(_angle), -sinf(_angle), 0.f,
			0.f, sinf(_angle), cosf(_angle), 0.f,
			0.f, 0.f, 0.f, 1.f
			});
	}

	Matrix4 Matrix4::YRotation(const float _angle)
	{
		return Matrix4({
			cosf(_angle), 0.f, sinf(_angle), 0.f,
			0.f, 1.f, 0.f, 0.f,
			-sinf(_angle), 0.f, cosf(_angle), 0.f,
			0.f, 0.f, 0.f, 1.f
			});
	}

	Matrix4 Matrix4::ZRotation(const float _angle)
	{
		return Matrix4({
			cosf(_angle), -sinf(_angle), 0.f, 0.f,
			sinf(_angle), cosf(_angle), 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
			});
	}

	Matrix4 Matrix4::Scale(const float _xScale, const float _yScale, const float _zScale)
	{
		return Matrix4({
			_xScale, 0.f, 0.f, 0.f,
			0.f, _yScale, 0.f, 0.f,
			0.f, 0.f, _zScale, 0.f,
			0.f, 0.f, 0.f, 1.f
			});
	}

	Matrix4 Matrix4::ViewMatrix(Vector3 _eye, Vector3 _lookAt, Vector3 _up)
	{
		Math::Vector3 zAxis = (_eye - _lookAt).Normalize();
		Math::Vector3 xAxis = _up.CrossProduct(zAxis);
		xAxis = xAxis.Normalize();
		Math::Vector3 yAxis = zAxis.CrossProduct(xAxis);
		yAxis = yAxis.Normalize();

		return Matrix4({
			xAxis[0], xAxis[1], xAxis[2], -_eye.m_X,
			yAxis[0], yAxis[1], yAxis[2], -_eye.m_Y,
			zAxis[0], zAxis[1], zAxis[2], -_eye.m_Z,
			0.f, 0.f, 0.f, 1.f
			});
	}


	Matrix4 Matrix4::ProjectionPerspectiveMatrix(const float _near, const float _far, const float _aspect, const float _fov)
	{
		float tanHalfFov = tanf(Utils::DegToRad(_fov) * 0.5f);
		float s = 1.0f / (_aspect * tanHalfFov);
		float s2 = 1.0f / tanHalfFov;

		// OPENGL Projection matrix
		//return Matrix4({
		//	s, 0.f, 0.f, 0.f,
		//	0.f, s2, 0.f, 0.f,
		//	0.f, 0.f, -(_far) / (_far - _near), -(_far * _near) / (_far - _near),
		//	0.f, 0.f, -1.f, 0.f
		//	});

		// VULKAN Projection matrix
		return Matrix4({
			s, 0.f, 0.f, 0.f,
			0.f, s2, 0.f, 0.f,
			0.f, 0.f, _far / (_far - _near), -(_far * _near) / (_far - _near),
			0.f, 0.f, 1.f, 0.f
			});

	}

	Matrix4 Matrix4::ProjectionOrthographicMatrix(const float _near, const float _far, const float _top, const float _bottom, const float _right, const float _left)
	{
		return Matrix4({
			2.f / (_right - _left), 0.f, 0.f, -(_right + _left) / (_right - _left),
			0.f, 2.f / (_top - _bottom), 0.f,-(_top + _bottom) / (_top - _bottom),
			0.f, 0.f, -2.f / (_far - _near), -(_near) * ((_far) / (_far - _near)),
			0.f, 0.f, 0.f, 1.f
			});
	}


	float* Matrix4::Value()
	{
		return &m_Values[0][0];
	}

	Matrix4 Matrix4::operator+(const Matrix4& _matrix2)
	{
		Matrix4 result(0.f);
		result = Add(_matrix2);

		return result;
	}

	Matrix4 Matrix4::operator+(const float _scalar)
	{
		Matrix4 result(0.f);
		result = Add(_scalar);

		return result;
	}

	Matrix4& Matrix4::operator+=(const Matrix4& _matrix2)
	{
		*this = Add(_matrix2);
		return *this;
	}

	Matrix4& Matrix4::operator+=(const float _scalar)
	{
		*this = Add(_scalar);
		return *this;
	}

	Matrix4 Matrix4::operator-(Matrix4& _matrix2)
	{
		Matrix4 result(0.f);
		result = Add(_matrix2.Opposite());

		return result;
	}

	Matrix4 Matrix4::operator-(const float _scalar)
	{
		Matrix4 result(0.f);
		result = Add(-_scalar);

		return result;
	}

	Matrix4& Matrix4::operator-=(Matrix4& _matrix2)
	{
		*this = Add(_matrix2.Opposite());
		return *this;
	}

	Matrix4& Matrix4::operator-=(const float _scalar)
	{
		*this = Add(-_scalar);
		return *this;
	}

	Matrix4 Matrix4::operator*(const Matrix4& _matrix2)
	{
		Matrix4 result(0.f);
		result = Multiply(_matrix2);

		return result;
	}

	Matrix4 Matrix4::operator*(const float _scalar)
	{
		Matrix4 result(0.f);
		result = Multiply(_scalar);

		return result;
	}

	Vector4 Matrix4::operator*(const Vector4& _vector)
	{
		Vector4 result(0.f);
		result = Multiply(_vector);

		return result;
	}

	Matrix4& Matrix4::operator*=(const Matrix4& _matrix2)
	{
		*this = Multiply(_matrix2);
		return *this;
	}

	Matrix4& Matrix4::operator*=(const float _scalar)
	{
		*this = Multiply(_scalar);
		return *this;
	}

	Matrix4 Matrix4::operator/(const float _scalar)
	{
		Matrix4 result(0.f);
		result = Divide(_scalar);

		return result;
	}

	Matrix4& Matrix4::operator/=(const float _scalar)
	{
		*this = Divide(_scalar);
		return *this;
	}

	Vector4 Matrix4::operator[](const int _index)
	{
		assert(0 <= _index && _index < 4 && "Invalid index");

		switch (_index)
		{
		case 0:
			return Vector4(m_Values[0][0], m_Values[0][1], m_Values[0][2], m_Values[0][3]);
			break;
		case 1:
			return Vector4(m_Values[1][0], m_Values[1][1], m_Values[1][2], m_Values[1][3]);
			break;
		case 2:
			return Vector4(m_Values[2][0], m_Values[2][1], m_Values[2][2], m_Values[2][3]);
			break;
		case 3:
			return Vector4(m_Values[3][0], m_Values[3][1], m_Values[3][2], m_Values[3][3]);
			break;
		}
		return Vector4(0.f);
	}
}