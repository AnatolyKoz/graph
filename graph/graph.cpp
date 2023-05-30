#pragma once
#include <iostream>
#include "_Traider.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <functional>
#include "_DrawGraph.h"
#include "_Graph.h"
#include "_GraphService.h"
#include "_Drawablevertex.h"
#include "_Drawui.h"


int main() {

    // Font 
    sf::Font font;
    if (!font.loadFromFile("CyrilicOld.TTF"))
        return -1;

    // graph serv
    auto graph = new _Graph<unsigned int, _DrawableVertex>();
    auto drawGraph = new _Drawgraph(graph, font);
    auto drawui = new _Drawui(font);
    auto _graphService = new _GraphService<unsigned int, _DrawableVertex>();
    std::vector<_Button*>& buttons = drawui->getButtons();
    // SFML context
    sf::RenderWindow window(sf::VideoMode(700, 700), "Graph");
    window.setFramerateLimit(144);
    sf::Event event;
    sf::Vector2f coords;
    // funcs 
    using namespace std::placeholders;

    std::function<void(_Graph<unsigned int, _DrawableVertex>*, _Graph_vertex<unsigned int, _DrawableVertex>*, _Graph_vertex<unsigned int, _DrawableVertex>*, unsigned int)> createEdge =
        [](_Graph<unsigned int, _DrawableVertex>* graph, _Graph_vertex<unsigned int, _DrawableVertex>* _vertexFrom, _Graph_vertex<unsigned int, _DrawableVertex>* _vertexTo, unsigned int width) {
        _Graph_edge<unsigned int, _DrawableVertex>* edge = new  _Graph_edge<unsigned int, _DrawableVertex>();
        edge->_vertexFrom = _vertexFrom;
        edge->_vertexTo = _vertexTo;
        edge->value = width;
        graph->_Insert_edge(edge);
    };
    auto createE = std::bind(createEdge, _1, _2);

    std::function<void()> activateCreateE = [&](){
        unsigned int key = drawGraph->checkAllocateVertex(coords);
        if (key == -1) {
            std::cout << "click closer\n";
            return;
        }
        if (drawGraph->getSelectedKey() == -1 || drawGraph->getSelectedKey() == key) {
            drawGraph->selectId(key);
            return;
        }
        _Graph_edge<unsigned int, _DrawableVertex>* edge = new  _Graph_edge<unsigned int, _DrawableVertex>();
        edge->_vertexFrom = graph->getVertex(drawGraph->getSelectedKey());
        edge->_vertexTo = graph->getVertex(key);
        unsigned int width;
        std::cout << "Input width: ";
        std::cin >> width;
        edge->value = width;
        graph->_Insert_edge(edge);
        drawGraph->selectId(-1);
    };

    buttons[1]->doOnClick(activateCreateE);

    std::function<void(_Graph<unsigned int, _DrawableVertex>* graph, sf::Vector2f coords)> createVertex = [](_Graph<unsigned int, _DrawableVertex>* graph, sf::Vector2f coords) {
        _DrawableVertex _drawVertex;
        sf::Vector2f poz(coords);
        _drawVertex.coords = poz;
        graph->_Add_vertex(_drawVertex);
    };
    auto createV = std::bind(createVertex, graph, _1);

    std::function<void()> activateCreateV = [&]() {
        
        if (drawGraph->checkAllocateVertex(coords) != -1) {
            std::cout << "too close to another vertex\n";
            return;
        }
        createV(coords);
    };
    buttons[0]->doOnClick(activateCreateV);

    std::function<void(_Graph<unsigned int, _DrawableVertex>* graph, unsigned int key)> deleteVertex = [](_Graph<unsigned int, _DrawableVertex>* graph, unsigned int key ) {
        graph->_Delete_Vertex(key);
    };
    auto deleteV = std::bind(deleteVertex, graph, _1);

    std::function<void()> activateDelete = [&]() {
        unsigned int key = drawGraph->checkAllocateVertex(coords);
        if (key == -1)
            std::cout << "click closer\n";
        else
            deleteV(key);
    };
    buttons[2]->doOnClick(activateDelete);

   
    _Traider t;

    while (window.isOpen()) {
       
        while (window.pollEvent(event)) {
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

                    auto a = _graphService->BFS(graph, graph->getVertex(first));
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

                if (event.key.code == sf::Keyboard::R) {
                    unsigned int first, second;
                    std::cout << "Input first id:";
                    std::cin >> first;
                    std::cout << "Input second id:";
                    std::cin >> second;

                    graph->_Delete_edge(graph->getVertex(first), graph->getVertex(second));
                }

                if (event.key.code == sf::Keyboard::T) {
                    auto matrix = _graphService->_Floid(graph);
                    int inf = 10e8;
                    for (int i = 0; i < matrix.size(); i++) 
                        matrix[i][i] = inf;
                    
                    auto answ = t.voiajer(matrix);
                    for (auto& a : answ.second) {
                        std::cout << a.first << " to " << a.second << "\n";
                    }

                    std::cout << answ.first;
                    
                
                }

                if (event.key.code == sf::Keyboard::C) {
                    auto i = 0;
                    auto edgesMap = graph->getEdges();
                    std::cout << "  Key    From      To  weight" << "\n";
                    for (auto& edges : edgesMap) {
                        for (auto& edge : edges.second) {
                            const auto a = [](int digit) {
                                int i = 0;
                                for (digit; digit > 0; digit /= 10) {
                                    i += 1;
                                }
                                return i;
                            };

                            for (int j = 4 - a(i); j != 0; j--)
                                std::cout << " ";
                            std::cout << i++;

                            for (int j = 8 - a(edge->_vertexFrom->key); j != 0; j--)
                                std::cout << " ";
                            std::cout << edge->_vertexFrom->key;

                            for (int j = 8 - a(edge->_vertexTo->key); j != 0; j--)
                                std::cout << " ";
                            std::cout << edge->_vertexTo->key;

                            for (int j = 8 - a(edge->value); j != 0; j--)
                                std::cout << " ";
                            std::cout << edge->value;

                            std::cout << "\n";
                        }
                    }
                }
            }
            if (event.type == sf::Event::Closed) { 
                window.close(); }

            coords.x = event.mouseButton.x;
            coords.y = event.mouseButton.y;
            if (!(drawGraph->checkAllocateVoid(coords)))
                continue;

            if (drawui->checkAllocateButton(coords)) {
                drawui->alocateButton(coords);
                drawGraph->selectId(-1);
                continue;
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                drawui->activateButton();
                continue;

            }
        }

        window.clear();
        window.draw(*drawGraph);
        window.draw(*drawui);
        window.display();
    }
}
