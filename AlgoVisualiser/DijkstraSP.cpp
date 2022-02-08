#include "DijkstraSP.h"

DijkstraSP::DijkstraSP(vector<vector<int>> *costList) {
	
	numOfVertexes = (*costList).size() * (*costList)[0].size();

	adjList = new vector<vector<pair<int,int>> >(numOfVertexes, vector<pair<int,int>>());

	for(int i = 0; i < (*costList).size(); i++)
		for (int j = 0; j < (*costList)[i].size(); j++) {
			DijkstraSP::addNeighbours(i, j, (*costList));
		}
}


void DijkstraSP::runDijkstraAlgorithm(int src) {
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<> > currentDistances;

	vector<int> shortestDistance(numOfVertexes, INT_MAX);

	shortestDistance[src] = 0;

	currentDistances.push(make_pair(0, src));

	while (!currentDistances.empty()) {
		int curr = currentDistances.top().second;
		currentDistances.pop();

		//proccessing neighbours
		for (int i = 0; (*adjList)[curr].size(); i++) {

			int v = (*adjList)[curr][i].first;
			int cost = (*adjList)[curr][i].second;

			if (shortestDistance[v] > shortestDistance[curr] + cost) {
				shortestDistance[v] = shortestDistance[curr] + cost;
				currentDistances.push(make_pair(shortestDistance[v], v));
			}
		}
	}

}

void DijkstraSP::addNeighbours(int i, int j, vector<vector<int>>& costList) {

	const unsigned int MAP_ROWS = costList.size();
	const unsigned int MAP_COLS = costList[0].size();

	for (int x = -1; i <= 1; i++)
		for (int y = -1; j <= 1; j++) {
			if (!(x == 0 && y == 0) && isSafe(i + x, j + y, MAP_ROWS, MAP_COLS)) {
				int currentCellNum = i * MAP_COLS + j;
				int neighCellNum = (i + x) * MAP_COLS + (j + y);
				int neighCellCost = costList[i + x][j + y];

				(*adjList)[currentCellNum].push_back(make_pair(neighCellNum, neighCellCost));
				
			}
		}
}

bool DijkstraSP::isSafe(int i, int j, const int ROW_LIMIT, const int COL_LIMIT) {

	return ((i >= 0 && j >= 0) && (i < ROW_LIMIT && j < COL_LIMIT));
		
}


DijkstraSP::~DijkstraSP() {
	delete adjList;
}