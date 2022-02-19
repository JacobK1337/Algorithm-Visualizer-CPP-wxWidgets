#include "DijkstraSP.h"

using namespace std;
using namespace def_type;
DijkstraSP::DijkstraSP(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler) : GraphAlgorithm(MAP_ROWS, MAP_COLS, handler) {

	adjList = make_unique<vector2DPair>(m_MAP_ROWS * m_MAP_COLS, vector1DPair());

}
DijkstraSP::~DijkstraSP() {

}

void DijkstraSP::setSource(const int& t_newSource)
{
	m_source = t_newSource;
}

int DijkstraSP::getSource() {
	return m_source;
}

void DijkstraSP::setBlockedCells(vector1DBool& blockedButtons) {
	mapButtonBlocked = blockedButtons;
}


void DijkstraSP::generateValues(AlgorithmThread* workingThread)
{
	srand(time(NULL));
	costList = std::make_unique<def_type::vector2DInt>(m_MAP_ROWS, def_type::vector1DInt(m_MAP_COLS, 1));
	for (int i = 0; i < m_MAP_ROWS; i++) {
		for (int j = 0; j < m_MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * m_MAP_COLS + j;

			int randomNum = rand() % 100;
			(*costList)[i][j] = randomNum;

			if (FIRST_DIM_EQ != m_source) {
				THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(FIRST_DIM_EQ, (*costList)[i][j], wxColour(255, 255, 255));
				evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);
			}

		}
	}

}

void DijkstraSP::runAlgorithm(AlgorithmThread* workingThread)
{
	DijkstraSP::applyAdjList();
	DijkstraSP::dijkstra(workingThread);
	
}

void DijkstraSP::dijkstra(AlgorithmThread* workingThread) {

	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<> > currentDistances;

	shortestDistance = make_unique<vector1DInt>(m_MAP_ROWS * m_MAP_COLS, INT_MAX);
	ancestor = make_unique<vector1DInt>(m_MAP_ROWS * m_MAP_COLS, -1);

	(*shortestDistance)[m_source] = 0;
	(*ancestor)[m_source] = m_source;

	currentDistances.push(make_pair(0, m_source));
	while (!currentDistances.empty()) {

		int curr = currentDistances.top().second;
		currentDistances.pop();
		for (int i = 0; i < (*adjList)[curr].size(); i++) {

			int v = (*adjList)[curr][i].first;
			int cost = (*adjList)[curr][i].second;

			if ((*shortestDistance)[v] > (*shortestDistance)[curr] + cost) {

				if (workingThread->TestDestroy()) {

					workingThread->flagThreadBreak(true);
					return;

				}

				if (!workingThread->TestDestroy()) {

					(*shortestDistance)[v] = (*shortestDistance)[curr] + cost;
					(*ancestor)[v] = curr;
					currentDistances.push(make_pair((*shortestDistance)[v], v));
					THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(v, (*shortestDistance)[v], wxColour(204, 204, 0));
					evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);
					wxMilliSleep(100);

				}

			}
		}
	}
}

void DijkstraSP::showPathToSource(const int& t_vertexFrom, AlgorithmThread* workingThread)
{
	int temp = t_vertexFrom;

	while (temp != m_source) {

		if (!workingThread->TestDestroy()) {

			THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(temp, -1, wxColour(51, 255, 51));
			evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);
			temp = (*ancestor)[temp];

			wxMilliSleep(100);
		}

		else {

			workingThread->flagThreadBreak(true);
			return;

		}

	}
}

void DijkstraSP::addNeighbours(const int& i, const int& j)
{
	const unsigned int MAP_ROWS = (*costList).size();
	const unsigned int MAP_COLS = (*costList)[0].size();
	for (int x = -1; x <= 1; x++)
		for (int y = -1; y <= 1; y++) {
			if (!(x == 0 && y == 0) && isSafe(i + x, j + y)) {
				int currentCellNum = i * MAP_COLS + j;
				int neighCellNum = (i + x) * MAP_COLS + (j + y);
				int neighCellCost = (*costList)[i + x][j + y];

				if (!mapButtonBlocked[currentCellNum] && !mapButtonBlocked[neighCellNum])
					(*adjList)[currentCellNum].push_back(make_pair(neighCellNum, neighCellCost));

			}
		}
}

bool DijkstraSP::isSafe(const int& i, const int& j)
{
	return ((i >= 0 && j >= 0) && (i < m_MAP_ROWS&& j < m_MAP_COLS));
}

const int DijkstraSP::getShortestDistance(const int FIRST_DIM_EQ) {
	return (*shortestDistance)[FIRST_DIM_EQ];
}

/*
void DijkstraSP::setCostList(vector<vector<int>>& costList) {
	this->costList = costList;
}

void DijkstraSP::incrementCellCost(const int i, const int j) {
	costList[i][j] ++;
}

int DijkstraSP::checkCellCost(const int i, const int j) {
	return costList[i][j];
}
*/

void DijkstraSP::applyAdjList() {
	for (int i = 0; i < (*costList).size(); i++) {
		for (int j = 0; j < (*costList)[i].size(); j++) {
			DijkstraSP::addNeighbours(i, j);
		}
	}
}
