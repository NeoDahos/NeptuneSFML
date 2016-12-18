#ifndef SPRITE_BATCH_H
#define SPRITE_BATCH_H
#include <NeptuneSFML\Export.h>

#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\VertexArray.hpp>
#include <SFML\System\NonCopyable.hpp>
#include <SFML\System\Sleep.hpp>

#include <vector>
//#include <thread>
//#include <condition_variable>

namespace sf
{
	class Sprite;
}

namespace nep
{
	class NEPTUNE_API SpriteBatch : public sf::NonCopyable
	{
	public:
		static SpriteBatch& GetInstance();
		//~SpriteBatch();

		template<typename T>
		void Draw(sf::RenderTarget & _target, const std::vector<T *> & _toDraw, const sf::Texture * _texture);
		void Draw(sf::RenderTarget & _target, const std::vector<sf::Sprite *> & _toDraw, const sf::Texture * _texture);

	protected:
		/*template<int ThreadCount>
		class ThreadData
		{
		public:
			// Build threads with particles to process.
			// The threads will be blocked by default at the entry of the threaded function.
			// Call signal after set restartThread to true to start a process.
			ThreadData(sf::VertexArray& _array, void(*_fct)(int _idThread, ThreadData<ThreadCount>& _data));
			~ThreadData();

			// Wake up threads to process particles.
			// Async operation.
			void Process();

			// Determine if ONE threads is still processing.
			bool IsProcessing() const;

			// Wake up the threads then tell them to quit their functions.
			// Wait for every threads had quit their functions.
			void Stop();

			// Thread functions.
			// Called in other threads.
			static void CopyToVertexArray(int _idThread, ThreadData<ThreadCount>& _data);

			sf::Sprite ** firstSprite;
			int totalSpriteCount;

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
		};*/
		// -----------------------------------------------------------------------------------------------

	private:
		//SpriteBatch();

		sf::RenderStates m_renderStates;
		sf::VertexArray m_vertices;
		//ThreadData<4> m_threadData;
	};

	template<typename T>
	void SpriteBatch::Draw(sf::RenderTarget & _target, const std::vector<T *> & _toDraw, const sf::Texture * _texture)
	{
		sf::Color color;
		sf::FloatRect bounds;
		sf::IntRect textureRect;
		sf::Vertex* currentVertex;
		T * const * currentSprite;
		const size_t spriteCount = _toDraw.size();

		if (spriteCount == 0)
			return;

		m_renderStates.texture = _texture;
		m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
		m_vertices.resize(spriteCount * 4);
		/*m_threadData.totalSpriteCount = spriteCount;
		m_threadData.firstSprite = (sf::Sprite**)(_toDraw.data());
		m_threadData.Process();
		while (m_threadData.IsProcessing())
			sf::sleep(sf::microseconds(100));*/

		currentVertex = &m_vertices[0];
		currentSprite = _toDraw.data();

		for (size_t i = 0; i < spriteCount; i++)
		{
			bounds = (*currentSprite)->getGlobalBounds();
			textureRect = (*currentSprite)->getTextureRect();
			color = (*currentSprite)->getColor();

			currentVertex->position.x = bounds.left;
			currentVertex->position.y = bounds.top;
			currentVertex->texCoords.x = static_cast<float>(textureRect.left);
			currentVertex->texCoords.y = static_cast<float>(textureRect.top);
			currentVertex->color = color;

			currentVertex++;
			currentVertex->position.x = bounds.left + bounds.width;
			currentVertex->position.y = bounds.top;
			currentVertex->texCoords.x = static_cast<float>(textureRect.left + textureRect.width);
			currentVertex->texCoords.y = static_cast<float>(textureRect.top);
			currentVertex->color = color;

			currentVertex++;
			currentVertex->position.x = bounds.left + bounds.width;
			currentVertex->position.y = bounds.top + bounds.height;
			currentVertex->texCoords.x = static_cast<float>(textureRect.left + textureRect.width);
			currentVertex->texCoords.y = static_cast<float>(textureRect.top + textureRect.height);
			currentVertex->color = color;

			currentVertex++;
			currentVertex->position.x = bounds.left;
			currentVertex->position.y = bounds.top + bounds.height;
			currentVertex->texCoords.x = static_cast<float>(textureRect.left);
			currentVertex->texCoords.y = static_cast<float>(textureRect.top + textureRect.height);
			currentVertex->color = color;

			currentVertex++;
			currentSprite++;
		}

		_target.draw(m_vertices, m_renderStates);
	}

