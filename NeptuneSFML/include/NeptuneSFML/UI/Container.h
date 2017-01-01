#ifndef CONTAINER_H
#define CONTAINER_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\UI\Widget.h>

#include <SFML\Graphics\RectangleShape.hpp>

namespace nep
{
	class NEPTUNE_API Container : public Widget
	{
	public:
		Container();
		virtual ~Container();
		
		virtual void SetPosition(const sf::Vector2f & _position);
		void SetSize(const sf::Vector2f & _size);

		void AddChild(Widget * _child);
		void RemoveChild(Widget * _child);
		void RemoveAllChildren();

		virtual bool HandleEvent(const sf::Event& _event);
		virtual void Draw(sf::RenderTarget& _target);

	private:
		static unsigned int  s_instanceCount;

		std::vector<Widget *> m_children;
		sf::RectangleShape m_zone;
	};
}

#endif // CONTAINER_H