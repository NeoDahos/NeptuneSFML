#ifndef TOOLS_H
#define TOOLS_H
#include <NeptuneSFML\Export.h>

#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Color.hpp>

#include <string>
#include <random>


namespace nep
{
	extern NEPTUNE_API std::default_random_engine m_rand_generator;
	extern NEPTUNE_API std::uniform_real_distribution<float> m_rand_realDistribution;

	constexpr const float PI = 3.14159265359f;
	constexpr const float PI2 = PI / 2.f;
	constexpr const float TWO_PI = 2.f * PI;
	constexpr const float EPSILON = 0.00001f;

	NEPTUNE_API sf::Color LerpColor(const sf::Color& _min, const sf::Color& _max, float _time);

	NEPTUNE_API float GetRandomValue();
	NEPTUNE_API int GetRandomValue(int min, int max);
	NEPTUNE_API std::string OpenFilePicker();

	template <typename T>
	float VectorAngle(const sf::Vector2<T>& _vector1, const sf::Vector2<T>& _vector2)
	{
		float scalarProduct = _vector1.x * _vector2.x + _vector1.y * _vector2.y;
		float lengthV1 = sqrtf(_vector1.x * _vector1.x + _vector1.y * _vector1.y);
		float lengthV2 = sqrtf(_vector2.x * _vector2.x + _vector2.y * _vector2.y);

		return (acosf(scalarProduct / (lengthV1 * lengthV2)) * 180.f) / PI;
	}

	template <typename T>
	float VectorOrientedAngle(const sf::Vector2<T>& _vector1, const sf::Vector2<T>& _vector2)
	{
		float angle = VectorAngle(_vector1, _vector2);

		if (CrossProduct(_vector1, _vector2) > 0)
			return angle;
		else
			return -angle;
	}

	// Rotate vector by an angle (in Rad)
	template <class T>
	void VectorRotation(sf::Vector2<T>& _vector, float _angle)
	{
		T X = _vector.x * cos(_angle) - _vector.y * sin(_angle);
		T Y = _vector.x * sin(_angle) + _vector.y * cos(_angle);

		_vector.x = X;
		_vector.y = Y;
	}

	template <typename T>
	float CrossProduct(const sf::Vector2<T>& _vector1, const sf::Vector2<T>& _vector2)
	{
		return (_vector1.x * _vector2.y) - (_vector1.y * _vector2.x);
	}

	template <typename T>
	float DotProduct(const sf::Vector2<T>& _vector1, const sf::Vector2<T>& _vector2)
	{
		return (_vector1.x * _vector2.x) + (_vector1.y * _vector2.y);
	}

	template <typename T>
	float VectorLength(const sf::Vector2<T>& _v)
	{
		return sqrtf(Square(_v.x) + Square(_v.y));
	}

	template <typename T>
	float VectorLengthSq(const sf::Vector2<T>& _v)
	{
		return Square(_v.x) + Square(_v.y);
	}

	template <typename T>
	sf::Vector2<T> VectorNormalization(const sf::Vector2<T>& _v)
	{
		float length = VectorLength(_v);

		if (length != 0.f)
			return sf::Vector2<T>(_v.x / length, _v.y / length);
		else
			return _v;
	}

	template <typename T>
	void VectorNormalize(sf::Vector2<T>& _vector)
	{
		_vector = VectorNormalization(_vector);
	}

	template<class T>
	constexpr const T& Clamp(const T& _value, const T& _min, const T& _max)
	{
		return (_value < _min) ? _min : (_max < _value) ? _max : _value;
	}

	template<typename T>
	T Lerp(T _min, T _max, float _time)
	{
		return static_cast<T>(_min + _time * (_max - _min));
	}

	template <typename T>
	constexpr bool IsZeroByEpsilon(const T& _value)
	{
		return -EPSILON <= _value && _value <= EPSILON;
	}

	template <typename T>
	constexpr bool IsEqualByEpsilon(const T& _value1, const T& _value2)
	{
		return IsZeroByEpsilon(_value1 - _value2);
	}

	template<class T>
	constexpr const T Square(const T& _value)
	{
		return _value * _value;
	};

	template<typename T>
	constexpr const T DegToRad(const T& _angle)
	{
		return static_cast<T>((_angle * nep::PI) / 180.f);
	}

	template<typename T>
	constexpr const T RadToDeg(const T& _angle)
	{
		return static_cast<T>((_angle * 180.f) / nep::PI);
	}

	// Thanks https://stackoverflow.com/a/26221725
	template<typename ... Args>
	std::string StringFormat(const std::string& format, Args ... args)
	{
		int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
		if (size_s <= 0)
			throw std::runtime_error("Error during formatting.");

		auto size = static_cast<size_t>(size_s);
		auto buf = std::make_unique<char[]>(size);
		std::snprintf(buf.get(), size, format.c_str(), args ...);
		return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
	}
}

#endif // TOOLS_H