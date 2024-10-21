#include "Matrices/Matrix2.h"
#include "Utils/Utils.h"

namespace Math
{
	const Matrix2 Matrix2::identity({
		1.f, 0.f,
		0.f, 1.f
		});

	const Matrix2 Matrix2::zero(0.f);

	const Matrix2 Matrix2::one(1.f);

	Matrix2::Matrix2(const std::array<float, 4> _values)
	{
		m_Values[0][0] = _values[0], m_Values[0][1] = _values[1];
		m_Values[1][0] = _values[2], m_Values[1][1] = _values[3];
	}

	Matrix2::Matrix2(const float _value)
	{
		m_Values[0][0] = _value, m_Values[0][1] = _value;
		m_Values[1][0] = _value, m_Values[1][1] = _value;
	}

	Matrix2::Matrix2(Vector2& _vector1, Vector2& _vector2)
	{
		m_Values[0][0] = _vector1[0], m_Values[0][1] = _vector1[1];
		m_Values[1][0] = _vector2[0], m_Values[1][1] = _vector2[1];
	}

	Matrix2::Matrix2(const Matrix2& _matrix)
	{
		*this = _matrix;
	}

	void Matrix2::Print() const
	{
		std::cout << "[ " << RED << m_Values[0][0] << " " << m_Values[0][1] << END << " ]" << std::endl;
		std::cout << "[ " << GREEN <<m_Values[1][0] << " " << m_Values[1][1] << END << " ]" << std::endl;
		std::cout << std::endl;
	}

	Matrix2 Matrix2::Opposite()
	{
		return Matrix2({
			-m_Values[0][0], -m_Values[0][1],
			-m_Values[1][0], -m_Values[1][1]
			});
	}

	Matrix2 Matrix2::Identity()
	{
		Matrix2 result({
			1.f, 0.f,
			0.f, 1.f
			});

		return result;
	}

	Matrix2 Matrix2::Add(const Matrix2& _matrix2)
	{
		Matrix2 result(0.f);

		result.m_Values[0][0] = m_Values[0][0] + _matrix2.m_Values[0][0];
		result.m_Values[1][0] = m_Values[1][0] + _matrix2.m_Values[1][0];

		result.m_Values[0][1] = m_Values[0][1] + _matrix2.m_Values[0][1];
		result.m_Values[1][1] = m_Values[1][1] + _matrix2.m_Values[1][1];

		return result;
	}

	Matrix2 Matrix2::Add(const float _scalar)
	{
		return Matrix2({
			m_Values[0][0] + _scalar, m_Values[0][1] + _scalar,
			m_Values[1][0] + _scalar, m_Values[1][1] + _scalar
			});
	}

	Matrix2 Matrix2::Multiply(const Matrix2& _matrix2)
	{
		Matrix2 result(0.f);

		result.m_Values[0][0] = m_Values[0][0] * _matrix2.m_Values[0][0] + m_Values[0][1] * _matrix2.m_Values[1][0];
		result.m_Values[0][1] = m_Values[0][0] * _matrix2.m_Values[0][1] + m_Values[0][1] * _matrix2.m_Values[1][1];

		result.m_Values[1][0] = m_Values[1][0] * _matrix2.m_Values[0][0] + m_Values[1][1] * _matrix2.m_Values[1][0];
		result.m_Values[1][1] = m_Values[1][0] * _matrix2.m_Values[0][1] + m_Values[1][1] * _matrix2.m_Values[1][1];

		return result;
	}

	Matrix2 Matrix2::Multiply(const float _scalar)
	{
		return Matrix2({
			m_Values[0][0] * _scalar, m_Values[0][1] * _scalar,
			m_Values[1][0] * _scalar, m_Values[1][1] * _scalar
			});
	}

	Vector2 Matrix2::Multiply(Vector2 _vector)
	{
		return Vector2(
			m_Values[0][0] * _vector[0] + m_Values[0][1] * _vector[1],
			m_Values[1][0] * _vector[0] + m_Values[1][1] * _vector[1]
		);
	}

