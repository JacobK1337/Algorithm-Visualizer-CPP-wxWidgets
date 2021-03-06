#include "../../../include/algorithms/graph/DijkstraSP.h"

DijkstraSP::DijkstraSP(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler) : GraphAlgorithm(MAP_ROWS, MAP_COLS, handler) {

}
DijkstraSP::~DijkstraSP() {

}

void DijkstraSP::setSource(const int& t_newSource)
{
	m_source = t_newSource;
}

void DijkstraSP::setDest(const int& t_newDest) {
	m_dest = t_newDest;
}
void DijkstraSP::setBlockedCells(def_type::vector1DBool& blockedButtons) {
	cellBlocked = blockedButtons;
}


void DijkstraSP::generateValues(AlgorithmThread* workingThread)
{
	srand(time(NULL));
	costList = std::make_unique<def_type::vector2DInt>(m_MAP_ROWS, def_type::vector1DInt(m_MAP_COLS, 1));
	for (int i = 0; i < m_MAP_ROWS; i++) {
		for (int j = 0; j < m_MAP_COLS; j++) {

			const int cellNum = i * m_MAP_COLS + j;
			int randomNum = rand() % 100;
			(*costList)[i][j] = randomNum;

			animation::cellColorTransition(animation::DEFAULT_COLOR_TRANS_ONSTART, 
				cellNum, 
				std::to_string((*costList)[i][j]), 
				0, 
				m_parentEventHandler);
		}
	}

}

void DijkstraSP::runAlgorithm(AlgorithmThread* workingThread)
{

	std::vector<cellInfo> finalPath(m_MAP_ROWS * m_MAP_COLS, { -1, INT_MAX });
	DijkstraSP::dijkstra(finalPath, workingThread);
}

void DijkstraSP::dijkstra(std::vector<cellInfo>& finalPath, AlgorithmThread* workingThread) {

	//priority queue implementation of dijkstra -> fast way of getting smallest cost edge
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<> > currentDistances;
	finalPath[m_source].parent = m_source;
	finalPath[m_source].cost = 0;

	currentDistances.push(std::make_pair(0, m_source));
	while (!currentDistances.empty()) {

		int curr = currentDistances.top().second;
		currentDistances.pop();

		for (int x = -1; x <= 1; x++)
			for (int y = -1; y <= 1; y++) {

				int nextCellX = curr / m_MAP_COLS + x;
				int nextCellY = curr % m_MAP_COLS + y;
				int nextCellNum = nextCellX * m_MAP_COLS + nextCellY;
				if (isSafe(nextCellX, nextCellY) && !(x == 0 && y == 0)) {

					int currCost = (*costList)[nextCellX][nextCellY];
					if (finalPath[nextCellNum].cost > finalPath[curr].cost + currCost) {

						finalPath[nextCellNum].cost = finalPath[curr].cost + currCost;
						finalPath[nextCellNum].parent = curr;
						currentDistances.push(std::make_pair(finalPath[nextCellNum].cost, nextCellNum));

						if (!workingThread->TestDestroy() && nextCellNum != m_dest) {

							animation::cellColorTransition(animation::DEFAULT_COLOR_TRANS_YELLOW,
								nextCellNum,
								std::to_string(finalPath[nextCellNum].cost),
								animation::DEFAULT_DELAY,
								m_parentEventHandler);

						}

						else if (workingThread->TestDestroy()) {
							workingThread->flagThreadBreak(true);
							return;
						}
					}
				}
			}
	}

	DijkstraSP::showPathToSource(finalPath, workingThread);
}

void DijkstraSP::showPathToSource(std::vector<cellInfo>& finalPath, AlgorithmThread* workingThread) {

	int temp = m_dest;
	while (temp != m_source) {

		if (!workingThread->TestDestroy()) {

			animation::cellColorTransition(
				animation::DEFAULT_COLOR_TRANS_GREEN,
				temp,
				std::to_string(finalPath[temp].cost),
				animation::DEFAULT_DELAY,
				m_parentEventHandler
			);

			if (finalPath[temp].parent == -1)
				return;

			else
				temp = finalPath[temp].parent;

		}

		else {

			workingThread->flagThreadBreak(true);
			return;

		}

	}
}

bool DijkstraSP::isSafe(const int& i, const int& j)
{
	return ((i >= 0 && j >= 0) && (i < m_MAP_ROWS&& j < m_MAP_COLS) && !cellBlocked[i * m_MAP_COLS + j]);
}

