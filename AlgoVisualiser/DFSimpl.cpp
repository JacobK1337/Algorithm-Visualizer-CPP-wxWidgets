#include "DFSimpl.h"

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
			const int cellNum = i * m_MAP_COLS + j;
			animation::cellColorTransition(animation::DEFAULT_COLOR_TRANS_ONSTART, 
				cellNum, 
				"", 
				0, 
				m_parentEventHandler);
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

void DFSimpl::setBlockedCells(def_type::vector1DBool& blockedCells) {
	cellBlocked = blockedCells;
}

void DFSimpl::runAlgorithm(AlgorithmThread* workingThread)
{
	std::vector<cellInfo> finalPath(m_MAP_ROWS * m_MAP_COLS, {-1, -1});
	std::vector<bool> visited(m_MAP_ROWS * m_MAP_COLS, false);

	finalPath[m_source] = { m_source, 0 };
	
	dfs(m_source, finalPath, visited, workingThread);
}

void DFSimpl::dfs(const int& src, std::vector<cellInfo>& finalPath, def_type::vector1DBool& visited, AlgorithmThread* workingThread) {

	visited[src] = true;

	if (src != m_source && !workingThread->TestDestroy()) {

		animation::cellColorTransition(animation::DEFAULT_COLOR_TRANS_YELLOW, 
			src, 
			std::to_string(finalPath[src].when), 
			animation::DEFAULT_DELAY, 
			m_parentEventHandler);

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
	return ((i >= 0 && j >= 0) && (i < m_MAP_ROWS && j < m_MAP_COLS) && !cellBlocked[i * m_MAP_COLS + j]);

}

void DFSimpl::showPathToSource(std::vector<cellInfo>& finalPath, AlgorithmThread* workingThread) {
	
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

