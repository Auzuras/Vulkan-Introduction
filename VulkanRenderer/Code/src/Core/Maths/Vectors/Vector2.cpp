#include "Vectors/Vector2.h"
#include "Utils/Utils.h"

namespace Math
{
	const Vector2 Vector2::zero(0.f);

	const Vector2 Vector2::one(1.f);

	Vector2::Vector2(float _x, float _y)
		:m_X(_x), m_Y(_y)
	{}

	Vector2::Vector2(const float _nbr)
		:m_X(_nbr), m_Y(_nbr)
	{}

	Vector2::Vector2(const Vector2 & _vector)
	{
		*this = _vector;
	}

	void Vector2::Print() const
	{
		std::cout << "( " << L_RED << m_X << END << ", " << L_GREEN << m_Y << END << " )" << std::endl;
		std::cout << std::endl;
	}

	Vector2 Vector2::Opposite()
	{
		return Vector2(-m_X, -m_Y);
	}

	Vector2 Vector2::Normalize()
	{
		float norm = Norm();

		if (norm == 0)
			return *this;

		return Vector2(m_X / norm, m_Y / norm);
	}

	Vector2 Vector2::Add(const Vector2& _vector2)
	{
		return Vector2(m_X + _vector2.m_X, m_Y + _vector2.m_Y);
	}

	Vector2 Vector2::Add(const float _scalar)
	{
		return Vector2(m_X + _scalar, m_Y + _scalar);
	}

	Vector2 Vector2::Multiply(const Vector2& _vector2)
	{
		return Vector2(m_X * _vector2.m_X, m_Y * _vector2.m_Y);
	}

	Vector2 Vector2::Multiply(const float _scalar)
	{
		return Vector2(m_X * _scalar, m_Y * _scalar);
	}

	Vector2 Vector2::Divide(float _scalar)
	{
		assert(_scalar != 0.f && "The scalar is equal to zero");

		if (_scalar == 0.f)
			return *this;

		return Vector2(m_X / _scalar, m_Y / _scalar);
	}

	const float Vector2::DotProduct(const Vector2& _vector2) const
	{
		return m_X * _vector2.m_X + m_Y * _vector2.m_Y;
	}

	const float Vector2::CrossProduct(const Vector2& _vector2) const
	{
		return m_X * _vector2.m_Y - m_Y * _vector2.m_X;
	}

	const float Vector2::Angle(Vector2& _vector2) const
	{
		float calcul = DotProduct(_vector2) / (Norm() * _vector2.Norm());

		if (calcul < -1.f)
			calcul = -1.f;
		else if (calcul > 1.f)
			calcul = 1.f;

		float angle = acosf(calcul);

		if (CrossProduct(_vector2) >= 0.f)
			return angle;
		else
			return -angle;
	}

	const float Vector2::Distance(const Vector2& _vector2) const
	{
		return sqrtf(powf(_vector2.m_X - m_X, 2) + powf(_vector2.m_Y - m_Y, 2));
	}

	const float Vector2::SquaredNorm() const
	{
		return powf(m_X, 2.f) + powf(m_Y, 2.f);
	}

	const float Vector2::Norm() const
	{
		return sqrtf(SquaredNorm());
	}

	const Vector2 Vector2::MidPoint(const Vector2& _vector2) const
	{
		return Vector2((m_X + _vector2.m_X) * 0.5f, (m_Y + _vector2.m_Y) * 0.5f);
	}

	Vector2 Vector2::Rotate(const float _angle, Vector2 _anchor)
	{
		Vector2 result(m_X, m_Y);

		float offsetX = m_X - _anchor.m_X;
		float offsetY = m_Y - _anchor.m_Y;

		result.m_X = offsetX * cosf(_angle) - offsetY * sinf(_angle) + _anchor.m_X;
		result.m_Y = offsetX * sinf(_angle) + offsetY * cosf(_angle) + _anchor.m_Y;

		return result;
	}

	Vector2 Vector2::operator+(Vector2& _vector2)
	{
		Vector2 result = Add(_vector2);
		return result;
	}

	Vector2 Vector2::operator+(float _scalar)
	{
		Vector2 result = Add(_scalar);
		return result;
	}

	Vector2& Vector2::operator+=(Vector2& _vector2)
	{
		this->Add(_vector2);
		return *this;
	}

	Vector2& Vector2::operator+=(float _scalar)
	{
		this->Add(_scalar);
		return *this;
	}

	Vector2 Vector2::operator-(Vector2& _vector2)
	{
		Vector2 result = Add(_vector2.Opposite());
		return result;
	}

	Vector2 Vector2::operator-(float _scalar)
	{
		Vector2 result = Add(-_scalar);
		return result;
	}

	Vector2& Vector2::operator-=(Vector2& _vector2)
	{
		this->Add(_vector2.Opposite());
		return *this;
	}

	Vector2& Vector2::operator-=(float _scalar)
	{
		this->Add(-_scalar);
		return *this;
	}

	Vector2 Vector2::operator*(Vector2& _vector2)
	{
		Vector2 result = Multiply(_vector2);
		return result;
	}

	Vector2 Vector2::operator*(float _scalar)
	{
		Vector2 result = Multiply(_scalar);
		return result;
	}

	Vector2& Vector2::operator*=(Vector2& _vector2)
	{
		this->Multiply(_vector2);
		return *this;
	}

	Vector2& Vector2::operator*=(float _scalar)
	{
		this->Multiply(_scalar);
		return *this;
	}

	Vector2 Vector2::operator/(float _scalar)
	{
		Vector2 result = Divide(_scalar);
		return result;
	}

	Vector2& Vector2::operator/=(float _scalar)
	{
		this->Divide(_scalar);
		return *this;
	}

	float Vector2::operator[](int _index)
	{
		assert(_index >= 0 && _index < 2 && "index invalid");

		switch (_index)
		{
		case 0:
			return m_X;
			break;
		case 1:
			return m_Y;
			break;
		}
		return 0.f;
	}
}