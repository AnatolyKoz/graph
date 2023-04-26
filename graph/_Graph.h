#pragma once
#include <map>
#include <set>
#include <list> 

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
public:
	_Graph() {
	}

	_Vertexptr _Insert_vertex(const _Vertexptr _Newvertex) {
		_MyVertexptrMap[_Newvertex->key] = (_Newvertex);
		return _Newvertex;
	}

	_Vertexptr _Add_vertex(value_type value) {
		_Vertexptr _Newvertex = new vertex_type();
		_Newvertex->value = value;
		int freeKey = 0;
		for (auto pairs  : _MyVertexptrMap) {
			int key = pairs.first;
			if (key == freeKey) freeKey++;
			else break;
		}
		
		_Newvertex->key = freeKey;
		return _Insert_vertex(_Newvertex);
	}

	 void _Delete_Vertex(key_type key) {
		for (auto& p : _MyEdgeMap) {
			auto& edges = p.second;
			for (int i = 0; i < edges.size(); i++) {
				if (edges[i]->_vertexTo->key == key || edges[i]->_vertexFrom->key == key) {
					delete(edges[i]);
					auto edge = edges.begin();
					std::advance(edge, i);
					edges.erase(edge);
				}
			}
		}

		auto pair = _MyVertexptrMap[key];
		_MyVertexptrMap.erase(key);
		delete(pair);

	}

	 _Edgeptr _Insert_edge(_Edgeptr _Newedge) {
		 for (auto& edge : _MyEdgeMap[_Newedge->_vertexFrom->key]) {
			 if (edge->_vertexTo == _Newedge->_vertexTo) {
				 delete(edge);
				 edge = _Newedge;
				 return _Newedge;
			 }
		 }
		 _MyEdgeMap[_Newedge->_vertexFrom->key].push_back(_Newedge);
		
		 return _Newedge;
	 }

	 void _Delete_edge(const _Vertexptr _VertexFrom, const _Vertexptr _VeretxTo) {

		 auto& vect = _MyEdgeMap[_VertexFrom->key];
		 for (auto i = 0; i < vect.size(); ++i) {
			 if (vect[i]->_vertexTo == _VeretxTo) {
				 auto edge = _MyEdgeMap[_VertexFrom->key].begin();
				 std::advance(edge, i);
				 vect.erase(edge);
			 }
		 }
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