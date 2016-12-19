#ifndef EMITTER_H
#define EMITTER_H
#include <NeptuneSFML\Export.h>

#include <SFML\System\Vector2.hpp>

#include <random>

namespace nep
{
	class ParticleSystem;

	class NEPTUNE_API Emitter
	{
	public:
		enum EmitterShape { Point = 0, Line, Circle };
		enum EmitterType { Continuous = 0, Pulse };
		
		//Emitter() {}

		void Init(ParticleSystem * _particleSystem, float _minInitialForce, float _maxInitialForce);
		void Update(float _deltaTime);

		void SetTypeContinuous(float _spawnRate);
		void SetTypePulse(float _pulseRate, int _quantity);

		void SetShapePoint(const sf::Vector2f & _position);
		void SetShapeLine(const sf::Vector2f & _lineStart, const sf::Vector2f & _lineEnd);
		void SetShapeCircle(const sf::Vector2f & _center, float _radius);

	private:
		void EmitPoint(int _count);
		void EmitLine(int _count);
		void EmitCircle(int _count);

		union ShapeData
		{
			struct ShapePoint
			{
				sf::Vector2f position;
			};
			struct ShapeLine
			{
				sf::Vector2f start;
				sf::Vector2f end;
			};
			struct ShapeCircle
			{
				sf::Vector2f center;
				float radius;
			};

			ShapeData() { }

			ShapePoint point;
			ShapeLine line;
			ShapeCircle circle;
		};

		union TypeData
		{
			struct // Continuous
			{
				float spawnRate;
			};
			struct // Pulse
			{
				float pulseRate;
				int quantity;
			};
		};

		std::default_random_engine m_randGenerator;
		ShapeData m_shapeData;
		TypeData m_typeData;
		float m_timeSinceLastEmission;
		float m_minInitialForce;
		float m_maxInitialForce;
		EmitterShape m_shape;
		EmitterType m_type;
		ParticleSystem * m_particleSystem;
	};
}

#endif // EMITTER_H