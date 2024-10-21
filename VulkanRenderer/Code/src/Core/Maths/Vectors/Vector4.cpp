#include "Vectors/Vector4.h"
#include "Utils/Utils.h"

namespace Math
{
	const Vector4 Vector4::zero(0.f);

	const Vector4 Vector4::one(1.f);

	Vector4::Vector4(const float _x, const float _y, const float _z, const float _w)
		:m_X(_x), m_Y(_y), m_Z(_z), m_W(_w)
	{}

	Vector4::Vector4(const float _nbr)
		:m_X(_nbr), m_Y(_nbr), m_Z(_nbr), m_W(_nbr)
	{}

	Vector4::Vector4(const Vector4 & _vector)
	{
		*this = _vector;
	}

	void Vector4::Print() const
	{
		std::cout << "( " << L_RED << m_X << END << ", " << L_GREEN << m_Y << END << ", " << L_BLUE << m_Z << END << ", " << L_YELLOW << m_W << END << " )" << std::endl;
		std::cout << std::endl;
	}

	Vector4 Vector4::Opposite()
	{
		return Vector4(-m_X, -m_Y, -m_Z, -m_W);
	}

	Vector4 Vector4::Normalize()
	{
		float norm = Norm();

		if (norm == 0)
			return *this;

		return Vector4(m_X / norm, m_Y / norm, m_Z / norm, m_W / norm);
	}

	Vector4 Vector4::Add(const Vector4& _vector2)
	{
		return Vector4(_vector2.m_X + m_X, _vector2.m_Y + m_Y, _vector2.m_Z + m_Z, _vector2.m_W + m_W);
	}

	Vector4 Vector4::Add(const float _scalar)
	{
		return Vector4(m_X + _scalar, m_Y + _scalar, m_Z + _scalar, m_W + _scalar);
	}

	Vector4 Vector4::Multiply(const Vector4& _vector2)
	{
		return Vector4(_vector2.m_X * m_X, _vector2.m_Y * m_Y, _vector2.m_Z * m_Z, _vector2.m_W * m_W);
	}

	Vector4 Vector4::Multiply(const float _scalar)
	{
		return Vector4(m_X * _scalar, m_Y * _scalar, m_Z * _scalar, m_W * _scalar);
	}

	Vector4 Vector4::Divide(float _scalar)
	{
		assert(_scalar != 0.f && "The scalar is equal to zero");

		if (_scalar == 0)
			return *this;

		return Vector4(m_X / _scalar, m_Y / _scalar, m_Z / _scalar, m_W / _scalar);
	}

	const float Vector4::DotProduct(const Vector4& _vector2) const
	{
		return m_X * _vector2.m_X + m_Y * _vector2.m_Y + m_Z * _vector2.m_Z + m_W * _vector2.m_W;
	}

	const float Vector4::Distance(const Vector4& _vector2) const
	{
		return sqrtf(powf(_vector2.m_X - m_X, 2) + powf(_vector2.m_Y - m_Y, 2) + powf(_vector2.m_Z - m_Z, 2) + powf(_vector2.m_W - m_W, 2));
	}

	const float Vector4::SquaredNorm() const
	{
		return powf(m_X, 2.f) + powf(m_Y, 2.f) + powf(m_Z, 2.f) + powf(m_W, 2.f);
	}
	
	const float Vector4::Norm() const
	{
		return sqrtf(SquaredNorm());
	}

	const Vector4 Vector4::MidPoint(const Vector4& _vector2) const
	{
		return Vector4((m_X + _vector2.m_X) * 0.5f, (m_Y + _vector2.m_Y) * 0.5f, (m_Z + _vector2.m_Z) * 0.5f, (m_W + _vector2.m_W) * 0.5f);
	}

	Vector4 Vector4::operator+(Vector4& _vector2)
	{
		Vector4 result = Add(_vector2);
		return result;
	}

	Vector4 Vector4::operator+(float _scalar)
	{
		Vector4 result = Add(_scalar);
		return result;
	}

	Vector4& Vector4::operator+=(Vector4& _vector2)
	{
		this->Add(_vector2);
		return *this;
	}

	Vector4& Vector4::operator+=(float _scalar)
	{
		this->Add(_scalar);
		return *this;
	}

	Vector4 Vector4::operator-(Vector4& _vector2)
	{
		Vector4 result = Add(_vector2.Opposite());
		return result;
	}

	Vector4 Vector4::operator-(float _scalar)
	{
		Vector4 result = Add(-_scalar);
		return result;
	}

	Vector4& Vector4::operator-=(Vector4& _vector2)
	{
		this->Add(_vector2.Opposite());
		return *this;
	}

	Vector4& Vector4::operator-=(float _scalar)
	{
		this->Add(-_scalar);
		return *this;
	}

	Vector4 Vector4::operator*(Vector4& _vector2)
	{
		Vector4 result = Multiply(_vector2);
		return result;
	}

	Vector4 Vector4::operator*(float _scalar)
	{
		Vector4 result = Multiply(_scalar);
		return result;
	}

	Vector4& Vector4::operator*=(Vector4& _vector2)
	{
		this->Multiply(_vector2);
		return *this;
	}

	Vector4& Vector4::operator*=(float _scalar)
	{
		this->Multiply(_scalar);
		return *this;
	}

	Vector4 Vector4::operator/(float _scalar)
	{
		Vector4 result = Divide(_scalar);
		return result;
	}

	Vector4& Vector4::operator/=(float _scalar)
	{
		this->Divide(_scalar);
		return *this;
	}

	float Vector4::operator[](int _index)
	{
		assert(_index >= 0 && _index < 4 && "index invalid");

		switch (_index)
		{
		case 0:
			return m_X;
			break;
		case 1:
			return m_Y;
			break;
		case 2:
			return m_Z;
			break;
		case 3:
			return m_W;
			break;
		}

		return 0.f;
	}
}