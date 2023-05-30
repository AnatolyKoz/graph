#pragma once
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <boost/signals2/signal.hpp>

#include "_Graph.h"
#include "_Drawablevertex.h"
#include <vector>
#include <string>


class _Button : public sf::Drawable, public sf::Transformable {
public:
	using OnClick = boost::signals2::signal<void()>;
	using slotType = OnClick::slot_type;
	OnClick onclick;
	sf::RectangleShape _myshape;
	sf::Text _Mytext;

	
	_Button() {}
	boost::signals2::connection doOnClick(const slotType& slot)
	{
		return onclick.connect(slot);
	}

	void activate() {
		onclick();
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)  const {
		states.transform *= getTransform();
		target.draw(_myshape);
		target.draw(_Mytext);
	}
};


 class _Drawui : public sf::Drawable, public sf::Transformable {
public:
	using font_type = sf::Font;
	using color_type = sf::Color;
	using coords_type = sf::Vector2f;
	using key_type = unsigned int;


	using vertex_type = _Graph_vertex<key_type, _DrawableVertex>;
	using Vertexptr = vertex_type*;

	using edge_type = _Graph_edge<key_type, _DrawableVertex>;
	using Edgeptr = edge_type*;

	const float fieldSize_t = 700;

private:
	sf::Font _Myfont;

	color_type shapeColor = sf::Color(255, 255, 255);
	color_type selectColor = sf::Color(120, 80, 0);
	color_type outlineColor = sf::Color(172, 235, 0);
private:
	std::vector<_Button*> _Mybuttons;
	_Button* active;
public:
	_Drawui(font_type _font) {
		_Myfont = _font;
		

		std::vector<std::string> initS{ "Create vertex", "Create edge", "delete" };
		for (int i = 0; i < 3; i++) {
			sf::RectangleShape shape(sf::Vector2f((fieldSize_t - 10) / 3, 180));
			sf::Color RectangleShapeColor = sf::Color(172, 235, 0);
			shape.setOutlineThickness(2.f);
			shape.setOutlineColor(outlineColor);
			shape.setFillColor(sf::Color::Transparent);
			shape.setPosition(5 + ((fieldSize_t - 10) / 3) * (i), (fieldSize_t - 190));


			auto txt = new sf::Text();
			txt->setString(initS[i]);
			txt->setFillColor(sf::Color(112, 124, 0));
			txt->setFont(_Myfont);
			txt->setPosition(5 + ((fieldSize_t - 10) / 3) * (i), (fieldSize_t - 190));
			_Button* button = new _Button();
			button->_Mytext  = *txt;
			button->_myshape = shape;
			_Mybuttons.push_back(button);

		}
		
		
		active = _Mybuttons[0];
	}


	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)  const {
		states.transform *= getTransform();
		
		for (_Button* b : _Mybuttons) {
			target.draw(*b);
		}
	}

	bool checkAllocateButton(coords_type coords) {
		if (coords.y > fieldSize_t - 190 && coords.y < fieldSize_t)
			return true;
		return false;
	}

	void activateButton() {
		active->activate();
	}

	std::vector<_Button*>& getButtons() {
		return _Mybuttons;
	}

	void alocateButton(coords_type coords) {
		if (coords.x > 0 && coords.x < ((fieldSize_t - 10) / 3)) {
			active = _Mybuttons[0];
			return;
		}
		if ((coords.x > (fieldSize_t - 10) / 3) && coords.x < ((fieldSize_t - 10) / 3) * 2) {
			active = _Mybuttons[1];
			return;
		}

		active = _Mybuttons[2];
	}
};