#include "ParticlesGame.h"

#include <NeptuneSFML\Particles\VertexParticle.h>
#include <NeptuneSFML\EngineCore.h>
#include <NeptuneSFML\Tools.h>

void ParticlesGame::Init(sf::RenderWindow& _window)
{
	sf::Vector2f windowSize = static_cast<sf::Vector2f>(_window.getSize());
	m_window = &_window;
	m_isMenuOpen = false;

	m_vertexParticleSystem.Init(windowSize / 2.f, 10000);

	m_emitter.Init(&m_vertexParticleSystem, 5.f, 15.f);
	m_emitter.SetTypeContinuous(0.001f);
	m_vertexParticleSystem.AddEmitter(&m_emitter);

	m_repeller.Init(static_cast<sf::Vector2f>(_window.getSize()) / 2.f, -15.f, false);
	m_vertexParticleSystem.AddEffector(&m_repeller);

	m_colorByVelocity.Init(sf::Color(255, 255, 0, 255), sf::Color(255, 0, 0, 255), 0.f, 100.f);
	m_vertexParticleSystem.AddEffector(&m_colorByVelocity);

	m_loopZone.Init(sf::FloatRect({ 0.f, 0.f }, windowSize));
	m_vertexParticleSystem.AddEffector(&m_loopZone);

	m_lifeTime.Init(3.f);
	m_vertexParticleSystem.AddEffector(&m_lifeTime);

	//--------------------------------------------------------------------------------------------------------
	//												UI
	//--------------------------------------------------------------------------------------------------------
	m_container.SetPosition({ 10.f, 10.f });
	m_container.SetSize({ 350.f, 220.f });
	m_widgets.push_back(&m_container);

	m_container.AddChild(&m_applyBtn);
	m_applyBtn.SetRelativePosition({ 10.f, 10.f });
	m_applyBtn.SetRenderTarger(m_window);
	m_applyBtn.SetTextString("Apply");
	m_applyBtn.SetOnClickFct(std::bind(&ParticlesGame::ApplyBtn_OnClick, this, std::placeholders::_1));

	// Sliders
	m_container.AddChild(&m_particleCountSdr);
	m_particleCountSdr.SetRelativePosition({ 10.f, 80.f });
	m_particleCountSdr.SetRenderTarger(m_window);
	m_particleCountSdr.Configure(1000.f, 10000.f, 10000.f, 100000.f, 10000.f);
	m_particleCountSdr.SetOnValueChangeFct(std::bind(&ParticlesGame::Sliders_OnValueChange, this, std::placeholders::_1));

	m_container.AddChild(&m_emitterShapeSdr);
	m_emitterShapeSdr.SetRelativePosition({ 10.f, 110.f });
	m_emitterShapeSdr.SetRenderTarger(m_window);
	m_emitterShapeSdr.Configure(1.f, 1.f, 0.f, 2.f, 0.f);
	m_emitterShapeSdr.SetOnValueChangeFct(std::bind(&ParticlesGame::Sliders_OnValueChange, this, std::placeholders::_1));

	m_container.AddChild(&m_emitterTypeSdr);
	m_emitterTypeSdr.SetRelativePosition({ 10.f, 140.f });
	m_emitterTypeSdr.SetRenderTarger(m_window);
	m_emitterTypeSdr.Configure(1.f, 1.f, 0.f, 1.f, 0.f);
	m_emitterTypeSdr.SetOnValueChangeFct(std::bind(&ParticlesGame::EmitterTypeSdr_OnValueChange, this, std::placeholders::_1));

	m_container.AddChild(&m_emitterSpawnRateSdr);
	m_emitterSpawnRateSdr.SetRelativePosition({ 10.f, 170.f });
	m_emitterSpawnRateSdr.SetRenderTarger(m_window);
	m_emitterSpawnRateSdr.Configure(0.001f, 0.01f, 0.001f, 0.1f, 0.001f);
	m_emitterSpawnRateSdr.SetOnValueChangeFct(std::bind(&ParticlesGame::Sliders_OnValueChange, this, std::placeholders::_1));

	m_container.AddChild(&m_emitterPulseRateSdr);
	m_emitterPulseRateSdr.SetRelativePosition({ 10.f, 170.f });
	m_emitterPulseRateSdr.SetRenderTarger(m_window);
	m_emitterPulseRateSdr.Configure(0.1f, 0.5f, 0.1f, 10.f, 5.0f);
	m_emitterPulseRateSdr.SetOnValueChangeFct(std::bind(&ParticlesGame::Sliders_OnValueChange, this, std::placeholders::_1));
	m_emitterPulseRateSdr.SetVisiblity(false);

	m_container.AddChild(&m_emitterQuantitySdr);
	m_emitterQuantitySdr.SetRelativePosition({ 10.f, 200.f });
	m_emitterQuantitySdr.SetRenderTarger(m_window);
	m_emitterQuantitySdr.Configure(10.f, 100.f, 10.f, 1000.f, 100.f);
	m_emitterQuantitySdr.SetOnValueChangeFct(std::bind(&ParticlesGame::Sliders_OnValueChange, this, std::placeholders::_1));
	m_emitterQuantitySdr.SetVisiblity(false);

	// Labels
	m_container.AddChild(&m_particleCountLbl);
	m_particleCountLbl.SetTextString("Count : " + std::to_string(static_cast<int>(m_particleCountSdr.GetValue())));
	m_particleCountLbl.SetTextCharacterSize(15);
	m_particleCountLbl.SetRelativePosition({ 220.f, 75.f });

	m_container.AddChild(&m_emitterShapeLbl);
	m_emitterShapeLbl.SetTextString("Shape : Point");
	m_emitterShapeLbl.SetTextCharacterSize(15);
	m_emitterShapeLbl.SetRelativePosition({ 220.f, 105.f });

	m_container.AddChild(&m_emitterTypeLbl);
	m_emitterTypeLbl.SetTextString("Type : Continuous");
	m_emitterTypeLbl.SetTextCharacterSize(15);
	m_emitterTypeLbl.SetRelativePosition({ 220.f, 135.f });

	m_container.AddChild(&m_emitterSpawnRateLbl);
	std::stringstream formatStr;
	formatStr << std::fixed << std::setprecision(3) << "Spawn rate : " << m_emitterSpawnRateSdr.GetValue();
	m_emitterSpawnRateLbl.SetTextString(formatStr.str());
	m_emitterSpawnRateLbl.SetTextCharacterSize(15);
	m_emitterSpawnRateLbl.SetRelativePosition({ 220.f, 165.f });

	m_container.AddChild(&m_emitterPulseRateLbl);
	formatStr.str("");
	formatStr << std::setprecision(1) << "Pulse rate : " << m_emitterPulseRateSdr.GetValue();
	m_emitterPulseRateLbl.SetTextString(formatStr.str());
	m_emitterPulseRateLbl.SetTextCharacterSize(15);
	m_emitterPulseRateLbl.SetRelativePosition({ 220.f, 165.f });
	m_emitterPulseRateLbl.SetVisiblity(false);

	m_container.AddChild(&m_emitterQuantityLbl);
	m_emitterQuantityLbl.SetTextString("Quantity : " + std::to_string(static_cast<int>(m_emitterQuantitySdr.GetValue())));
	m_emitterQuantityLbl.SetTextCharacterSize(15);
	m_emitterQuantityLbl.SetRelativePosition({ 220.f, 195.f });
	m_emitterQuantityLbl.SetVisiblity(false);

	m_container.SetActive(m_isMenuOpen);
	m_container.SetVisiblity(m_isMenuOpen);
}

