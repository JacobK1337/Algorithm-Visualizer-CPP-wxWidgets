#include "DFSimpl.h"

using namespace std;
using namespace def_type;
DFSimpl::DFSimpl(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler) : GraphAlgorithm(MAP_ROWS, MAP_COLS, handler) {

}

DFSimpl::~DFSimpl() {

}

void DFSimpl::generateValues(AlgorithmThread* workingThread)
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

void DFSimpl::setSource(const int& t_newSource)
{
	m_source = t_newSource;
}

void DFSimpl::setDest(const int& t_newDest) {
	m_dest = t_newDest;
}

void DFSimpl::setBlockedCells(vector<bool>& blockedCells) {
	mapBlockedCells = blockedCells;
}

void DFSimpl::runAlgorithm(AlgorithmThread* workingThread)
{
	std::vector<cellInfo> finalPath(m_MAP_ROWS * m_MAP_COLS, {-1, -1});
	std::vector<bool> visited(m_MAP_ROWS * m_MAP_COLS, false);

	finalPath[m_source] = { m_source, 0 };
	
	dfs(m_source, finalPath, visited, workingThread);
}

void DFSimpl::dfs(const int& src, std::vector<cellInfo>& finalPath, std::vector<bool>& visited, AlgorithmThread* workingThread) {

	visited[src] = true;

	if (src != m_source && !workingThread->TestDestroy()) {

		THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(src, std::to_string(finalPath[src].when), wxColour(204, 204, 0));
		evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);
		wxMilliSleep(100);
	}

	else if (workingThread->TestDestroy()) {
		workingThread->flagThreadBreak(true);
		return;
	}

	if (src == m_dest) {
		DFSimpl::showPathToSource(finalPath, workingThread);
		return;
	}

	for(int x = -1; x <= 1; x++)
		for (int y = -1; y <= 1; y++) {

			int nextCellX = src / m_MAP_COLS + x;
			int nextCellY = src % m_MAP_COLS + y;
			int nextCellNum = nextCellX * m_MAP_COLS + nextCellY;

			if (isSafe(nextCellX, nextCellY) && !(x == 0 && y == 0) && !visited[nextCellNum]) {
				finalPath[nextCellNum].parent = src;
				finalPath[nextCellNum].when = finalPath[src].when + 1;
				dfs(nextCellNum, finalPath, visited, workingThread);
			}
				
		}

}

bool DFSimpl::isSafe(const int& i, const int& j)
{
	return ((i >= 0 && j >= 0) && (i < m_MAP_ROWS && j < m_MAP_COLS) && !mapBlockedCells[i * m_MAP_COLS + j]);

}

void DFSimpl::showPathToSource(std::vector<cellInfo>& finalPath, AlgorithmThread* workingThread) {
	
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

