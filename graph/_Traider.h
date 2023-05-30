#pragma once
#include <vector>

#define vec std::vector<std::vector<int>>


class _Traider {
private:
	const int infi = 10e9;
	int a = infi;
	int decrement(vec& matrix) {
		int w = 0;
		for (int i = 0; i < matrix.size(); i++) {
			int min = 0;
			for (int j = 0; j < matrix.size(); j++) {
				if (matrix[i][j] < matrix[i][min]) min = j;
			}
			w += matrix[i][min];
			int m = matrix[i][min];
			for (int j = 0; j < matrix.size(); j++) {
				matrix[i][j] -= m;
			}
		}

		for (int j = 0; j < matrix.size(); j++) {
			int min = 0;
			for (int i = 0; i < matrix.size(); i++) {
				if (matrix[i][j] < matrix[min][j]) min = i;
			}
			w += matrix[min][j];
			int m = matrix[min][j];

			for (int i = 0; i < matrix.size(); i++) {
				matrix[i][j] -= m;
			}
		}
		return w;
	}

	int findMIn(vec& matrix, int str, int col) {
		int minStr = infi, minColumn = infi;
		int save = matrix[str][col];
		matrix[str][col] = infi;
		for (int i = 0; i < matrix.size(); i++) {
			minStr = std::min(matrix[str][i], minStr);
			minColumn = std::min(matrix[i][col], minColumn);
		}
		matrix[str][col] = save;
		return minColumn + minStr;

	}

	void createMatrix(vec& matrix, vec& newMatrix, std::pair<int, int> chosen) {
		auto save = matrix[chosen.second][chosen.first];
		matrix[chosen.second][chosen.first] = infi;

		for (int i = 0; i < matrix.size() - 1; i++) {
			newMatrix.push_back(std::vector<int>());
		}

		for (int i = 0; i < matrix.size() - 1; i++) {
			for (int j = 0; j < matrix.size() - 1; j++)
				newMatrix[i].push_back(matrix[i >= chosen.first ? i + 1 : i][j >= chosen.second ? j + 1 : j]);
		}
		matrix[chosen.second][chosen.first] = save;
	}


	std::pair<int, std::vector<std::pair<int, int>>> cycle(vec& matrix, int w, std::vector<std::pair<int, int>> rout, std::vector<int> strHeader, std::vector<int> columnHeader) {
		if (w > a) return { w, rout };
		if (matrix.size() == 2) {
			if ( (strHeader[0] != columnHeader[1]) && (strHeader[1] != columnHeader[0])) {

				rout.push_back({ strHeader[0],columnHeader[1] });
				rout.push_back({ strHeader[1],columnHeader[0] });
			}
			else  {
				rout.push_back({ strHeader[1],columnHeader[1] });
				rout.push_back({ strHeader[0],columnHeader[0] });
			}


			a = std::min(w, a);
			return { w, rout };
		}
		std::vector<std::pair<int, int>> edges;

		for (int i = 0; i < matrix.size(); i++) {
			for (int j = 0; j < matrix.size(); j++) {
				if (matrix[i][j] == 0) {
					edges.push_back({ i, j });
				}
			}
		}

		auto maxEdgeBounty = -1;
		auto maxEdge = *edges.begin();
		for (auto& p : edges) {
			int bounty = findMIn(matrix, p.first, p.second);
			if (bounty >= maxEdgeBounty) {
				maxEdgeBounty = bounty;
				maxEdge = p;
			}
		}

		vec newMat;
		createMatrix(matrix, newMat, maxEdge);
		int go = w + decrement(newMat);
		rout.push_back({ strHeader[maxEdge.first], columnHeader[maxEdge.second] });
		std::vector<int> newStrHead;
		for (int i = 0; i < strHeader.size(); i++) {
			if (i != maxEdge.first) newStrHead.push_back(strHeader[i]);
		}
		std::vector<int> newColumnHead;
		for (int i = 0; i < columnHeader.size(); i++) {
			if (i != maxEdge.second) newColumnHead.push_back(columnHeader[i]);
		}
		auto ansG = cycle(newMat, go, rout, newStrHead, newColumnHead);

		int notGo = w + maxEdgeBounty;
		matrix[maxEdge.first][maxEdge.second] = infi;
		auto ansN = cycle(matrix, notGo, rout, strHeader, columnHeader);

		if (ansG.first > ansN.first) {
			return ansN;
		}
		return ansG;
	}
public:
	std::pair<int, std::vector<std::pair<int, int>>> voiajer(vec matrix) {
		int wBaze = decrement(matrix);
		std::vector<std::pair<int, int>> rout;
		std::vector<int> strHeader; for (int i = 0; i < matrix.size(); i++) strHeader.push_back(i);
		std::vector<int> columnHeader; for (int i = 0; i < matrix.size(); i++) columnHeader.push_back(i);
		return cycle(matrix, wBaze, rout, strHeader, columnHeader);
	}
};