void ParticlesGame::DeInit()
{
}

void ParticlesGame::HandleEvent(const sf::Event & _event)
{
	bool isEventConsumed = false;
	for (size_t i = 0; i < m_widgets.size(); i++)
	{
		if (isEventConsumed)
			m_widgets[i]->HandleEvent(_event);
		else
			isEventConsumed = m_widgets[i]->HandleEvent(_event);
	}

	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::Space)
		{
			printf("Particle count : %d\n", m_vertexParticleSystem.GetParticleCount());
		}
		else if (_event.key.code == sf::Keyboard::M)
		{
			m_isMenuOpen = !m_isMenuOpen;
			m_container.SetActive(m_isMenuOpen);
			m_container.SetVisiblity(m_isMenuOpen);
		}
	}
	else if (_event.type == sf::Event::MouseButtonPressed && !isEventConsumed)
	{
		m_repeller.SetPosition(m_window->mapPixelToCoords({ _event.mouseButton.x, _event.mouseButton.y }));
		m_repeller.SetActive();
	}
	else if (_event.type == sf::Event::MouseButtonReleased)
	{
		m_repeller.SetActive(false);
	}
	else if (_event.type == sf::Event::MouseMoved)
	{
		m_repeller.SetPosition(m_window->mapPixelToCoords({ _event.mouseMove.x, _event.mouseMove.y }));
	}
}

