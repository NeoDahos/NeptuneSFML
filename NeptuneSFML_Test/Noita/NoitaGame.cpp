#include "NoitaGame.h"

#include <NeptuneSFML\EngineCore.h>
#include <NeptuneSFML\Tools.h>

void NoitaGame::Init(sf::RenderWindow& _window)
{
	m_windowRef = &_window;
	m_view = m_windowRef->getDefaultView();
	m_windowRef->setView(m_view);

	const sf::Font& defaultFont = nep::FontMng.GetFont("default");

	m_fpsText.setFont(defaultFont);
	m_fpsText.setPosition(m_windowRef->getSize().x - 180.f, 20.f);
	m_fpsText.setCharacterSize(16);

	m_simulationInfosText.setFont(defaultFont);
	m_simulationInfosText.setPosition(20.f, 0.f);
	m_simulationInfosText.setCharacterSize(16);

	m_particleToSpawnText.setFont(defaultFont);
	m_particleToSpawnText.setPosition(20.f, 50.f);
	m_particleToSpawnText.setCharacterSize(16);
	m_particleToSpawnText.setString("Particle type : Sand");
}

void NoitaGame::DeInit()
{
}

void NoitaGame::HandleEvent(const sf::Event& _event)
{
	switch (_event.type)
	{
	case sf::Event::EventType::MouseButtonPressed:
	{
		if (_event.mouseButton.button == sf::Mouse::Button::Left)
			m_isLeftMouseButtonClicked = true;
		else if (_event.mouseButton.button == sf::Mouse::Button::Right)
			m_isRightMouseButtonClicked = true;
		break;
	}
	case sf::Event::EventType::MouseButtonReleased:
		if (_event.mouseButton.button == sf::Mouse::Button::Left)
			m_isLeftMouseButtonClicked = false;
		else if (_event.mouseButton.button == sf::Mouse::Button::Right)
			m_isRightMouseButtonClicked = false;
		break;
	case sf::Event::EventType::MouseWheelScrolled:
	{
		float zoomFactor = _event.mouseWheelScroll.delta * -0.05f;
		m_currentViewZoom *= (1 - zoomFactor);
		m_view.zoom(1 + zoomFactor);
		m_windowRef->setView(m_view);
		break;
	}
	case sf::Event::EventType::KeyPressed:
		switch (_event.key.code)
		{
		case sf::Keyboard::Num1:
			m_particleTypeToSpawn = NoitaParticle::Type::Sand;
			m_particleToSpawnText.setString("Particle type : Sand");
			break;
		case sf::Keyboard::Num2:
			m_particleTypeToSpawn = NoitaParticle::Type::Water;
			m_particleToSpawnText.setString("Particle type : Water");
			break;
		case sf::Keyboard::Num3:
			m_particleTypeToSpawn = NoitaParticle::Type::Wood;
			m_particleToSpawnText.setString("Particle type : Wood");
			break;
		case sf::Keyboard::Left:
			m_viewMoveDirection.x = -1.f;
			m_isViewMoving = true;
			break;
		case sf::Keyboard::Right:
			m_viewMoveDirection.x = 1.f;
			m_isViewMoving = true;
			break;
		case  sf::Keyboard::Up:
			m_viewMoveDirection.y = -1.f;
			m_isViewMoving = true;
			break;
		case sf::Keyboard::Down:
			m_viewMoveDirection.y = 1.f;
			m_isViewMoving = true;
			break;
		case sf::Keyboard::Add:
			m_brushSize++;
			break;
		case sf::Keyboard::Subtract:
			m_brushSize--;
			if (m_brushSize < 0)
				m_brushSize = 0;
			break;
		}
		break;
	case sf::Event::EventType::KeyReleased:
		if (_event.key.code == sf::Keyboard::Left || _event.key.code == sf::Keyboard::Right)
			m_viewMoveDirection.x = 0.f;
		else if (_event.key.code == sf::Keyboard::Up || _event.key.code == sf::Keyboard::Down)
			m_viewMoveDirection.y = 0.f;

		if (m_viewMoveDirection.x == 0.f && m_viewMoveDirection.y == 0.f)
			m_isViewMoving = false;

		break;
	}
}