	Matrix2 Matrix2::Divide(float _scalar)
	{
		assert(_scalar != 0.f && "The scalar is equal to zero");

		if (_scalar == 0.f)
			return *this;

		return Matrix2({
			m_Values[0][0] / _scalar, m_Values[0][1] / _scalar,
			m_Values[1][0] / _scalar, m_Values[1][1] / _scalar
			});
	}

	const Vector2 Matrix2::Diagonal() const
	{
		return Vector2(m_Values[0][0], m_Values[1][1]);
	}

	Matrix2 Matrix2::Transpose()
	{
		return Matrix2({
			m_Values[0][0], m_Values[1][0],
			m_Values[0][1], m_Values[1][1],
			});
	}

	const float Matrix2::Trace() const
	{
		return m_Values[0][0] + m_Values[1][1];
	}

	const float Matrix2::Determinant() const
	{
		return fabsf(m_Values[0][0] * m_Values[1][1] - m_Values[1][0] * m_Values[0][1]);
	}

	Matrix2 Matrix2::Inverse()
	{
		float det = Determinant();

		if (det == 0.f)
			return *this;

		float invDet = 1.f / det;

		return Matrix2({
			m_Values[1][1] * invDet, -m_Values[0][1] * invDet,
			-m_Values[1][0] * invDet, m_Values[0][0] * invDet
			});
	}

	Matrix2 Matrix2::Rotation(const float _angle)
	{
		return Matrix2({
			cosf(_angle), -sinf(_angle),
			sinf(_angle), cosf(_angle)
			});
	}

	Matrix2 Matrix2::operator+(const Matrix2& _matrix2)
	{
		Matrix2 result(0.f);
		result = Add(_matrix2);

		return result;
	}

	Matrix2 Matrix2::operator+(const float _scalar)
	{
		Matrix2 result(0.f);
		result = Add(_scalar);

		return result;
	}

	Matrix2& Matrix2::operator+=(const Matrix2& _matrix2)
	{
		*this = Add(_matrix2);
		return *this;
	}

	Matrix2& Matrix2::operator+=(const float _scalar)
	{
		*this = Add(_scalar);
		return *this;
	}

	Matrix2 Matrix2::operator-(Matrix2& _matrix2)
	{
		Matrix2 result(0.f);
		result = Add(_matrix2.Opposite());

		return result;
	}

	Matrix2 Matrix2::operator-(const float _scalar)
	{
		Matrix2 result(0.f);
		result = Add(-_scalar);

		return result;
	}

	Matrix2& Matrix2::operator-=(Matrix2& _matrix2)
	{
		*this = Add(_matrix2.Opposite());
		return *this;
	}

	Matrix2& Matrix2::operator-=(const float _scalar)
	{
		*this = Add(-_scalar);
		return *this;
	}

	Matrix2 Matrix2::operator*(const Matrix2& _matrix2)
	{
		Matrix2 result(0.f);
		result = Multiply(_matrix2);

		return result;
	}

	Matrix2 Matrix2::operator*(const float _scalar)
	{
		Matrix2 result(0.f);
		result = Multiply(_scalar);

		return result;
	}

	Vector2 Matrix2::operator*(const Vector2& _vector)
	{
		Vector2 result(0.f);
		result = Multiply(_vector);

		return result;
	}

	Matrix2& Matrix2::operator*=(const Matrix2& _matrix2)
	{
		*this = Multiply(_matrix2);
		return *this;
	}

	Matrix2& Matrix2::operator*=(const float _scalar)
	{
		*this = Multiply(_scalar);
		return *this;
	}

	Matrix2 Matrix2::operator/(const float _scalar)
	{
		Matrix2 result(0.f);
		result = Divide(_scalar);

		return result;
	}

	Matrix2& Matrix2::operator/=(const float _scalar)
	{
		*this = Divide(_scalar);
		return *this;
	}

	Vector2 Matrix2::operator[](const int _index)
	{
		assert(0 <= _index && _index < 2 && "Invalid index");

		switch (_index)
		{
		case 0:
			return Vector2(m_Values[0][0], m_Values[0][1]);
			break;
		case 1:
			return Vector2(m_Values[1][0], m_Values[1][1]);
			break;
		}
		return Vector2(0.f);
	}
}