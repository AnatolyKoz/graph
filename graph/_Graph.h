#pragma once
#include <map>
#include <set>
#include <vector>
template<class _Kty, class _Ty>
struct _Graph_vertex {
	_Ty value;
	_Kty key;
};

template<class _Kty, class _Ty>
struct _Graph_edge {
	_Graph_vertex<_Kty, _Ty>* _vertexFrom;
	unsigned int value;
	_Graph_vertex<_Kty, _Ty>* _vertexTo;

};

template<class _Kty, class _Ty>
class _Graph {
public:
	
	using key_type = _Kty;
	using value_type = _Ty;

	using vertex_type = _Graph_vertex<key_type, value_type>;
	using _Vertexptr = vertex_type*;

	using edge_type = _Graph_edge<key_type, value_type>;
	using _Edgeptr = edge_type*;

private:
	std::map<key_type, _Vertexptr> _MyVertexptrMap;
	std::map<key_type, std::vector<_Edgeptr>> _MyEdgeMap;
	unsigned int lastId;
public:
	_Graph() {
		lastId = 1;
	}

	_Vertexptr _Insert_verex(const _Vertexptr _Newvertex) {
		_MyVertexptrMap[_Newvertex->key] = (_Newvertex);
		return _Newvertex;
	}

	_Vertexptr _Add_vertex(value_type value) {
		_Vertexptr _Newvertex = new vertex_type();
		_Newvertex->value = value;
		_Newvertex->key = lastId++;
		return _Insert_verex(_Newvertex);
	}

	 void _Delate_Vertex(key_type key) {
		_MyEdgeMap.erase(key);
		for (auto& p : _MyEdgeMap) {
			for (auto& edge : p.second) {
				if (edge->_vertexTo->key == key) {
					//p.second
				}
			}
		}
		_MyVertexptrMap.erase(key);
	}

	 _Edgeptr _Insert_edge(_Edgeptr edge) {
		 _MyEdgeMap[edge->_vertexFrom->key].push_back(edge);
		 return edge;
	 }

	 _Vertexptr getVertex(key_type _key) {
		 return _MyVertexptrMap[_key];
	 }
	
	std::map<key_type, _Vertexptr>& getVertexes() {
		return _MyVertexptrMap;
	}

	std::map<key_type, std::vector<_Edgeptr>>& getEdges() {
		return _MyEdgeMap;
	}
};