#include "Matrices/Matrix3.h"
#include "Utils/Utils.h"

namespace Math
{
	const Matrix3 Matrix3::identity({
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f
		});

	const Matrix3 Matrix3::zero(0.f);

	const Matrix3 Matrix3::one(1.f);

	Matrix3::Matrix3(const std::array<float, 9> _values)
	{
		m_Values[0][0] = _values[0], m_Values[0][1] = _values[1], m_Values[0][2] = _values[2];
		m_Values[1][0] = _values[3], m_Values[1][1] = _values[4], m_Values[1][2] = _values[5];
		m_Values[2][0] = _values[6], m_Values[2][1] = _values[7], m_Values[2][2] = _values[8];
	}

	Matrix3::Matrix3(const float _value)
	{
		m_Values[0][0] = _value, m_Values[0][1] = _value, m_Values[0][2] = _value;
		m_Values[1][0] = _value, m_Values[1][1] = _value, m_Values[1][2] = _value;
		m_Values[2][0] = _value, m_Values[2][1] = _value, m_Values[2][2] = _value;
	}

	Matrix3::Matrix3(Vector3& _vector1, Vector3& _vector2, Vector3& _vector3)
	{
		m_Values[0][0] = _vector1[0], m_Values[0][1] = _vector1[1], m_Values[0][2] = _vector1[2];
		m_Values[1][0] = _vector2[0], m_Values[1][1] = _vector2[1], m_Values[1][2] = _vector2[2];
		m_Values[2][0] = _vector3[0], m_Values[2][1] = _vector3[1], m_Values[2][2] = _vector3[2];
	}

	Matrix3::Matrix3(const Matrix3& _matrix)
	{
		*this = _matrix;
	}

	void Matrix3::Print() const
	{
		std::cout << "[ " << RED << m_Values[0][0] << " " << m_Values[0][1] << " " << m_Values[0][2] << END << " ]" << std::endl;
		std::cout << "[ " << GREEN << m_Values[1][0] << " " << m_Values[1][1] << " " << m_Values[1][2] << END << " ]" << std::endl;
		std::cout << "[ " << BLUE << m_Values[2][0] << " " << m_Values[2][1] << " " << m_Values[2][2] << END << " ]" << std::endl;
		std::cout << std::endl;
	}

	Matrix3 Matrix3::Opposite()
	{
		return Matrix3({
			-m_Values[0][0], -m_Values[0][1], -m_Values[0][2],
			-m_Values[1][0], -m_Values[1][1], -m_Values[1][2],
			-m_Values[2][0], -m_Values[2][1], -m_Values[2][2],
			});
	}

	Matrix3 Matrix3::Identity()
	{
		return identity;
	}

	Matrix3 Matrix3::Add(const Matrix3& _matrix2)
	{
		Matrix3 result(0.f);

		result.m_Values[0][0] = m_Values[0][0] + _matrix2.m_Values[0][0];
		result.m_Values[1][0] = m_Values[1][0] + _matrix2.m_Values[1][0];
		result.m_Values[2][0] = m_Values[2][0] + _matrix2.m_Values[2][0];

		result.m_Values[0][1] = m_Values[0][1] + _matrix2.m_Values[0][1];
		result.m_Values[1][1] = m_Values[1][1] + _matrix2.m_Values[1][1];
		result.m_Values[2][1] = m_Values[2][1] + _matrix2.m_Values[2][1];

		result.m_Values[0][2] = m_Values[0][2] + _matrix2.m_Values[0][2];
		result.m_Values[1][2] = m_Values[1][2] + _matrix2.m_Values[1][2];
		result.m_Values[2][2] = m_Values[2][2] + _matrix2.m_Values[2][2];

		return result;
	}

	Matrix3 Matrix3::Add(const float _scalar)
	{
		return Matrix3({
			m_Values[0][0] + _scalar, m_Values[0][1] + _scalar, m_Values[0][2] + _scalar,
			m_Values[1][0] + _scalar, m_Values[1][1] + _scalar, m_Values[1][2] + _scalar,
			m_Values[2][0] + _scalar, m_Values[2][1] + _scalar, m_Values[2][2] + _scalar
			});
	}