	/*template<int ThreadCount>
	SpriteBatch::ThreadData<ThreadCount>::ThreadData(sf::VertexArray & _array,
		void(*_fct)(int _idThread, ThreadData<ThreadCount>&_data)) : varray(_array), stopThreads(false)
	{
		for (int i = 0; i < ThreadCount; i++)
		{
			restartThread[i] = false;
			threads[i] = new std::thread(_fct, i, std::ref<ThreadData<ThreadCount>>(*this));
		}
	}

	template<int ThreadCount>
	SpriteBatch::ThreadData<ThreadCount>::~ThreadData()
	{
		for (int i = 0; i < ThreadCount; i++)
			delete threads[i];
	}

	template<int ThreadCount>
	void SpriteBatch::ThreadData<ThreadCount>::Process()
	{
		// Verify waiting conditions.
		for (int i = 0; i < ThreadCount; i++)
			restartThread[i] = true;

		// Wake up threads.
		condition.notify_all();
	}

	template<int ThreadCount>
	bool SpriteBatch::ThreadData<ThreadCount>::IsProcessing() const
	{
		for (int i = 0; i < ThreadCount; i++)
			if (restartThread[i])
				return true;

		return false;
	}

	template<int ThreadCount>
	void SpriteBatch::ThreadData<ThreadCount>::Stop()
	{
		stopThreads = true;
		condition.notify_all();

		for (int i = 0; i < ThreadCount; i++)
			threads[i]->join();
	}

	template<int ThreadCount>
	void SpriteBatch::ThreadData<ThreadCount>::CopyToVertexArray(int _idThread, ThreadData<ThreadCount>& _data)
	{
		// Condition's mutex.
		std::unique_lock<std::mutex> lock(_data.mutexes[_idThread]);
		sf::Color color;
		sf::FloatRect bounds;
		sf::IntRect textureRect;
		sf::Vertex * currentVertex;
		sf::Sprite ** currentSprite;
		int spriteCountPerThread;
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
			spriteCountPerThread = _data.totalSpriteCount / ThreadCount;
			indexStart = spriteCountPerThread * _idThread;
			currentVertex = &_data.varray[indexStart];
			currentSprite = _data.firstSprite + indexStart;

			for (int i = indexStart; i < indexStart + spriteCountPerThread; i++)
			{
				bounds = (*currentSprite)->getGlobalBounds();
				textureRect = (*currentSprite)->getTextureRect();
				color = (*currentSprite)->getColor();

				currentVertex->position.x = bounds.left;
				currentVertex->position.y = bounds.top;
				currentVertex->texCoords.x = static_cast<float>(textureRect.left);
				currentVertex->texCoords.y = static_cast<float>(textureRect.top);
				currentVertex->color = color;

				currentVertex++;
				currentVertex->position.x = bounds.left + bounds.width;
				currentVertex->position.y = bounds.top;
				currentVertex->texCoords.x = static_cast<float>(textureRect.left + textureRect.width);
				currentVertex->texCoords.y = static_cast<float>(textureRect.top);
				currentVertex->color = color;

				currentVertex++;
				currentVertex->position.x = bounds.left + bounds.width;
				currentVertex->position.y = bounds.top + bounds.height;
				currentVertex->texCoords.x = static_cast<float>(textureRect.left + textureRect.width);
				currentVertex->texCoords.y = static_cast<float>(textureRect.top + textureRect.height);
				currentVertex->color = color;

				currentVertex++;
				currentVertex->position.x = bounds.left;
				currentVertex->position.y = bounds.top + bounds.height;
				currentVertex->texCoords.x = static_cast<float>(textureRect.left);
				currentVertex->texCoords.y = static_cast<float>(textureRect.top + textureRect.height);
				currentVertex->color = color;

				currentVertex++;
				currentSprite++;
			}

			// Indicate that we finished to process particles and go to the wait.
			_data.restartThread[_idThread] = false;
		}
	}*/
}

#endif // SPRITE_BATCH_H