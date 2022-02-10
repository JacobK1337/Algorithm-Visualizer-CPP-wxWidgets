#include "DijkstraSP.h"

DijkstraSP::DijkstraSP(wxButton** buttons, const int VERTEX_COUNT) {

	this->VERTEX_COUNT = VERTEX_COUNT;
	mapButtons = buttons;
	adjList = new vector<vector<pair<int, int>>>(VERTEX_COUNT, vector < pair<int, int> >());

}


void DijkstraSP::runDijkstraAlgorithm(int src) {
	DijkstraSP::applyAdjList();

	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<> > currentDistances;

	vector<int> shortestDistance(VERTEX_COUNT, INT_MAX);

	shortestDistance[src] = 0;
	currentDistances.push(make_pair(0, src));
	while (!currentDistances.empty()) {
		int curr = currentDistances.top().second;
		currentDistances.pop();
		//mapButtons[curr]->SetBackgroundColour(wxColour(0, 0, 0));
		//proccessing neighbours
		for (int i = 0; i < (*adjList)[curr].size(); i++) {

			int v = (*adjList)[curr][i].first;
			int cost = (*adjList)[curr][i].second;
			
			if (shortestDistance[v] > shortestDistance[curr] + cost) {
				wxMilliSleep(100);
				shortestDistance[v] = shortestDistance[curr] + cost;
				currentDistances.push(make_pair(shortestDistance[v], v));
				mapButtons[v]->SetBackgroundColour(wxColour(204, 204, 0));
				mapButtons[v]->SetLabelText(to_string(shortestDistance[v]));
			}
		}
	}

}


void DijkstraSP::setCostList(vector<vector<int>>& costList) {
	this->costList = costList;
}


void DijkstraSP::applyAdjList() {
	for (int i = 0; i < costList.size(); i++) {
		for (int j = 0; j < costList[i].size(); j++) {
			DijkstraSP::addNeighbours(i, j);
		}
	}
}

void DijkstraSP::addNeighbours(int i, int j) {

	const unsigned int MAP_ROWS = costList.size();
	const unsigned int MAP_COLS = costList[0].size();
	for (int x = -1; x <= 1; x++)
		for (int y = -1; y <= 1; y++) {
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