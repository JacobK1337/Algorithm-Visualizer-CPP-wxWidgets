#include "DijkstraSP.h"

using namespace std;
using namespace def_type;
DijkstraSP::DijkstraSP(const int t_MAP_ROWS, const int t_MAP_COLS, wxEvtHandler* handler) : parentEvtHandler(handler) {

	VERTEX_COUNT = t_MAP_ROWS * t_MAP_COLS;
	adjList = make_unique<vector2DPair>(VERTEX_COUNT, vector1DPair());

}

void DijkstraSP::setSource(const int src) {
	source = src;
}

int DijkstraSP::getSource() {
	return source;
}

void DijkstraSP::setBlockedCells(vector1DBool& blockedButtons) {
	mapButtonBlocked = blockedButtons;
}

wxThread::ExitCode DijkstraSP::runDijkstraAlgorithm(wxThread* workingThread) {
	DijkstraSP::applyAdjList();

	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<> > currentDistances;

	shortestDistance = make_unique<vector1DInt>(VERTEX_COUNT, INT_MAX);
	ancestor = make_unique<vector1DInt>(VERTEX_COUNT, -1);

	(*shortestDistance)[source] = 0;
	(*ancestor)[source] = source;

	currentDistances.push(make_pair(0, source));
	while (!currentDistances.empty()) {

		int curr = currentDistances.top().second;
		currentDistances.pop();
		for (int i = 0; i < (*adjList)[curr].size(); i++) {

			int v = (*adjList)[curr][i].first;
			int cost = (*adjList)[curr][i].second;

			if ((*shortestDistance)[v] > (*shortestDistance)[curr] + cost) {

				if (workingThread->TestDestroy()) {
					return (wxThread::ExitCode)0;
				}

				wxMilliSleep(400);
				if (!workingThread->TestDestroy()) {

					(*shortestDistance)[v] = (*shortestDistance)[curr] + cost;
					(*ancestor)[v] = curr;
					currentDistances.push(make_pair((*shortestDistance)[v], v));
					THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(v, (*shortestDistance)[v], wxColour(204, 204, 0));
					evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, parentEvtHandler, *THREAD_DATA);
				}
			}
		}
	}

	return 0;

}

const int DijkstraSP::getShortestDistance(const int FIRST_DIM_EQ) {
	return (*shortestDistance)[FIRST_DIM_EQ];
}

void DijkstraSP::showPathToSource(int t_vertexFrom) {

	int temp = t_vertexFrom;

	while (temp != source) {

		THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(temp, -1, wxColour(51, 255, 51));
		evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, parentEvtHandler, *THREAD_DATA);

		wxMilliSleep(100);

		temp = (*ancestor)[temp];
	}

}

void DijkstraSP::setCostList(vector<vector<int>>& costList) {
	this->costList = costList;
}

void DijkstraSP::incrementCellCost(const int i, const int j) {
	costList[i][j] ++;
}

int DijkstraSP::checkCellCost(const int i, const int j) {
	return costList[i][j];
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

				if (!mapButtonBlocked[currentCellNum] && !mapButtonBlocked[neighCellNum])
					(*adjList)[currentCellNum].push_back(make_pair(neighCellNum, neighCellCost));

			}
		}
}

bool DijkstraSP::isSafe(int i, int j, const int ROW_LIMIT, const int COL_LIMIT) {

	return ((i >= 0 && j >= 0) && (i < ROW_LIMIT&& j < COL_LIMIT));

}


DijkstraSP::~DijkstraSP() {
}