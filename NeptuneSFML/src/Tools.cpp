#include <NeptuneSFML\Tools.h>

#include <math.h>

namespace nep
{
	float VectorAngle(const sf::Vector2f& _vector1, const sf::Vector2f& _vector2)
	{
		float scalarProduct = _vector1.x * _vector2.x + _vector1.y * _vector2.y;
		float lengthV1 = sqrtf(_vector1.x * _vector1.x + _vector1.y * _vector1.y);
		float lengthV2 = sqrtf(_vector2.x * _vector2.x + _vector2.y * _vector2.y);

		return (acosf(scalarProduct / (lengthV1 * lengthV2)) * 180.f) / PI;
	}

	float VectorOrientedAngle(const sf::Vector2f& _vector1, const sf::Vector2f& _vector2)
	{
		float angle = VectorAngle(_vector1, _vector2);

		if (CrossProduct(_vector1, _vector2) > 0)
			return angle;
		else
			return -angle;
	}

	float CrossProduct(const sf::Vector2f& _vector1, const sf::Vector2f& _vector2)
	{
		return (_vector1.x * _vector2.y) - (_vector1.y * _vector2.x);
	}

	float VectorLength(const sf::Vector2f& _v)
	{
		return sqrtf(SQUARE(_v.x) + SQUARE(_v.y));
	}

	float VectorLengthSq(const sf::Vector2f& _v)
	{
		return SQUARE(_v.x) + SQUARE(_v.y);
	}

	sf::Vector2f VectorNormalization(sf::Vector2f& _v)
	{
		float length = VectorLength(_v);
		return sf::Vector2f(_v.x / length, _v.y / length);
	}

	void VectorNormalize(sf::Vector2f& _vector)
	{
		float length = VectorLength(_vector);

		if (length != 0.f)
		{
			_vector.x /= length;
			_vector.y /= length;
		}
	}
}