void NoitaGame::Update(float _deltaTime)
{
	// Input update
	if (m_isViewMoving)
	{
		m_view.move(BaseViewMoveSpeed * _deltaTime * (1.f / m_currentViewZoom) * m_viewMoveDirection);
		m_windowRef->setView(m_view);
	}

	if (m_isLeftMouseButtonClicked)
	{
		sf::Vector2i mouseWindowPosition = sf::Mouse::getPosition(*m_windowRef);
		sf::Vector2i mouseWorldPosition = sf::Vector2i(m_windowRef->mapPixelToCoords(mouseWindowPosition, m_view));
		mouseWorldPosition.y *= -1;
		SpawnParticles(mouseWorldPosition, m_particleTypeToSpawn);
	}
	else if (m_isRightMouseButtonClicked)
	{
		sf::Vector2i mouseWindowPosition = sf::Mouse::getPosition(*m_windowRef);
		sf::Vector2i mouseWorldPosition = sf::Vector2i(m_windowRef->mapPixelToCoords(mouseWindowPosition, m_view));
		mouseWorldPosition.y *= -1;
		SpawnParticles(mouseWorldPosition, NoitaParticle::Type::Empty);
	}

	m_chunkManager.Update(_deltaTime);

	// UI update
	m_timeUntilNextUIUpdate -= _deltaTime;
	if (m_timeUntilNextUIUpdate <= 0.f)
	{
		m_fpsText.setString(nep::StringFormat("%.0f FPS (%.4f ms)", (1.f / _deltaTime), (_deltaTime * 1000.f)));
		m_simulationInfosText.setString(nep::StringFormat("%i / %i particles\n%i chunks",
			m_chunkManager.GetTotalDynamicParticleCount(), m_chunkManager.GetTotalParticleCount(), m_chunkManager.GetChunksCount()));

		m_timeUntilNextUIUpdate = TimeBetweenUIUpdate;
	}
}

void NoitaGame::Draw(sf::RenderTarget& _target, const sf::RenderStates& _states)
{
	_target.setView(m_view);
	m_chunkManager.Draw(_target);

	_target.setView(_target.getDefaultView());

	_target.draw(m_fpsText);
	_target.draw(m_simulationInfosText);
	_target.draw(m_particleToSpawnText);
}

void NoitaGame::SpawnParticles(const sf::Vector2i& center, NoitaParticle::Type typeToSpawn)
{
	NoitaParticle particle;
	switch (typeToSpawn)
	{
	case NoitaParticle::Type::Sand:
		particle = NoitaParticle::SandParticle;
		break;
	case NoitaParticle::Type::Water:
		particle = NoitaParticle::WaterParticle;
		break;
	case NoitaParticle::Type::Wood:
		particle = NoitaParticle::WoodParticle;
		break;
	default:
		particle = NoitaParticle::EmptyParticle;
		break;
	}

	//SpawnParticlesInCircle(centerX, centerY, m_brushSize, particle);
	SpawnParticlesInSquare(center, m_brushSize, particle);
}

void NoitaGame::SpawnParticlesInSquare(const sf::Vector2i& center, int radius, const NoitaParticle& particleToSpawn)
{
	for (int x = -radius; x <= radius; x++)
	{
		for (int y = -radius; y <= radius; y++)
		{
			m_chunkManager.SpawnParticle(center.x + x, center.y + y, particleToSpawn);
		}
	}
}

// http://rosettacode.org/wiki/Bitmap/Midpoint_circle_algorithm#C.23
void NoitaGame::SpawnParticlesInCircle(const sf::Vector2i& center, int radius, const NoitaParticle& particleToSpawn)
{
	int d = (5 - radius * 4) / 4;
	int x = 0;
	int y = radius;

	do
	{
		m_chunkManager.SpawnParticle(center.x + x, center.y + y, particleToSpawn);
		m_chunkManager.SpawnParticle(center.x + x, center.y - y, particleToSpawn);
		m_chunkManager.SpawnParticle(center.x - x, center.y + y, particleToSpawn);
		m_chunkManager.SpawnParticle(center.x - x, center.y - y, particleToSpawn);
		m_chunkManager.SpawnParticle(center.x + y, center.y + x, particleToSpawn);
		m_chunkManager.SpawnParticle(center.x + y, center.y - x, particleToSpawn);
		m_chunkManager.SpawnParticle(center.x - y, center.y + x, particleToSpawn);
		m_chunkManager.SpawnParticle(center.x - y, center.y - x, particleToSpawn);

		if (d < 0)
			d += 2 * x + 1;
		else
		{
			d += 2 * (x - y) + 1;
			y--;
		}

		x++;
	} while (x <= y);
}