	Matrix3 Matrix3::Multiply(const Matrix3& _matrix2)
	{
		Matrix3 result(0.f);

		result.m_Values[0][0] = m_Values[0][0] * _matrix2.m_Values[0][0] + m_Values[0][1] * _matrix2.m_Values[1][0] + m_Values[0][2] * _matrix2.m_Values[2][0];
		result.m_Values[0][1] = m_Values[0][0] * _matrix2.m_Values[0][1] + m_Values[0][1] * _matrix2.m_Values[1][1] + m_Values[0][2] * _matrix2.m_Values[2][1];
		result.m_Values[0][2] = m_Values[0][0] * _matrix2.m_Values[0][2] + m_Values[0][1] * _matrix2.m_Values[1][2] + m_Values[0][2] * _matrix2.m_Values[2][2];

		result.m_Values[1][0] = m_Values[1][0] * _matrix2.m_Values[0][0] + m_Values[1][1] * _matrix2.m_Values[1][0] + m_Values[1][2] * _matrix2.m_Values[2][0];
		result.m_Values[1][1] = m_Values[1][0] * _matrix2.m_Values[0][1] + m_Values[1][1] * _matrix2.m_Values[1][1] + m_Values[1][2] * _matrix2.m_Values[2][1];
		result.m_Values[1][2] = m_Values[1][0] * _matrix2.m_Values[0][2] + m_Values[1][1] * _matrix2.m_Values[1][2] + m_Values[1][2] * _matrix2.m_Values[2][2];

		result.m_Values[2][0] = m_Values[2][0] * _matrix2.m_Values[0][0] + m_Values[2][1] * _matrix2.m_Values[1][0] + m_Values[2][2] * _matrix2.m_Values[2][0];
		result.m_Values[2][1] = m_Values[2][0] * _matrix2.m_Values[0][1] + m_Values[2][1] * _matrix2.m_Values[1][1] + m_Values[2][2] * _matrix2.m_Values[2][1];
		result.m_Values[2][2] = m_Values[2][0] * _matrix2.m_Values[0][2] + m_Values[2][1] * _matrix2.m_Values[1][2] + m_Values[2][2] * _matrix2.m_Values[2][2];

		return result;
	}

	Matrix3 Matrix3::Multiply(const float _scalar)
	{
		return Matrix3({
			m_Values[0][0] * _scalar, m_Values[0][1] * _scalar, m_Values[0][2] * _scalar,
			m_Values[1][0] * _scalar, m_Values[1][1] * _scalar, m_Values[1][2] * _scalar,
			m_Values[2][0] * _scalar, m_Values[2][1] * _scalar, m_Values[2][2] * _scalar
			});
	}

	Vector3 Matrix3::Multiply(Vector3 _vector)
	{
		return Vector3(
			m_Values[0][0] * _vector[0] + m_Values[0][1] * _vector[1] + m_Values[0][2] * _vector[2],
			m_Values[1][0] * _vector[0] + m_Values[1][1] * _vector[1] + m_Values[1][2] * _vector[2],
			m_Values[2][0] * _vector[0] + m_Values[2][1] * _vector[1] + m_Values[2][2] * _vector[2]
		);
	}

	Matrix3 Matrix3::Divide(float _scalar)
	{
		assert(_scalar != 0.f && "The scalar is equal to zero");

		if (_scalar == 0.f)
			return *this;

		return Matrix3({
			m_Values[0][0] / _scalar, m_Values[0][1] / _scalar, m_Values[0][2] / _scalar,
			m_Values[1][0] / _scalar, m_Values[1][1] / _scalar, m_Values[1][2] / _scalar,
			m_Values[2][0] / _scalar, m_Values[2][1] / _scalar, m_Values[2][2] / _scalar
			});
	}

	const Vector3 Matrix3::Diagonal() const
	{
		return Vector3(m_Values[0][0], m_Values[1][1], m_Values[2][2]);
	}

	Matrix3 Matrix3::Transpose()
	{
		return Matrix3({
			m_Values[0][0], m_Values[1][0], m_Values[2][0],
			m_Values[0][1], m_Values[1][1], m_Values[2][1],
			m_Values[0][2], m_Values[1][2], m_Values[2][2]
			});
	}

	const float Matrix3::Trace() const
	{
		return m_Values[0][0] + m_Values[1][1] + m_Values[2][2];
	}

	const float Math::Matrix3::Determinant() const
	{
		return fabsf(m_Values[0][0] * (m_Values[1][1] * m_Values[2][2] - m_Values[2][1] * m_Values[1][2]) - m_Values[1][0] * (m_Values[0][1] * m_Values[2][2] - m_Values[2][1] * m_Values[0][2]) + m_Values[2][0] * (m_Values[0][1] * m_Values[1][2] - m_Values[1][1] * m_Values[0][2]));
	}

	Matrix3 Matrix3::Inverse()
	{
		float det = Determinant();

		if (det == 0.f)
			return *this;

		float invDet = 1.f / det;

		return Matrix3::identity;
	}

