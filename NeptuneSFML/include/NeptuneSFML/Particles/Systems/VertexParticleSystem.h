#ifndef VERTEX_PARTICLE_SYSTEM_H
#define VERTEX_PARTICLE_SYSTEM_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\Particles\Systems\ParticleSystem.h>

#include <SFML\Graphics\VertexArray.hpp>

#include <thread>
#include <condition_variable>

namespace nep
{
	class VertexParticle;
	class ParticleEffector;

	class NEPTUNE_API VertexParticleSystem : public ParticleSystem
	{
	public:
		VertexParticleSystem();
		virtual ~VertexParticleSystem();

		void Init(const sf::Vector2f& _position, int _maxParticleCount);
		virtual void Update(float _deltaTime);
		virtual void Draw(sf::RenderTarget& _target);

		virtual void AddParticle(const sf::Vector2f & _position = sf::Vector2f(), const sf::Vector2f & _initialForce = sf::Vector2f(), float _mass = 1.f, const sf::Color & _color = sf::Color::White);

		virtual void AddForce(sf::Vector2f _force);

		virtual size_t GetParticleCount() const;

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
		sf::VertexArray m_vertices;
		ThreadData<4> m_threadDataCopy;
		ThreadData<4> m_threadDataUpdate;
		int m_maxParticleCount;
	};

	template<int ThreadCount>
	VertexParticleSystem::ThreadData<ThreadCount>::ThreadData(sf::VertexArray & _array, std::vector<VertexParticle*>& _particles, std::vector<ParticleEffector*>& _effectors,
		void(*_fct)(int _idThread, ThreadData<ThreadCount>&_data)) : varray(_array), particles(_particles), effectors(_effectors), stopThreads(false)
	{
		for (int i = 0; i < ThreadCount; i++)
		{
			restartThread[i] = false;
			threads[i] = new std::thread(_fct, i, std::ref<ThreadData<ThreadCount>>(*this));
		}
	}

	template<int ThreadCount>
	VertexParticleSystem::ThreadData<ThreadCount>::~ThreadData()
	{
		for (int i = 0; i < ThreadCount; i++)
			delete threads[i];
	}

	template<int ThreadCount>
	void VertexParticleSystem::ThreadData<ThreadCount>::Process()
	{
		// Verify waiting conditions.
		for (int i = 0; i < ThreadCount; i++)
			restartThread[i] = true;

		// Wake up threads.
		condition.notify_all();
	}

	template<int ThreadCount>
	bool VertexParticleSystem::ThreadData<ThreadCount>::IsProcessing() const
	{
		for (int i = 0; i < ThreadCount; i++)
			if (restartThread[i])
				return true;

		return false;
	}

	template<int ThreadCount>
	void VertexParticleSystem::ThreadData<ThreadCount>::Stop()
	{
		stopThreads = true;
		condition.notify_all();

		for (int i = 0; i < ThreadCount; i++)
			threads[i]->join();
	}

	template<int ThreadCount>
	void VertexParticleSystem::ThreadData<ThreadCount>::CopyToVertexArray(int _idThread, ThreadData<ThreadCount>& _data)
	{
		// Condition's mutex.
		std::unique_lock<std::mutex> lock(_data.mutexes[_idThread]);
		VertexParticle** currentParticle = nullptr;
		int particleCountPerThread;
		int indexStart;

		while (1)
		{
			// Waiting condition.
			while (!_data.restartThread[_idThread] && !_data.stopThreads)
				_data.condition.wait(lock);

			// If asked to quit thread, exit of the function.
			if (_data.stopThreads)
				return;

			// Otherwise process the particles.
			particleCountPerThread = _data.particles.size() / ThreadCount;
			indexStart = particleCountPerThread * _idThread;
			currentParticle = _data.particles.data() + indexStart;

			for (int i = indexStart; i < indexStart + particleCountPerThread; i++)
			{
				_data.varray[i] = **currentParticle;
				currentParticle++;
			}

			// Indicate that we finished to process particles and go to the wait.
			_data.restartThread[_idThread] = false;
		}
	}

	template<int ThreadCount>
	void VertexParticleSystem::ThreadData<ThreadCount>::UpdateParticle(int _idThread, ThreadData& _data)
	{
		// Condition's mutex.
		std::unique_lock<std::mutex> lock(_data.mutexes[_idThread]);
		VertexParticle** currentParticle = nullptr;
		size_t effectorCount;
		int particleCountPerThread;
		int indexStart;

		while (1)
		{
			// Waiting condition.
			while (!_data.restartThread[_idThread] && !_data.stopThreads)
				_data.condition.wait(lock);

			// If asked to quit thread, exit of the function.
			if (_data.stopThreads)
				return;

			// Otherwise process the particles.
			effectorCount = _data.effectors.size();
			particleCountPerThread = _data.particles.size() / ThreadCount;
			indexStart = particleCountPerThread * _idThread;
			currentParticle = _data.particles.data() + indexStart;

			for (int i = indexStart; i < indexStart + particleCountPerThread; i++)
			{
				(*currentParticle)->Update(_data.deltaTime);
				for (size_t j = 0; j < effectorCount; j++)
					_data.effectors[j]->Apply((*currentParticle));
				currentParticle++;
			}

			// Indicate that we finished to process particles and go to the wait.
			_data.restartThread[_idThread] = false;
		}
	}
}

#endif // VERTEX_PARTICLE_SYSTEM_H