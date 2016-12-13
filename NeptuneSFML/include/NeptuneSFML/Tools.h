#ifndef TOOLS_H
#define TOOLS_H
#include <NeptuneSFML\Export.h>

#include <SFML\System\Vector2.hpp>

namespace nep
{
#define PI 3.14159265359f
#define EPSILON 0.00001f
#define GRAVITY 10.f
#define SQUARE( Arg ) ( (Arg) * (Arg) )
#define DegToRad(Angle) ((Angle * PI) / 180.f)
#define RadToDeg(Angle) ((Angle * 180.f) / PI)

	NEPTUNE_API float VectorAngle(const sf::Vector2f& _vector1, const sf::Vector2f& _vector2);
	NEPTUNE_API float VectorOrientedAngle(const sf::Vector2f& _vector1, const sf::Vector2f& _vector2);
	NEPTUNE_API float CrossProduct(const sf::Vector2f& _vector1, const sf::Vector2f& _vector2);

	NEPTUNE_API float VectorLength(const sf::Vector2f& _v);
	NEPTUNE_API float VectorLengthSq(const sf::Vector2f& _v);
	NEPTUNE_API sf::Vector2f VectorNormalization(sf::Vector2f& _v);
	NEPTUNE_API void VectorNormalize(sf::Vector2f& _vector);

	template <typename T>
	T Clamp(T _value, T _min, T _max)
	{
		if (_value < _min)
			return _min;
		else if (_value > _max)
			return _max;

		return _value;
	}

	template <typename T>
	bool IsZeroByEpsilon(T _value)
	{
		return  -EPSILON <= _value && _value <= EPSILON;
	}

	template <typename T>
	bool IsEqualByEpsilon(T _value1, T _value2)
	{
		return (((_value1 - EPSILON) <= _value2) && (_value2 <= (_value1 + EPSILON)));
	}

	// Rotate vector by an angle (in Rad)
	template <class T>
	void VectorRotation(sf::Vector2<T>& _vector, const float _angle)
	{
		T X = _vector.x * cos(_angle) - _vector.y * sin(_angle);
		T Y = _vector.x * sin(_angle) + _vector.y * cos(_angle);

		_vector.x = X;
		_vector.y = Y;
	}
}

#endif // TOOLS_H