	Matrix3 Matrix3::XRotation(const float _angle)
	{
		return Matrix3({
			1.f, 0.f, 0.f,
			0.f, cosf(_angle), -sinf(_angle),
			0.f, sinf(_angle), cosf(_angle)
			});
	}

	Matrix3 Matrix3::YRotation(const float _angle)
	{
		return Matrix3({
			cosf(_angle), 0.f, sinf(_angle),
			0.f, 1.f, 0.f,
			-sinf(_angle), 0.f, cosf(_angle)
			});
	}

	Matrix3 Matrix3::ZRotation(const float _angle)
	{
		return Matrix3({
			cosf(_angle), -sinf(_angle), 0.f,
			sinf(_angle), cosf(_angle), 0.f,
			0.f, 0.f, 1.f
			});
	}

	Matrix3 Matrix3::RotationAxix(Vector3& _axis, const float _angle)
	{
		float c = cosf(_angle);
		float s = sinf(_angle);

		float axisX = _axis[0];
		float axisY = _axis[1];
		float axisZ = _axis[2];

		return Matrix3({
			(axisX * axisX) * (1.f - c) + c, axisX * axisY * (1.f - c) - axisZ * s, axisX * axisZ * (1.f - c) + axisY * s,
			axisX * axisY * (1.f - c) + _axis[2] * s, (axisY * axisY) * (1.f - c) + c, axisY * axisZ* (1.f - c) - axisX * s,
			axisX * axisY * (1.f - c) - axisY * s, axisY * axisZ* (1.f - c) + axisX * s, (axisZ * axisZ) * (1.f - c) + c
			});
	}

	Matrix3 Matrix3::operator+(const Matrix3& _matrix2)
	{
		Matrix3 result(0.f);
		result = Add(_matrix2);

		return result;
	}

	Matrix3 Matrix3::operator+(const float _scalar)
	{
		Matrix3 result(0.f);
		result = Add(_scalar);

		return result;
	}

	Matrix3& Matrix3::operator+=(const Matrix3& _matrix2)
	{
		*this = Add(_matrix2);
		return *this;
	}

	Matrix3& Matrix3::operator+=(const float _scalar)
	{
		*this = Add(_scalar);
		return *this;
	}

	Matrix3 Matrix3::operator-(Matrix3& _matrix2)
	{
		Matrix3 result(0.f);
		result = Add(_matrix2.Opposite());

		return result;
	}

	Matrix3 Matrix3::operator-(const float _scalar)
	{
		Matrix3 result(0.f);
		result = Add(-_scalar);

		return result;
	}

	Matrix3& Matrix3::operator-=(Matrix3& _matrix2)
	{
		*this = Add(_matrix2.Opposite());
		return *this;
	}

	Matrix3& Matrix3::operator-=(const float _scalar)
	{
		*this = Add(-_scalar);
		return *this;
	}

	Matrix3 Matrix3::operator*(const Matrix3& _matrix2)
	{
		Matrix3 result(0.f);
		result = Multiply(_matrix2);

		return result;
	}

	Matrix3 Matrix3::operator*(const float _scalar)
	{
		Matrix3 result(0.f);
		result = Multiply(_scalar);

		return result;
	}

	Vector3 Matrix3::operator*(const Vector3& _vector)
	{
		Vector3 result(0.f);
		result = Multiply(_vector);

		return result;
	}

	Matrix3& Matrix3::operator*=(const Matrix3& _matrix2)
	{
		*this = Multiply(_matrix2);
		return *this;
	}

	Matrix3& Matrix3::operator*=(const float _scalar)
	{
		*this = Multiply(_scalar);
		return *this;
	}

	Matrix3 Matrix3::operator/(const float _scalar)
	{
		Matrix3 result(0.f);
		result = Divide(_scalar);

		return result;
	}

	Matrix3& Matrix3::operator/=(const float _scalar)
	{
		*this = Divide(_scalar);
		return *this;
	}

	Vector3 Matrix3::operator[](const int _index)
	{
		assert(0 <= _index && _index < 3 && "Invalid index");

		switch (_index)
		{
		case 0:
			return Vector3(m_Values[0][0], m_Values[0][1], m_Values[0][2]);
			break;
		case 1:
			return Vector3(m_Values[1][0], m_Values[1][1], m_Values[1][2]);
			break;
		case 2:
			return Vector3(m_Values[2][0], m_Values[2][1], m_Values[2][2]);
			break;
		}
		return Vector3(0.f);
	}
}