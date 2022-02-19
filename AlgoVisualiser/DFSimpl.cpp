#include "DFSimpl.h"

using namespace std;
using namespace def_type;
DFSimpl::DFSimpl(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler) : GraphAlgorithm(MAP_ROWS, MAP_COLS, handler) {

	adjList = make_unique<vector2DInt>(MAP_ROWS * MAP_COLS, vector1DInt());
	visList = make_unique<vector1DBool>(MAP_ROWS * MAP_COLS, false);
	ancestor = make_unique<vector1DInt>(MAP_ROWS * MAP_COLS, -1);
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
			THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(FIRST_DIM_EQ, -1, wxColour(255,255,255));
			evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);

		}
	}
}

void DFSimpl::setSource(const int& t_newSource)
{
	m_source = t_newSource;
}

int DFSimpl::getSource() {
	return m_source;
}

void DFSimpl::setBlockedCells(vector<bool>& blockedCells) {
	mapBlockedCells = blockedCells;
}

void DFSimpl::runAlgorithm(AlgorithmThread* workingThread)
{
	DFSimpl::applyAdjList();
	(*ancestor)[m_source] = m_source;
	int when = 1;
	dfs(m_source, when, workingThread);
}

void DFSimpl::dfs(const int& src, int& when, AlgorithmThread* workingThread) {

	(*visList)[src] = true;

	if (src != m_source && !workingThread->TestDestroy()) {

		THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(src, when, wxColour(204, 204, 0));
		evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);
		wxMilliSleep(100);
	}

	else if (workingThread->TestDestroy()) {
		workingThread->flagThreadBreak(true);
		return;
	}

	for (int i = 0; i < (*adjList)[src].size(); i++) {
		int curr = (*adjList)[src][i];

		if (!(*visList)[curr]) {
			(*ancestor)[curr] = src;
			dfs(curr, ++when, workingThread);
		}
	}

}

void DFSimpl::applyAdjList() {
	for (int i = 0; i < m_MAP_ROWS; i++)
		for (int j = 0; j < m_MAP_COLS; j++) {
			DFSimpl::addNeighbours(i, j);
		}

}

bool DFSimpl::isSafe(const int& i, const int& j)
{
	return ((i >= 0 && j >= 0) && (i < m_MAP_ROWS && j < m_MAP_COLS));

}



void DFSimpl::showPathToSource(const int& t_vertexFrom, AlgorithmThread* workingThread)
{
	int temp = t_vertexFrom;

	while (temp != m_source) {

		if (!workingThread->TestDestroy()) {
			THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(temp, -1, wxColour(51, 255, 51));
			evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);
			wxMilliSleep(100);
		}

		else {
			workingThread->flagThreadBreak(true);
			return;
		}

		temp = (*ancestor)[temp];
	}
}

void DFSimpl::addNeighbours(const int& i, const int& j)
{
	for (int x = -1; x <= 1; x++)
		for (int y = -1; y <= 1; y++) {
			if (!(x == 0 && y == 0) && isSafe(i + x, j + y)) {
				int currentCellNum = i * m_MAP_COLS + j;
				int neighCellNum = (i + x) * m_MAP_COLS + (j + y);

				if (!mapBlockedCells[currentCellNum] && !mapBlockedCells[neighCellNum])
					(*adjList)[currentCellNum].push_back(neighCellNum);


			}
		}
}

