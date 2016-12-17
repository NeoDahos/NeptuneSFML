#ifndef VERTEX_PARTICLE_SYSTEM_H
#define VERTEX_PARTICLE_SYSTEM_H
#include <NeptuneSFML\Export.h>

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\VertexArray.hpp>

#include <vector>
#include <thread>
#include <condition_variable>

namespace nep
{
	class VertexParticle;
	class ParticleEffector;

	class NEPTUNE_API VertexParticleSystem
	{
	public:
		VertexParticleSystem();
		~VertexParticleSystem();

		void Init(const sf::Vector2f& _position, const sf::Vector2f& _windowSize);
		void Update(float _deltaTime);
		void Draw(sf::RenderTarget& _target);

		void AddParticle(sf::Vector2f _initialForce = sf::Vector2f(), float _mass = 1.f, const sf::Color& _color = sf::Color::White);
		void AddEffector(ParticleEffector * const _effector);

		void AddForce(sf::Vector2f _force);

		size_t GetParticleCount() const;

	protected:
		template<int ThreadCount>
		class ThreadData
		{
		public:
			/**
			* Build threads with particles to process.
			* The threads will be blocked by default at the entry of the threaded function.
			* Call signal after set restartThread to true to start a process.
			*/
			ThreadData(sf::VertexArray& _array, std::vector<VertexParticle*>& _particles, std::vector<ParticleEffector*>& _effectors, void(*_fct)(int _idThread, ThreadData<ThreadCount>& _data));
			~ThreadData();

			/**
			* Wake up threads to process particles.
			* Async operation.
			*/
			void Process();

			/**
			* Determine if ONE threads is still processing.
			*/
			bool IsProcessing() const;

			/**
			* Wake up the threads then tell them to quit their functions.
			* Wait for every threads had quit their functions.
			*/
			void Stop();

			/**
			* Thread functions.
			* Called in other threads.
			*/
			static void CopyToVertexArray(int _idThread, ThreadData<ThreadCount>& _data);
			static void UpdateParticle(int _idThread, ThreadData<ThreadCount>& _data);

			float deltaTime;

		private:
			std::thread* threads[ThreadCount];
			// Waiting condtion. Allow to do the job only when it's asked in the main thread.
			bool restartThread[ThreadCount];
			// Component that allow the thread to be blocked ( and not in an active way ).
			std::condition_variable condition;
			// Mutex linked to the condition. Must be have one per thread to not be blocked in the threaded function.
			std::mutex mutexes[ThreadCount];

			// Waiting and stoping condition. Allow us to stop the threads.
			bool stopThreads;

			// Data (we are not the owner, we just keep references ).
			sf::VertexArray& varray;
			std::vector<VertexParticle*>& particles;
			std::vector<ParticleEffector *>& effectors;
		};
		// -----------------------------------------------------------------------------------------------

	private:
		std::vector<VertexParticle *> m_particles;
		std::vector<ParticleEffector *> m_effectors;
		sf::VertexArray m_vertices;
		ThreadData<4> m_threadDataCopy;
		ThreadData<4> m_threadDataUpdate;
		sf::Vector2f m_position;
		sf::Vector2f m_windowSize;
	};
}



#endif // VERTEX_PARTICLE_SYSTEM_H