void ParticlesGame::Update(float _deltaTime)
{	
	m_vertexParticleSystem.Update(_deltaTime);
}

void ParticlesGame::Draw(sf::RenderTarget& _target)
{
	m_vertexParticleSystem.Draw(_target);

	for (size_t i = 0; i < m_widgets.size(); i++)
		m_widgets[i]->Draw(_target);
}

void ParticlesGame::ApplyBtn_OnClick(sf::Event::MouseButtonEvent _buttonEvent)
{
	const int emitterShape = static_cast<int>(m_emitterShapeSdr.GetValue());
	const int emitterType = static_cast<int>(m_emitterTypeSdr.GetValue());

	if (emitterShape == 0)
		m_emitter.SetShapePoint({ 0.f, 0.f });
	else if(emitterShape == 1)
		m_emitter.SetShapeCircle({ 0.f, 0.f }, 50.f);
	else if(emitterShape == 2)
		m_emitter.SetShapeLine({ -100.f, 0.f }, { 100.f, 0.f });

	if (emitterType == 0)
		m_emitter.SetTypeContinuous(m_emitterSpawnRateSdr.GetValue());
	else if(emitterType == 1)
		m_emitter.SetTypePulse(m_emitterPulseRateSdr.GetValue(), static_cast<int>(m_emitterQuantitySdr.GetValue()));

	m_vertexParticleSystem.SetMaxParticle(static_cast<unsigned int>(m_particleCountSdr.GetValue()));
}

void ParticlesGame::Sliders_OnValueChange(float _newValue)
{
	const int emitterShape = static_cast<int>(m_emitterShapeSdr.GetValue());
	const int emitterType = static_cast<int>(m_emitterTypeSdr.GetValue());
	std::stringstream formatStr;

	m_particleCountLbl.SetTextString("Count : " + std::to_string(static_cast<int>(m_particleCountSdr.GetValue())));

	if (emitterShape == 0)
		m_emitterShapeLbl.SetTextString("Shape : Point");
	else if (emitterShape == 1)
		m_emitterShapeLbl.SetTextString("Shape : Circle");
	else if (emitterShape == 2)
		m_emitterShapeLbl.SetTextString("Shape : Line");

	if (emitterType == 0)
		m_emitterTypeLbl.SetTextString("Type : Continuous");
	else if (emitterType == 1)
		m_emitterTypeLbl.SetTextString("Type : Pulse");

	formatStr << std::fixed << std::setprecision(3) << "Spawn rate : " << m_emitterSpawnRateSdr.GetValue();
	m_emitterSpawnRateLbl.SetTextString(formatStr.str());

	formatStr.str("");
	formatStr << std::setprecision(1) << "Pulse rate : " << m_emitterPulseRateSdr.GetValue();
	m_emitterPulseRateLbl.SetTextString(formatStr.str());

	m_emitterQuantityLbl.SetTextString("Quantity : " + std::to_string(static_cast<int>(m_emitterQuantitySdr.GetValue())));
}

void ParticlesGame::EmitterTypeSdr_OnValueChange(float _newValue)
{
	Sliders_OnValueChange(_newValue);

	if (static_cast<int>(_newValue) == 0)
	{
		m_emitterSpawnRateSdr.SetVisiblity(true);
		m_emitterPulseRateSdr.SetVisiblity(false);
		m_emitterQuantitySdr.SetVisiblity(false);

		m_emitterSpawnRateLbl.SetVisiblity(true);
		m_emitterPulseRateLbl.SetVisiblity(false);
		m_emitterQuantityLbl.SetVisiblity(false);
	}
	else
	{
		m_emitterSpawnRateSdr.SetVisiblity(false);
		m_emitterPulseRateSdr.SetVisiblity(true);
		m_emitterQuantitySdr.SetVisiblity(true);

		m_emitterSpawnRateLbl.SetVisiblity(false);
		m_emitterPulseRateLbl.SetVisiblity(true);
		m_emitterQuantityLbl.SetVisiblity(true);
	}
}