#include <NeptuneSFML\UI\Container.h>

namespace nep
{
	unsigned int Container::s_instanceCount = 0;

	Container::Container() : Widget()
	{
		s_instanceCount++;
		m_name = "Container" + std::to_string(s_instanceCount);

		m_zone.setFillColor(sf::Color(40, 40, 40));
		m_zone.setOutlineThickness(2);
		m_zone.setOutlineColor(sf::Color(20, 20, 20));
		m_zone.setSize({ 100.f, 100.f });
	}

	Container::~Container()
	{
		s_instanceCount--;
	}

	void Container::SetPosition(const sf::Vector2f & _position)
	{
		const size_t childrenCount = m_children.size();
		const sf::Vector2f offset = _position - m_position;
		m_position = _position;
		m_zone.move(offset);

		for (size_t i = 0; i < childrenCount; i++)
			m_children[i]->SetPosition(m_children[i]->GetPosition() + offset);
	}

	void Container::SetSize(const sf::Vector2f & _size)
	{
		m_zone.setSize(_size);
	}

	void Container::AddChild(Widget * _child)
	{
		if (std::find(m_children.begin(), m_children.end(), _child) == m_children.end())
		{
			m_children.push_back(_child);
			_child->SetParent(this);
		}
	}

	void Container::RemoveChild(Widget * _child)
	{
		const size_t childrenCount = m_children.size();
		for (size_t i = 0; i < childrenCount; i++)
		{
			if (m_children[i] == _child)
			{
				m_children[i]->SetParent(nullptr);
				delete m_children[i];
				m_children[i] = m_children[childrenCount];
				m_children.pop_back();
				break;
			}
		}
	}

	void Container::RemoveAllChildren()
	{
		const size_t childrenCount = m_children.size();
		for (size_t i = 0; i < childrenCount; i++)
		{
			m_children[i]->SetParent(nullptr);
			delete m_children[i];
		}

		m_children.clear();
	}

	bool Container::HandleEvent(const sf::Event & _event)
	{
		bool isEventConsumed = false;
		if (m_state != WidgetState::Inactive)
		{
			const size_t childrenCount = m_children.size();

			for (size_t i = 0; i < childrenCount; i++)
			{
				if (isEventConsumed)
					m_children[i]->HandleEvent(_event);
				else
					isEventConsumed = m_children[i]->HandleEvent(_event);
			}

			if (_event.type == sf::Event::MouseMoved)
			{
				const bool mouseIn = m_renderTarget ?
					m_zone.getGlobalBounds().contains(m_renderTarget->mapPixelToCoords({ _event.mouseMove.x, _event.mouseMove.y })) :
					m_zone.getGlobalBounds().contains(static_cast<float>(_event.mouseMove.x), static_cast<float>(_event.mouseMove.y));

				if (mouseIn && !m_isMouseIn)
				{
					HandleMouseEnter(_event.mouseMove.x, _event.mouseMove.y);
				}
				else if (!mouseIn && m_isMouseIn)
				{
					HandleMouseLeave(_event.mouseMove.x, _event.mouseMove.y);
				}
			}
			else if (_event.type == sf::Event::MouseButtonPressed)
			{
				if (m_isMouseIn)
				{
					HandleMouseButtonEvent(_event.mouseButton.button, true, _event.mouseButton.x, _event.mouseButton.y);
					isEventConsumed = true;
				}
			}
		}

		return isEventConsumed;
	}

	void Container::Draw(sf::RenderTarget & _target)
	{
		if (m_isVisible)
		{
			const size_t childrenCount = m_children.size();
			_target.draw(m_zone);
			
			for (size_t i = 0; i < childrenCount; i++)
				m_children[i]->Draw(_target);
		}
	}
}