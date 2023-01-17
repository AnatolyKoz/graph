#include <iostream>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>
#include <string>
#include "_DrawGraph.h"
#include "_Graph.h"
#include "ButtonMode.h"
#include "_GraphService.h"

int main() {
    sf::Font font;
    if (!font.loadFromFile("CyrilicOld.TTF"))
        return -1;


    auto graph = new _Graph<unsigned int, _DrawVertex>();
    auto drawGraph = new _Drawgraph(graph, font);
    auto buttonMode = new ButtonMode();
    auto _graphService = new _GraphService<unsigned int, _DrawVertex>();

    sf::RenderWindow window(sf::VideoMode(700, 700), "Graph");
    window.setFramerateLimit(144);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();


            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::D) {
                    unsigned int first, second;
                    std::cout << "Input first id:";
                    std::cin >> first;
                    std::cout << "Input second id:";
                    std::cin >> second;

                    std::cout << _graphService->_Dijkstra(graph, graph->getVertex(first), graph->getVertex(second)) << "\n";
                }

                if (event.key.code == sf::Keyboard::F) {
                    unsigned int first;
                    std::cout << "Input from id:";
                    std::cin >> first;

                    auto a = _graphService->DFS(graph, graph->getVertex(first));
                    for (auto& i : a)
                        std::cout << "Vertex: " << i.first->key << " dist: " << i.second << "\n";
                }

                if (event.key.code == sf::Keyboard::B) {
                    unsigned int first;
                    std::cout << "Input from id:";
                    std::cin >> first;

                    auto a = _graphService->DFS(graph, graph->getVertex(first));
                    for (auto& i : a)
                        std::cout << "Vertex: " << i.first->key << " dist: " << i.second << "\n";
                }
            }

            sf::Vector2f coords(event.mouseButton.x, event.mouseButton.y);

            if (!(drawGraph->checkAllocateVoid(coords)))
                continue;
            if (drawGraph->checkAllocateButton(coords)) {
                *buttonMode = drawGraph->alocateButtonMode(coords);
                drawGraph->selectId(0);
                continue;
            }
           

            if (event.type == sf::Event::MouseButtonPressed) {
                switch (*buttonMode) {
                    case ButtonMode::CREATE_EDGE: {
                        unsigned int key = drawGraph->checkAllocateVertex(coords);
                        if (key == 0) {
                            std::cout << "click closer\n";
                            break;
                        }
                        if (drawGraph->getSelectedKey() == 0 || drawGraph->getSelectedKey() == key) {
                            drawGraph->selectId(key);
                            break;
                        }

                        _Graph_edge<unsigned int, _DrawVertex>* edge = new  _Graph_edge<unsigned int, _DrawVertex>();
                        edge->_vertexFrom = graph->getVertex(drawGraph->getSelectedKey());
                        edge->_vertexTo = graph->getVertex(key);
                        unsigned int width;
                        std::cout << "Input width: ";
                        std::cin >> width;
                        edge->value = width;
                        graph->_Insert_edge(edge);
                        drawGraph->selectId(0);
                        break;
                    }
                    case ButtonMode::CREATE_VERTEX: {

                        if (drawGraph->checkAllocateVertex(coords) != 0) {
                            std::cout << "too close to another vertex\n";
                            break;
                        }
                        _DrawVertex _drawVertex;
                        sf::Vector2f poz(event.mouseButton.x, event.mouseButton.y);
                        _drawVertex.coords = poz;
                        graph->_Add_vertex(_drawVertex);
                        break;
                    }
                    case ButtonMode::DELEAT: {
                        unsigned int key = drawGraph->checkAllocateVertex(coords);
                        if (key == 0)
                            std::cout << "click closer\n";
                        else
                            graph->_Delate_Vertex(key);
                        break;
                    }
                                           

                }
            }
        }

        window.clear();
        window.draw(*drawGraph);
        window.display();
    }
}