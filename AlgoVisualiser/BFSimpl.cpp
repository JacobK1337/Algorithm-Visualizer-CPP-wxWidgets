#include "BFSimpl.h"

BFSimpl::BFSimpl(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler) : GraphAlgorithm(MAP_ROWS, MAP_COLS, handler) {

}

BFSimpl::~BFSimpl() {

}

void BFSimpl::setSource(const int& src) {
	m_source = src;
}

void BFSimpl::setDest(const int& t_newDest) {
	m_dest = t_newDest;
}

void BFSimpl::generateValues(AlgorithmThread* workingThread)
{
	for (int i = 0; i < m_MAP_ROWS; i++)
	{
		for (int j = 0; j < m_MAP_COLS; j++)
		{
			const int cellNum = i * m_MAP_COLS + j;
			animation::cellColorTransition(animation::DEFAULT_COLOR_TRANS_ONSTART, 
				cellNum, 
				"", 
				0, 
				m_parentEventHandler);
		}
	}
}

void BFSimpl::runAlgorithm(AlgorithmThread* workingThread)
{

	std::vector<cellInfo> finalPath(m_MAP_ROWS * m_MAP_COLS, { -1, -1 });
	std::vector<bool> visited(m_MAP_ROWS * m_MAP_COLS, false);
	bfs(finalPath, visited, workingThread);
}


void BFSimpl::setBlockedCells(def_type::vector1DBool& blockedCells) {
	cellBlocked = blockedCells;
}


void BFSimpl::bfs(std::vector<cellInfo>& finalPath, def_type::vector1DBool& visited, AlgorithmThread* workingThread) {
	std::queue<int> q;

	visited[m_source] = true;
	finalPath[m_source] = { m_source, 0 };
	q.push(m_source);

	while (!q.empty()) {
		int curr = q.front();
		q.pop();

		if (curr == m_dest) {
			BFSimpl::showPathToSource(finalPath, workingThread);
			return;
		}

		if (!workingThread->TestDestroy()) {
			animation::cellColorTransition(animation::DEFAULT_COLOR_TRANS_YELLOW, 
				curr, 
				std::to_string(finalPath[curr].when), 
				animation::DEFAULT_DELAY, 
				m_parentEventHandler);
		}

		else {
			workingThread->flagThreadBreak(true);
			return;
		}

		for (int x = -1; x <= 1; x++)
			for (int y = -1; y <= 1; y++) {

				int nextCellX = curr / m_MAP_COLS + x;
				int nextCellY = curr % m_MAP_COLS + y;
				int nextCellNum = nextCellX * m_MAP_COLS + nextCellY;

				if (isSafe(nextCellX, nextCellY) && !(x == 0 && y == 0) && !visited[nextCellNum]) {
					finalPath[nextCellNum].parent = curr;
					finalPath[nextCellNum].when = finalPath[curr].when + 1;
					visited[nextCellNum] = true;
					q.push(nextCellNum);
				}

			}
		
	}
}

void BFSimpl::showPathToSource(std::vector<cellInfo>& finalPath, AlgorithmThread* workingThread) {

	int temp = m_dest;

	while (temp != m_source) {

		if (!workingThread->TestDestroy()) {

			animation::cellColorTransition(animation::DEFAULT_COLOR_TRANS_GREEN, 
				temp, 
				"", 
				animation::DEFAULT_DELAY, 
				m_parentEventHandler);

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

bool BFSimpl::isSafe(const int& i, const int& j) {

	return ((i >= 0 && j >= 0) && (i < m_MAP_ROWS && j < m_MAP_COLS) && !cellBlocked[i * m_MAP_COLS + j]);

}

