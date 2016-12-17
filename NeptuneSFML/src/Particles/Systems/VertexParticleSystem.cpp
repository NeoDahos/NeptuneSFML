#include <NeptuneSFML\Particles\Systems\VertexParticleSystem.h>
#include <NeptuneSFML\Particles\Effectors\ParticleEffector.h>
#include <NeptuneSFML\Particles\VertexParticle.h>

#include <Windows.h>

namespace nep
{
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
	void nep::VertexParticleSystem::ThreadData<ThreadCount>::Stop()
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
				//_data.varray[i] = *(_data.particles[i]);
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
				//_data.particles[i]->Update(_data.deltaTime);
				(*currentParticle)->Update(_data.deltaTime);
				for (size_t j = 0; j < effectorCount; j++)
					_data.effectors[j]->Apply((*currentParticle));
					//_data.effectors[j]->Apply(_data.particles[i]);
				currentParticle++;
			}

			// Indicate that we finished to process particles and go to the wait.
			_data.restartThread[_idThread] = false;
		}
	}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	VertexParticleSystem::VertexParticleSystem() : m_vertices(sf::PrimitiveType::Points),
		m_threadDataCopy(m_vertices, m_particles, m_effectors, ThreadData<4>::CopyToVertexArray), m_threadDataUpdate(m_vertices, m_particles, m_effectors, ThreadData<4>::UpdateParticle)
	{
	}


	VertexParticleSystem::~VertexParticleSystem()
	{
		m_threadDataCopy.Stop();
		m_threadDataUpdate.Stop();

		int particleCount = static_cast<int>(m_particles.size());
		for (int i = 0; i < particleCount; i++)
			delete m_particles[i];

		m_particles.clear();
	}

	void VertexParticleSystem::Init(const sf::Vector2f& _position, const sf::Vector2f& _windowSize)
	{
		m_position = _position;
		m_windowSize = _windowSize;
	}

	void VertexParticleSystem::Update(float _deltaTime)
	{
		size_t effectorCount = m_effectors.size();
		int particleCount = static_cast<int>(m_particles.size());
		sf::Vector2f newPosition;
		m_threadDataUpdate.deltaTime = _deltaTime;

		m_threadDataUpdate.Process();
		while (m_threadDataUpdate.IsProcessing())
			Sleep(1);

		for (int i = particleCount - 1; i >= 0; i--)
		{
			if (m_particles[i]->IsDead())
			{
				delete m_particles[i];
				m_particles[i] = m_particles.back();
				m_particles.pop_back();
			}
		}
	}

	void VertexParticleSystem::Draw(sf::RenderTarget & _target)
	{
		int particleCount = static_cast<int>(m_particles.size());

		if (particleCount > 0)
		{
			m_vertices.resize(particleCount);

			m_threadDataCopy.Process();
			while (m_threadDataCopy.IsProcessing())
				Sleep(1);

			_target.draw(m_vertices);
		}
	}

	void VertexParticleSystem::AddParticle(sf::Vector2f _initialForce, float _mass, const sf::Color & _color)
	{
		VertexParticle* newParticle = new VertexParticle();
		newParticle->Init(m_position, _initialForce, _mass);
		newParticle->color = _color;
		m_particles.push_back(newParticle);
	}

	void VertexParticleSystem::AddEffector(ParticleEffector * const _effector)
	{
		m_effectors.push_back(_effector);
	}

	void VertexParticleSystem::AddForce(sf::Vector2f _force)
	{
		size_t particleCount = m_particles.size();
		for (size_t i = 0; i < particleCount; i++)
			m_particles[i]->AddForce(_force);
	}

	size_t VertexParticleSystem::GetParticleCount() const
	{
		return m_particles.size();
	}
}