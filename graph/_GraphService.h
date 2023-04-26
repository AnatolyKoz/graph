#pragma once
#include "_Graph.h"
#include <algorithm>
#include <set>
#include <map>
#include <list>
#include <queue>
#include <stack>
template<class _Kty, class _Ty>
class _GraphService {
private:
	using key_type = _Kty;
	using value_type = _Ty;

	using vertex_type = _Graph_vertex<key_type, value_type>;
	using _Vertexptr = vertex_type*;

	using edge_type = _Graph_edge<key_type, value_type>;
	using _Edgeptr = edge_type*;

	using graph_type = _Graph<_Kty, _Ty>;
	using _Graphptr = graph_type*;

	const int infinity = 10e8;
public:

	std::vector<std::vector<int>> _Floid(_Graphptr graph) const {

		std::vector<std::vector<int>> baze;

		int graphSize = graph->getVertexes().size();
		for (int i = 0; i < graphSize; i++) {
			baze.push_back(std::vector<int>());
			for (int j = 0; j < graphSize; j++) {
				baze[i].push_back(infinity);
			}
		}
		for (auto& vertex : graph->getVertexes()) {
			baze[vertex.second->key][vertex.second->key] = 0;
		}

		for (auto& edgeV : graph->getEdges()) {
			for (auto& edge : edgeV.second) {
				baze[edge->_vertexFrom->key][edge->_vertexTo->key] = edge->value;
			}
		}

		for (int k = 0; k < graphSize; k++) {
			for (int i = 0; i < graphSize; i++) {
				for (int j = 0; j < graphSize; j++) {
					if (baze[i][j] > baze[i][k] + baze[k][j]) {
						baze[i][j] = baze[i][k] + baze[k][j];
					}
				}
			}
		}
		return baze;
	}

	int _Dijkstra(_Graphptr graph, _Vertexptr vertexFrom, _Vertexptr vertexTo) const {
		std::set<_Vertexptr> visited;
		std::list<std::pair<_Vertexptr, int>> reachable{ {vertexFrom, 0} };
		auto _Pairptr = reachable.begin();
		auto& edges = graph->getEdges();

		for (_Pairptr; _Pairptr != reachable.end(); _Pairptr++) {
			if (visited.find(_Pairptr->first) != visited.end() || _Pairptr->first == vertexTo) {
				break;
			}
			visited.insert(_Pairptr->first);
			for (auto& edge : edges[_Pairptr->first->key])
				if (!(visited.find(edge->_vertexTo) != visited.end())) {
					auto width = _Pairptr->second + (int)edge->value;
					auto a = _Pairptr;
					for (a; a != reachable.end(); std::advance(a, 1)) {
						if (a->second > width) {
							reachable.insert(a, { edge->_vertexTo, width });
							break;
						}
					}
					if (a == reachable.end())
						reachable.push_back({ edge->_vertexTo, width });
				}
		}

		_Pairptr = reachable.begin();
		for (_Pairptr; _Pairptr != reachable.end(); _Pairptr++) {
			if (_Pairptr->first == vertexTo) {
				return _Pairptr->second;
			}
		}
		return -1;
	}

	std::map<_Vertexptr, int> BFS(_Graphptr graph, _Vertexptr vertexFrom) const {
		std::queue<_Vertexptr> q;
		auto& edges = graph->getEdges();
		std::set<_Vertexptr> visited;

		std::map<_Vertexptr, int> reached{ {vertexFrom, 0} };
		q.push(vertexFrom);

		while (!q.empty()) {
			auto now = q.front();
			visited.insert(now);
			q.pop();
			for (auto& e : edges[now->key]) {
				if (!(visited.find(e->_vertexTo) != visited.end())) {
					q.push(e->_vertexTo);
					if (reached.find(e->_vertexTo) == reached.end())
						reached[e->_vertexTo] = reached[now] + e->value;
					else if (reached[e->_vertexTo] > reached[now] + e->value) {
						reached[e->_vertexTo] = reached[now] + e->value;
					}
				}
			}
		}
		return reached;
	}


	std::map<_Vertexptr, int> DFS(_Graphptr graph, _Vertexptr vertexFrom) const {
		std::stack<_Vertexptr> q;
		auto& edges = graph->getEdges();
		std::set<_Vertexptr> visited;

		std::map<_Vertexptr, int> reached{ {vertexFrom, 0} };
		q.push(vertexFrom);

		while (!q.empty()) {
			auto now = q.top();
			visited.insert(now);
			q.pop();
			for (auto& e : edges[now->key]) {
				if (!(visited.find(e->_vertexTo) != visited.end())) {
					q.push(e->_vertexTo);
					if (reached.find(e->_vertexTo) == reached.end())
						reached[e->_vertexTo] = reached[now] + e->value;
					else if (reached[e->_vertexTo] > reached[now] + e->value) {
						reached[e->_vertexTo] = reached[now] + e->value;
					}
				}
			}
		}
		return reached;

	}
};