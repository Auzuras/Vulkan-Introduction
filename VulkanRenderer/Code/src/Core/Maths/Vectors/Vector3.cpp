#include "Vectors/Vector3.h"
#include "Matrices/Matrix3.h"
#include "Utils/Utils.h"


namespace Math
{
	const Vector3 Vector3::zero(0.f);
						
	const Vector3 Vector3::one(1.f);

	Vector3::Vector3(float _x, float _y, float _z)
		:m_X(_x), m_Y(_y), m_Z(_z)
	{}

	Vector3::Vector3(const float _nbr)
		:m_X(_nbr), m_Y(_nbr), m_Z(_nbr)
	{}

	Vector3::Vector3(const Vector3& _vector)
	{
		*this = _vector;
	}

	void Vector3::Print() const
	{
		std::cout << "( " << L_RED << m_X << END << ", " << L_GREEN << m_Y << END << ", " << L_BLUE << m_Z << END << " )" << std::endl;
		std::cout << std::endl;
	}

	Vector3 Vector3::Opposite()
	{
		return Vector3(-m_X, -m_Y, -m_Z);
	}

	Vector3 Vector3::Normalize()
	{
		float norm = Norm();
		
		if (norm == 0)
			return *this;

		return Vector3(m_X / norm, m_Y / norm, m_Z / norm);
	}

	Vector3 Vector3::Add(const Vector3& _vector2)
	{
		return Vector3(m_X + _vector2.m_X, m_Y + _vector2.m_Y, m_Z + _vector2.m_Z);
	}

	Vector3 Vector3::Add(const float _scalar)
	{
		return Vector3(m_X + _scalar, m_Y + _scalar, m_Z + _scalar);
	}

	Vector3 Vector3::Multiply(const Vector3& _vector2)
	{
		return Vector3(m_X * _vector2.m_X, m_Y * _vector2.m_Y, m_Z * _vector2.m_Z);
	}

	Vector3 Vector3::Multiply(const float _scalar)
	{
		return Vector3(m_X * _scalar, m_Y * _scalar, m_Z * _scalar);
	}

	Vector3 Vector3::Divide(float _scalar)
	{
		assert(_scalar != 0.f && "The scalar is equal to zero");

		if (_scalar == 0.f)
			return *this;

		return Vector3(m_X / _scalar, m_Y / _scalar, m_Z / _scalar);
	}

	const float Vector3::DotProduct(const Vector3& _vector2) const
	{
		return m_X * _vector2.m_X + m_Y * _vector2.m_Y + m_Z * _vector2.m_Z;
	}

	const Vector3 Vector3::CrossProduct(const Vector3& _vector2) const
	{
		return Vector3(m_Y * _vector2.m_Z - m_Z * _vector2.m_Y, m_Z * _vector2.m_X - m_X * _vector2.m_Z, m_X * _vector2.m_Y - m_Y * _vector2.m_X);
	}

	const float Vector3::Angle(Vector3& _vector2) const
	{
		float calcul = DotProduct(_vector2) / (Norm() * _vector2.Norm());

		if (calcul < -1.f)
			calcul = -1.f;
		else if (calcul > 1.f)
			calcul = 1.f;

		float angle = acosf(calcul);
	
		return angle;
	}

	const float Vector3::Distance(const Vector3& _vector2) const
	{
		return sqrtf(powf(_vector2.m_X - m_X, 2) + powf(_vector2.m_Y - m_Y, 2) + powf(_vector2.m_Z - m_Z, 2));
	}

	const float Vector3::SquaredNorm() const
	{
		return powf(m_X, 2.f) + powf(m_Y, 2.f) + powf(m_Z, 2.f);
	}

	const float Vector3::Norm() const
	{
		return sqrtf(SquaredNorm());
	}

	const Vector3 Vector3::MidPoint(const Vector3& _vector2) const
	{
		return Vector3((m_X + _vector2.m_X) * 0.5f, (m_Y + _vector2.m_Y) * 0.5f, (m_Z + _vector2.m_Z) * 0.5f);
	}

	Vector3 Vector3::Rotate(const float _angle, Vector3 _anchor, Vector3 _axis)
	{
		Vector3 result(m_X - _anchor.m_X, m_Y - _anchor.m_Y, m_Z - _anchor.m_Z);

		_axis = _axis.Normalize();

		Matrix3 axisMatrix = axisMatrix.RotationAxix(_axis, _angle);

		result = axisMatrix * result;

		result += _anchor;

		return result;
	}

	Vector3 Vector3::Rotate(const float _xAngle, const float _yAngle, const float _zAngle, Vector3 _anchor)
	{
		Vector3 result(m_X - _anchor.m_X, m_Y - _anchor.m_Y, m_Z - _anchor.m_Z);

		Matrix3 globalRotation = globalRotation.XRotation(_xAngle) * globalRotation.YRotation(_yAngle) * globalRotation.ZRotation(_zAngle);

		result = globalRotation * result;

		result += _anchor;

		return result;
	}

	Vector3 Vector3::operator+(Vector3& _vector2)
	{
		Vector3 result = Add(_vector2);
		return result;
	}

	Vector3 Vector3::operator+(float _scalar)
	{
		Vector3 result = Add(_scalar);
		return result;
	}

	Vector3& Vector3::operator+=(Vector3& _vector2)
	{
		this->Add(_vector2);
		return *this;
	}

	Vector3& Vector3::operator+=(float _scalar)
	{
		this->Add(_scalar);
		return *this;
	}

	Vector3 Vector3::operator-(Vector3& _vector2)
	{
		Vector3 result = Add(_vector2.Opposite());
		return result;
	}

	Vector3 Vector3::operator-(float _scalar)
	{
		Vector3 result = Add(-_scalar);
		return result;
	}

	Vector3& Vector3::operator-=(Vector3& _vector2)
	{
		this->Add(_vector2.Opposite());
		return *this;
	}

	Vector3& Vector3::operator-=(float _scalar)
	{
		this->Add(-_scalar);
		return *this;
	}

	Vector3 Vector3::operator*(Vector3& _vector2)
	{
		Vector3 result = Multiply(_vector2);
		return result;
	}

	Vector3 Vector3::operator*(float _scalar)
	{
		Vector3 result = Multiply(_scalar);
		return result;
	}

	Vector3& Vector3::operator*=(Vector3& _vector2)
	{
		this->Multiply(_vector2);
		return *this;
	}

	Vector3& Vector3::operator*=(float _scalar)
	{
		this->Multiply(_scalar);
		return *this;
	}

	Vector3 Vector3::operator/(float _scalar)
	{
		Vector3 result = Divide(_scalar);
		return result;
	}

	Vector3& Vector3::operator/=(float _scalar)
	{
		this->Divide(_scalar);
		return *this;
	}

	float Vector3::operator[](int _index)
	{
		assert(_index >= 0 && _index < 3 && "index invalid");

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
		}
		return 0.f;
	}
}