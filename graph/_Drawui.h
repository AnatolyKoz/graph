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


struct _Button {
	boost::signals2::signal<void()> signal;
	std::string label;
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
	_Graph<key_type, _DrawableVertex>* _Mygraph;
	std::vector<_Button*> _Mybuttons;
	_Button* active;
public:
	_Drawui(_Graph<key_type, _DrawableVertex>* graph, font_type _font) {
		_Myfont = _font;
		_Mygraph = graph;
		boost::signals2::signal<void()> signal1;
		_createButton("Create vertex", std::move(signal1));

		boost::signals2::signal<void()> signal2;
		_createButton("Create Edge", std::move(signal2));

		boost::signals2::signal<void()> signal3;
		_createButton("Delete", std::move(signal3));

		active = _Mybuttons[0];
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)  const {
		states.transform *= getTransform();
		_drawButton(target, states);
	}

	void _drawButton(sf::RenderTarget& target, sf::RenderStates states) const {
		sf::Color RectangleShapeColor = sf::Color(172, 235, 0);
		sf::RectangleShape shape(sf::Vector2f((fieldSize_t - 10) / 3, 180));
		shape.setOutlineThickness(2.f);
		shape.setOutlineColor(outlineColor);
		shape.setFillColor(sf::Color::Transparent);

		int i = 0;
		for (auto button : _Mybuttons) {
			shape.setPosition(5 + ((fieldSize_t - 10) / 3) * (i), (fieldSize_t - 190));
			target.draw(shape, states);

			auto txt = new sf::Text();
			txt->setString(_Mybuttons[i]->label);

			txt->setFillColor(sf::Color(112, 124, 0));
			txt->setFont(_Myfont);
			txt->setPosition(5 + ((fieldSize_t - 10) / 3) * (i), (fieldSize_t - 190));
			target.draw(*txt, states);
			i++;
		}
	}

	void _createButton(std::string label, boost::signals2::signal<void()> signal) {
		auto button = new _Button();
		button->label = label;
		button->signal = std::move(signal);
		_Mybuttons.push_back(button);
	}

	bool checkAllocateButton(coords_type coords) {
		if (coords.y > fieldSize_t - 190 && coords.y < fieldSize_t)
			return true;
		return false;
	}

	void activateButton() {
		active->signal();
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