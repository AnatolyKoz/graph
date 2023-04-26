#pragma once
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "_Graph.h"
#include "_Drawablevertex.h"
#include <vector>
#include <string>

class _Drawgraph : public sf::Drawable, public sf::Transformable {
public:

	using font_type   = sf::Font;
	using color_type = sf::Color;
	using coords_type = sf::Vector2f;
	using key_type = unsigned int;


	using vertex_type = _Graph_vertex<key_type, _DrawableVertex>;
	using Vertexptr = vertex_type*;

	using edge_type = _Graph_edge<key_type, _DrawableVertex>;
	using Edgeptr = edge_type*;

	const float fieldSize_t = 700;

private:
	key_type selectedKey;
	sf::Font _Myfont;

	color_type shapeColor = sf::Color(255, 255, 255);
	color_type selectColor = sf::Color(120, 80, 0);
	color_type outlineColor = sf::Color(172, 235, 0);


	_Graph<key_type, _DrawableVertex>* _Mygraph;
public:

	_Drawgraph(_Graph<key_type, _DrawableVertex>* graph, font_type _font) {
		_Myfont = _font;
		_Mygraph = graph;
		selectedKey = -1;
	}

	void _drawVertex(sf::RenderTarget& target, sf::RenderStates states, Vertexptr _vertex) const {
		sf::CircleShape* shape = new sf::CircleShape(25);

		shape->setOrigin(shape->getRadius(), shape->getRadius());
		if (_vertex->key == selectedKey)
			shape->setFillColor(selectColor);
		else
			shape->setFillColor(shapeColor);

		shape->setPosition(_vertex->value.coords);
		target.draw(*shape, states);
	
		// text
		sf::Text a;
		auto txt = new sf::Text();
		txt->setString(std::to_string(_vertex->key));

		txt->setFillColor(sf::Color(112, 124, 0));
		txt->setFont(_Myfont);
		txt->setOrigin(shape->getRadius() / 2, shape->getRadius());
		txt->setPosition(_vertex->value.coords);
		target.draw(*txt, states);
	}

	
	void _drawEdge(sf::RenderTarget& target, sf::RenderStates states, Edgeptr _edge) const {
		sf::Color lineColor = sf::Color(172, 235, 0);

		sf::Vertex line[] = {
					sf::Vertex(_edge->_vertexFrom->value.coords),
					sf::Vertex(_edge->_vertexTo->value.coords),
		};
		target.draw(line, 2, sf::Lines, states);

		// text
		auto txt = new sf::Text();
		txt->setString(std::to_string(_edge->value));

		txt->setFillColor(sf::Color(112, 124, 0));
		txt->setFont(_Myfont);
		txt->setPosition( (_edge->_vertexFrom->value.coords.x + _edge->_vertexTo->value.coords.x) / 2 , 
			(_edge->_vertexFrom->value.coords.y + _edge->_vertexTo->value.coords.y) / 2);
		target.draw(*txt, states);
		/*
		sf::CircleShape triangle(10, 3);

	
		auto x1 = _edge->_vertexTo->value.coords.x;
		auto x0 = _edge->_vertexFrom->value.coords.x;
		auto y1 = _edge->_vertexTo->value.coords.y;
		auto y0 = _edge->_vertexFrom->value.coords.y;

		
		auto c = std::sqrt(pow((x0 - x1), 2));
		auto a = std::sqrt(pow((y0 - y1), 2));
		auto gep = std::sqrt(pow(a, 2) + pow(c, 2));

		auto tan = a / c;
 		auto si = a / gep;
		auto co = c / gep;
		triangle.setFillColor(sf::Color(172, 235, 0));
		
		auto triangleGep = gep - 50;
		auto y = triangleGep * si;
		auto x = triangleGep * co;

		std::cout << x + x0 << " " << y + y0 << "\n";
		triangle.setPosition(x + x0, y + y0);
		target.draw(triangle);
		*/
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)  const {
		states.transform *= getTransform();

		for (auto& pair : _Mygraph->getEdges()) {
			for (auto edge : pair.second)
				_drawEdge(target, states, edge);
		}

		for (auto vertex : _Mygraph->getVertexes()) {
			_drawVertex(target, states, vertex.second);
		}
	}

	key_type checkAllocateVertex(coords_type coordsC) {
		for (auto& vertex : _Mygraph->getVertexes()) {
			coords_type coords = vertex.second->value.coords;

			if ((std::pow(coords.x - coordsC.x, 2) + std::pow(coords.y - coordsC.y, 2)) <= 4000)
				return vertex.second->key;
		}
		return -1;
	}

	

	bool checkAllocateVoid(coords_type coords) {
		if ((coords.x > 0 && coords.x < fieldSize_t) && (coords.y > 0 && coords.y < fieldSize_t))
			return true;
		return false;
	}

	void selectId(key_type key) {
		selectedKey = key;
	}

	key_type getSelectedKey() {
		return selectedKey;
	}
};