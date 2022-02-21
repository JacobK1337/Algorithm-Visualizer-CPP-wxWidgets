#include "BFSimpl.h"

using namespace std;
using namespace def_type;
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
			const int FIRST_DIM_EQ = i * m_MAP_COLS + j;
			THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(FIRST_DIM_EQ, "", wxColour(255, 255, 255));
			evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);

		}
	}
}

void BFSimpl::runAlgorithm(AlgorithmThread* workingThread)
{

	std::vector<cellInfo> finalPath(m_MAP_ROWS * m_MAP_COLS, { -1, -1 });
	std::vector<bool> visited(m_MAP_ROWS * m_MAP_COLS, false);
	bfs(finalPath, visited, workingThread);
}


void BFSimpl::setBlockedCells(vector1DBool& blockedCells) {
	mapBlockedCells = blockedCells;
}


void BFSimpl::bfs(std::vector<cellInfo>& finalPath, std::vector<bool>& visited, AlgorithmThread* workingThread) {
	queue<int> q;

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

		//checking if thread was destroyed in parent
		if (!workingThread->TestDestroy()) {
			THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(curr, std::to_string(finalPath[curr].when), wxColour(204, 204, 0));
			evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);
			wxMilliSleep(100);
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
			THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(temp, "", wxColour(51, 255, 51));
			evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);
			if (finalPath[temp].parent == -1)
				return;

			else
				temp = finalPath[temp].parent;
			wxMilliSleep(100);
		}

		else {
			workingThread->flagThreadBreak(true);
			return;
		}


	}
}

bool BFSimpl::isSafe(const int& i, const int& j) {

	return ((i >= 0 && j >= 0) && (i < m_MAP_ROWS && j < m_MAP_COLS) && !mapBlockedCells[i * m_MAP_COLS + j]);

}

